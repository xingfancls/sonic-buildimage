/*! \file bcmgenl_psample.c
 *
 * BCMGENL psample callback module.
 *
 */
/*
 * $Copyright: Copyright 2018-2023 Broadcom. All rights reserved.
 * The term 'Broadcom' refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * A copy of the GNU General Public License version 2 (GPLv2) can
 * be found in the LICENSES folder.$
 */

#include <lkm/lkm.h>
#include <lkm/ngknet_kapi.h>
#include <ngknet_linux.h>

#include <linux/if_vlan.h>
#include <linux/namei.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/sched.h>

/*! \cond */
MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("BCMGENL Module");
MODULE_LICENSE("GPL");
/*! \endcond */

#include <bcmgenl.h>
#include <bcmgenl_psample.h>

#if BCMGENL_PSAMPLE_SUPPORT
#include <net/psample.h>
#define BCMGENL_PSAMPLE_NAME PSAMPLE_GENL_NAME

/* set BCMGENL_PSAMPLE_CB_DBG for debug info */
#define BCMGENL_PSAMPLE_CB_DBG
#ifdef BCMGENL_PSAMPLE_CB_DBG
static int debug;

#define DBG_LVL_VERB    0x1
#define DBG_LVL_PDMP    0x2
#define BCMGENL_PSAMPLE_DBG_VERB(...) \
    if (debug & DBG_LVL_VERB) {       \
        printk(__VA_ARGS__);          \
    }
#else
#define BCMGENL_PSAMPLE_DBG_VERB(...)
#endif



#define FCS_SZ 4

#define PSAMPLE_NLA_PADDING 4
#define PSAMPLE_PKT_HANDLED (1)

#define PSAMPLE_RATE_DFLT 1
#define PSAMPLE_SIZE_DFLT 128
static int psample_size = PSAMPLE_SIZE_DFLT;
MODULE_PARAM(psample_size, int, 0);
MODULE_PARM_DESC(psample_size,
"psample pkt size (default 128 bytes)");

#define BCMGENL_PSAMPLE_QLEN_DFLT 1024
static int bcmgenl_psample_qlen = BCMGENL_PSAMPLE_QLEN_DFLT;
MODULE_PARAM(bcmgenl_psample_qlen, int, 0);
MODULE_PARM_DESC(bcmgenl_psample_qlen, "psample queue length (default 1024 buffers)");

static bcmgenl_info_t g_bcmgenl_psample_info = {{0}};

/* Maintain sampled pkt statistics */
typedef struct psample_stats_s {
    unsigned long pkts_f_psample_cb;
    unsigned long pkts_f_psample_mod;
    unsigned long pkts_f_handled;
    unsigned long pkts_f_pass_through;
    unsigned long pkts_f_dst_mc;
    unsigned long pkts_f_dst_cpu;
    unsigned long pkts_c_qlen_cur;
    unsigned long pkts_c_qlen_hi;
    unsigned long pkts_d_qlen_max;
    unsigned long pkts_d_no_mem;
    unsigned long pkts_d_no_group;
    unsigned long pkts_d_sampling_disabled;
    unsigned long pkts_d_not_ready;
    unsigned long pkts_d_metadata;
    unsigned long pkts_d_skb;
    unsigned long pkts_d_skb_cbd;
    unsigned long pkts_d_meta_srcport;
    unsigned long pkts_d_meta_dstport;
    unsigned long pkts_d_invalid_size;
} bcmgenl_psample_stats_t;
static bcmgenl_psample_stats_t g_bcmgenl_psample_stats = {0};

typedef struct psample_meta_s {
    int trunc_size;
    int src_ifindex;
    int dst_ifindex;
    int sample_rate;
} psample_meta_t;

typedef struct psample_pkt_s {
    struct list_head list;
    psample_meta_t meta;
    struct sk_buff *skb;
#if IS_ENABLED(CONFIG_PSAMPLE)
    struct psample_group *group;
#endif /* CONFIG_PSAMPLE */
} psample_pkt_t;

typedef struct bcmgenl_psample_work_s {
    struct list_head pkt_list;
    struct work_struct wq;
    spinlock_t lock;
} bcmgenl_psample_work_t;
static bcmgenl_psample_work_t g_bcmgenl_psample_work = {{0}};

/* driver proc entry root */
static struct proc_dir_entry *psample_proc_root = NULL;

static bcmgenl_netif_t *
psample_netif_lookup_by_ifindex(int ifindex)  __attribute__ ((unused));
static bcmgenl_netif_t *
psample_netif_lookup_by_ifindex(int ifindex)
{
    struct list_head *list;
    bcmgenl_netif_t *bcmgenl_netif = NULL;
    unsigned long flags;

    /* look for port from list of available net_devices */
    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);
    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        bcmgenl_netif = (bcmgenl_netif_t*)list;
        if (bcmgenl_netif->dev->ifindex == ifindex) {
            spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);
            return bcmgenl_netif;
        }
    }
    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);
    return (NULL);
}

static bcmgenl_netif_t *
psample_netif_lookup_by_port(int port)
{
    struct list_head *list;
    bcmgenl_netif_t *bcmgenl_netif = NULL;
    unsigned long flags;

    /* look for port from list of available net_devices */
    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);
    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        bcmgenl_netif = (bcmgenl_netif_t*)list;
        if (bcmgenl_netif->port == port) {
            spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);
            return bcmgenl_netif;
        }
    }
    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);
    return (NULL);
}

static int
bcmgenl_psample_meta_get(struct sk_buff *skb, bcmgenl_pkt_t *bcmgenl_pkt, psample_meta_t *sflow_meta)
{
    int srcport, dstport, dstport_type;
    int src_ifindex = 0, dst_ifindex = 0;
    int sample_rate = 1;
    int sample_size = PSAMPLE_SIZE_DFLT;
    bcmgenl_netif_t *psample_netif = NULL;
    const struct ngknet_callback_desc *cbd;

    if (!skb || !sflow_meta) {
        printk("%s: skb or sflow_meta is NULL\n", __func__);
        return (-1);
    }
    cbd = NGKNET_SKB_CB(skb);
    /* get src and dst ports */
    srcport = bcmgenl_pkt->meta.src_port;
    dstport = bcmgenl_pkt->meta.dst_port;
    dstport_type = bcmgenl_pkt->meta.dst_port_type;
    /* SDKLT-43751: Skip check of dstport on TD4/TH4 */
    if (srcport == -1) {
        printk("%s: invalid srcport %d\n", __func__, srcport);
        return (-1);
    }

    /* find src port netif */
    if ((psample_netif = psample_netif_lookup_by_port(srcport))) {
        src_ifindex = psample_netif->dev->ifindex;
        sample_rate = psample_netif->sample_rate;
        sample_size = psample_netif->sample_size;
    } else {
        g_bcmgenl_psample_stats.pkts_d_meta_srcport++;
        BCMGENL_PSAMPLE_DBG_VERB("%s: could not find psample netif for src dev %s (ifidx %d)\n",
                                 __func__, cbd->net_dev->name, src_ifindex);
    }

    /* set generic dst type for MC pkts */
    if (dstport_type == DSTPORT_TYPE_MC) {
        g_bcmgenl_psample_stats.pkts_f_dst_mc++;
    } else if (dstport != 0) {
        /* find dst port netif for UC pkts (no need to lookup CPU port) */
        if ((psample_netif = psample_netif_lookup_by_port(dstport))) {
            dst_ifindex = psample_netif->dev->ifindex;
        } else {
            dst_ifindex = -1;
            g_bcmgenl_psample_stats.pkts_d_meta_dstport++;
            BCMGENL_PSAMPLE_DBG_VERB("%s: could not find dstport(%d)\n", __func__, dstport);
        }
    } else if (dstport == 0) {
        g_bcmgenl_psample_stats.pkts_f_dst_cpu++;
    }

    BCMGENL_PSAMPLE_DBG_VERB
        ("%s: srcport %d, dstport %d, src_ifindex %d, dst_ifindex %d\n",
         __func__, srcport, dstport, src_ifindex, dst_ifindex);

    memset(sflow_meta, 0, sizeof(psample_meta_t));
    sflow_meta->src_ifindex = src_ifindex;
    sflow_meta->dst_ifindex = dst_ifindex;
    sflow_meta->trunc_size  = sample_size;
    sflow_meta->sample_rate = sample_rate;
    return (0);
}

static struct sk_buff *
bcmgenl_psample_filter_cb(struct sk_buff *skb, ngknet_filter_t **filt)
{
    int rv = 0, dev_no, pkt_len;
    const struct ngknet_callback_desc *cbd = NULL;
    ngknet_filter_t *match_filt = NULL;
    psample_meta_t meta;
    uint8_t *pkt_ptr = NULL;
    bcmgenl_pkt_t bcmgenl_pkt;
    static uint32_t last_drop, last_alloc, last_skb;
#if IS_ENABLED(CONFIG_PSAMPLE)
    struct psample_group *group;
#endif /* CONFIG_PSAMPLE */

    if (!skb) {
        printk("%s: skb is NULL\n", __func__);
        g_bcmgenl_psample_stats.pkts_d_skb++;
        return (NULL);
    }
    cbd = NGKNET_SKB_CB(skb);
    match_filt = cbd->filt;
    /* SDKLT-43751: Get ptr offset to pkt payload to send to genetlink */
    pkt_ptr = cbd->pmd + cbd->pmd_len;
    pkt_len = skb->len - cbd->pmd_len;

    if (!cbd || !match_filt) {
        printk("%s: cbd(0x%p) or match_filt(0x%p) is NULL\n",
            __func__, cbd, match_filt);
        g_bcmgenl_psample_stats.pkts_d_skb_cbd++;
        return (skb);
    }

    /* check if this packet is sampled packet (from sample filter) */
    if (!match_filt ||
        (match_filt->dest_type != NGKNET_FILTER_DEST_T_CB) ||
        (strncmp(match_filt->desc, BCMGENL_PSAMPLE_NAME, NGKNET_FILTER_DESC_MAX) != 0)) {
        return (skb);
    }
    dev_no = cbd->dinfo->dev_no;

    BCMGENL_PSAMPLE_DBG_VERB
        ("pkt size %d, match_filt->dest_id %d\n", cbd->pkt_len, match_filt->dest_id);
    BCMGENL_PSAMPLE_DBG_VERB
        ("filter user data: 0x%08x\n", *(uint32_t *)match_filt->user_data);
    BCMGENL_PSAMPLE_DBG_VERB
        ("filter_cb for dev %d: %s\n", dev_no, cbd->dinfo->type_str);
    g_bcmgenl_psample_stats.pkts_f_psample_cb++;

#if IS_ENABLED(CONFIG_PSAMPLE)
    /* get psample group info. psample genetlink group ID passed in match_filt->dest_id */
    group = psample_group_get(g_bcmgenl_psample_info.netns, match_filt->dest_id);
    if (!group) {
        printk("%s: Could not find psample genetlink group %d\n", __func__, match_filt->dest_id);
        g_bcmgenl_psample_stats.pkts_d_no_group++;
        goto PSAMPLE_FILTER_CB_PKT_HANDLED;
    }
#endif /* CONFIG_PSAMPLE */
    /* get packet metadata */
    rv = bcmgenl_pkt_package(dev_no, skb,
                             &g_bcmgenl_psample_info,
                             &bcmgenl_pkt);
    if (rv < 0) {
        printk("%s: Could not parse pkt metadata\n", __func__);
        g_bcmgenl_psample_stats.pkts_d_metadata++;
        goto PSAMPLE_FILTER_CB_PKT_HANDLED;
    }

    BCMGENL_PSAMPLE_DBG_VERB
        ("%s: netns 0x%p, src_port %d, dst_port %d, dst_port_type %x\n",
         __func__,
         bcmgenl_pkt.netns,
         bcmgenl_pkt.meta.src_port,
         bcmgenl_pkt.meta.dst_port,
         bcmgenl_pkt.meta.dst_port_type);

    /* get psample metadata */
    rv = bcmgenl_psample_meta_get(skb, &bcmgenl_pkt, &meta);
    if (rv < 0) {
        printk("%s: Could not parse pkt metadata\n", __func__);
        g_bcmgenl_psample_stats.pkts_d_metadata++;
        goto PSAMPLE_FILTER_CB_PKT_HANDLED;
    }

    /* Adjust original pkt pkt_len to remove 4B FCS */
    if (pkt_len < FCS_SZ) {
        g_bcmgenl_psample_stats.pkts_d_invalid_size++;
        goto PSAMPLE_FILTER_CB_PKT_HANDLED;
    } else {
       pkt_len -= FCS_SZ;
    }

    /* Account for padding in libnl used by psample */
    if (meta.trunc_size >= pkt_len) {
        meta.trunc_size = pkt_len - PSAMPLE_NLA_PADDING;
    }
    BCMGENL_PSAMPLE_DBG_VERB
        ("%s: trunc_size %d, sample_rate %d "
         "src_ifindex %d, dst_ifindex %d\n",
         __func__, meta.trunc_size, meta.sample_rate,
         meta.src_ifindex, meta.dst_ifindex);
#if IS_ENABLED(CONFIG_PSAMPLE)
    BCMGENL_PSAMPLE_DBG_VERB
        ("%s: group 0x%x\n", __func__, group->group_num);
#endif /* CONFIG_PSAMPLE */

    /* drop if configured sample rate is 0 */
    if (meta.sample_rate > 0) {
        unsigned long flags;
        psample_pkt_t *psample_pkt;
        struct sk_buff *skb_psample;

        if (g_bcmgenl_psample_stats.pkts_c_qlen_cur >= bcmgenl_psample_qlen) {
            g_bcmgenl_psample_stats.pkts_d_qlen_max++;
            last_drop = 0;
            bcmgenl_limited_gprintk
                (last_drop, "%s: tail drop due to max qlen %d reached: %lu\n",
                 __func__, bcmgenl_psample_qlen,
                 g_bcmgenl_psample_stats.pkts_d_qlen_max);
            goto PSAMPLE_FILTER_CB_PKT_HANDLED;
        }

        if ((psample_pkt = kmalloc(sizeof(psample_pkt_t), GFP_ATOMIC)) == NULL) {
            g_bcmgenl_psample_stats.pkts_d_no_mem++;
            last_alloc = 0;
            bcmgenl_limited_gprintk
                (last_alloc, "%s: failed to alloc generic mem for pkt: %lu\n",
                 __func__, g_bcmgenl_psample_stats.pkts_d_no_mem);
            goto PSAMPLE_FILTER_CB_PKT_HANDLED;
        }
        memcpy(&psample_pkt->meta, &meta, sizeof(psample_meta_t));
#if IS_ENABLED(CONFIG_PSAMPLE)
        psample_pkt->group = group;
#endif /* CONFIG_PSAMPLE */
        if ((skb_psample = dev_alloc_skb(meta.trunc_size)) == NULL) {
            g_bcmgenl_psample_stats.pkts_d_no_mem++;
            last_skb = 0;
            bcmgenl_limited_gprintk
                (last_skb, "%s: failed to alloc generic mem for pkt skb: %lu\n",
                 __func__, g_bcmgenl_psample_stats.pkts_d_no_mem);
            goto PSAMPLE_FILTER_CB_PKT_HANDLED;
        }

        /* SDKLT-43751: Use ptr offset to pkt payload to send to genetlink */
        /* setup skb to point to pkt */
        memcpy(skb_psample->data, pkt_ptr, meta.trunc_size);
        skb_put(skb_psample, meta.trunc_size);
        skb_psample->len = meta.trunc_size;
        psample_pkt->skb = skb_psample;

        spin_lock_irqsave(&g_bcmgenl_psample_work.lock, flags);
        list_add_tail(&psample_pkt->list, &g_bcmgenl_psample_work.pkt_list);

        g_bcmgenl_psample_stats.pkts_c_qlen_cur++;
        if (g_bcmgenl_psample_stats.pkts_c_qlen_cur > g_bcmgenl_psample_stats.pkts_c_qlen_hi) {
            g_bcmgenl_psample_stats.pkts_c_qlen_hi = g_bcmgenl_psample_stats.pkts_c_qlen_cur;
        }

        schedule_work(&g_bcmgenl_psample_work.wq);
        spin_unlock_irqrestore(&g_bcmgenl_psample_work.lock, flags);
    } else {
        g_bcmgenl_psample_stats.pkts_d_sampling_disabled++;
    }

PSAMPLE_FILTER_CB_PKT_HANDLED:
    g_bcmgenl_psample_stats.pkts_f_pass_through++;
    return skb;
}

static void
bcmgenl_psample_task(struct work_struct *work)
{
    bcmgenl_psample_work_t *psample_work = container_of(work, bcmgenl_psample_work_t, wq);
    unsigned long flags;
    struct list_head *list_ptr, *list_next;
    psample_pkt_t *pkt;

    spin_lock_irqsave(&psample_work->lock, flags);
    list_for_each_safe(list_ptr, list_next, &psample_work->pkt_list) {
        /* dequeue pkt from list */
        pkt = list_entry(list_ptr, psample_pkt_t, list);
        list_del(list_ptr);
        g_bcmgenl_psample_stats.pkts_c_qlen_cur--;
        spin_unlock_irqrestore(&psample_work->lock, flags);

        /* send generic_pkt to generic netlink */
        if (pkt) {
            BCMGENL_PSAMPLE_DBG_VERB
                ("%s: trunc_size %d, sample_rate %d,"
                 "src_ifindex %d, dst_ifindex %d\n",
                 __func__, pkt->meta.trunc_size, pkt->meta.sample_rate,
                 pkt->meta.src_ifindex, pkt->meta.dst_ifindex);
#if IS_ENABLED(CONFIG_PSAMPLE)
            BCMGENL_PSAMPLE_DBG_VERB
                ("%s: group 0x%x\n", __func__, pkt->group->group_num);
#endif /* CONFIG_PSAMPLE */
            g_bcmgenl_psample_stats.pkts_f_psample_mod++;

            dev_kfree_skb_any(pkt->skb);
            kfree(pkt);
        }
        spin_lock_irqsave(&psample_work->lock, flags);
    }
    spin_unlock_irqrestore(&psample_work->lock, flags);
}

static int
bcmgenl_psample_netif_create_cb(ngknet_dev_info_t *dinfo, ngknet_netif_t *netif)
{
    bool found;
    struct list_head *list;
    bcmgenl_netif_t *new_netif, *lbcmgenl_netif;
    unsigned long flags;

    if (!dinfo) {
        printk("%s: dinfo is NULL\n", __func__);
        return (-1);
    }
    if (netif->id == 0) {
        printk("%s: netif->id == 0 is not a valid interface ID\n", __func__);
        return (-1);
    }
    if ((new_netif = kmalloc(sizeof(bcmgenl_netif_t), GFP_ATOMIC)) == NULL) {
        printk("%s: failed to alloc psample mem for netif '%s'\n",
               __func__, netif->name);
        return (-1);
    }

    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);
    new_netif->dev = dinfo->vdev[netif->id];
    new_netif->id = netif->id;
    new_netif->port = netif->port;
    new_netif->vlan = netif->vlan;
    new_netif->sample_rate = PSAMPLE_RATE_DFLT;
    new_netif->sample_size = PSAMPLE_SIZE_DFLT;

    /* insert netif sorted by ID similar to ngknet_netif_create() */
    found = false;
    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        lbcmgenl_netif = (bcmgenl_netif_t*)list;
        if (netif->id < lbcmgenl_netif->id) {
            found = true;
            g_bcmgenl_psample_info.netif_count++;
            break;
        }
    }

    if (found) {
        /* Replace previously removed interface */
        list_add_tail(&new_netif->list, &lbcmgenl_netif->list);
    } else {
        /* No holes - add to end of list */
        list_add_tail(&new_netif->list, &g_bcmgenl_psample_info.netif_list);
    }

    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);

    BCMGENL_PSAMPLE_DBG_VERB
        ("%s: added netlink psample netif '%s'\n", __func__, netif->name);
    return (0);
}

static int
bcmgenl_psample_netif_destroy_cb(ngknet_dev_info_t *dinfo, ngknet_netif_t *netif)
{
    bool found = false;
    struct list_head *list;
    bcmgenl_netif_t *lbcmgenl_netif;
    unsigned long flags;

    if (!dinfo || !netif) {
        printk("%s: dinfo or netif is NULL\n", __func__);
        return (-1);
    }

    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);

    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        lbcmgenl_netif = (bcmgenl_netif_t *)list;
        if (netif->id == lbcmgenl_netif->id) {
            found = true;
            list_del(&lbcmgenl_netif->list);
            BCMGENL_PSAMPLE_DBG_VERB
                ("%s: removing psample netif '%s'\n", __func__, netif->name);
            kfree(lbcmgenl_netif);
            g_bcmgenl_psample_info.netif_count--;
            break;
        }
    }

    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);

    if (!found) {
        printk("%s: netif ID %d not found!\n", __func__, netif->id);
        return (-1);
    }
    return (0);
}

/*
 * psample rate Proc Read Entry
 */
static int
bcmgenl_psample_proc_rate_show(struct seq_file *m, void *v)
{
    struct list_head *list;
    bcmgenl_netif_t *psample_netif;
    unsigned long flags;

    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);

    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        psample_netif = (bcmgenl_netif_t*)list;
        seq_printf(m, "  %-14s %d\n", psample_netif->dev->name, psample_netif->sample_rate);
    }

    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);

    return 0;
}

static int
bcmgenl_psample_proc_rate_open(struct inode * inode, struct file * file)
{
    return single_open(file, bcmgenl_psample_proc_rate_show, NULL);
}

/*
 * psample rate Proc Write Entry
 *
 *   Syntax:
 *   <netif>=<pkt sample rate>
 *
 *   Where <netif> is a virtual network interface name.
 *
 *   Examples:
 *   eth4=1000
 */
static ssize_t
bcmgenl_psample_proc_rate_write(
    struct file *file, const char *buf,
    size_t count, loff_t *loff)
{
    bool found;
    struct list_head *list;
    bcmgenl_netif_t *psample_netif;
    char sample_str[40], *ptr, *newline;
    unsigned long flags;


    if (count > sizeof(sample_str)) {
        count = sizeof(sample_str) - 1;
        sample_str[count] = '\0';
    }
    if (copy_from_user(sample_str, buf, count)) {
        return -EFAULT;
    }
    sample_str[count] = 0;
    newline = strchr(sample_str, '\n');
    if (newline) {
        /* Chop off the trailing newline */
        *newline = '\0';
    }

    if ((ptr = strchr(sample_str, '=')) == NULL &&
        (ptr = strchr(sample_str, ':')) == NULL) {
        printk("Error: Pkt sample rate syntax not recognized: '%s'\n",
               sample_str);
        return count;
    }
    *ptr++ = 0;

    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);

    found = false;
    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        psample_netif = (bcmgenl_netif_t*)list;
        if (strcmp(psample_netif->dev->name, sample_str) == 0) {
            psample_netif->sample_rate = simple_strtol(ptr, NULL, 10);
            found = true;
            break;
        }
    }

    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);

    if (!found) {
        printk("Warning: Failed setting psample rate on unknown network"
               "interface: '%s'\n", sample_str);
    }
    return count;
}

struct proc_ops bcmgenl_psample_proc_rate_file_ops = {
    PROC_OWNER(THIS_MODULE)
    .proc_open =       bcmgenl_psample_proc_rate_open,
    .proc_read =       seq_read,
    .proc_lseek =      seq_lseek,
    .proc_write =       bcmgenl_psample_proc_rate_write,
    .proc_release =    single_release,
};

/*
 * psample size Proc Read Entry
 */
static int
bcmgenl_psample_proc_size_show(struct seq_file *m, void *v)
{
    struct list_head *list;
    bcmgenl_netif_t *psample_netif;
    unsigned long flags;

    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);

    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        psample_netif = (bcmgenl_netif_t*)list;
        seq_printf(m, "  %-14s %d\n", psample_netif->dev->name, psample_netif->sample_size);
    }

    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);
    return 0;
}

static int
bcmgenl_psample_proc_size_open(struct inode * inode, struct file * file)
{
    return single_open(file, bcmgenl_psample_proc_size_show, NULL);
}

/*
 * psample size Proc Write Entry
 *
 *   Syntax:
 *   <netif>=<pkt sample size in bytes>
 *
 *   Where <netif> is a virtual network interface name.
 *
 *   Examples:
 *   eth4=128
 */
static ssize_t
bcmgenl_psample_proc_size_write(
    struct file *file, const char *buf,
    size_t count, loff_t *loff)
{
    bool found;
    struct list_head *list;
    bcmgenl_netif_t *bcmgenl_netif;
    char sample_str[40], *ptr, *newline;
    unsigned long flags;

    if (count > sizeof(sample_str)) {
        count = sizeof(sample_str) - 1;
        sample_str[count] = '\0';
    }
    if (copy_from_user(sample_str, buf, count)) {
        return -EFAULT;
    }
    sample_str[count] = 0;
    newline = strchr(sample_str, '\n');
    if (newline) {
        /* Chop off the trailing newline */
        *newline = '\0';
    }

    if ((ptr = strchr(sample_str, '=')) == NULL &&
        (ptr = strchr(sample_str, ':')) == NULL) {
        printk("Error: Pkt sample size syntax not recognized: '%s'\n", sample_str);
        return count;
    }
    *ptr++ = 0;

    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);

    found = false;
    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        bcmgenl_netif = (bcmgenl_netif_t*)list;
        if (strcmp(bcmgenl_netif->dev->name, sample_str) == 0) {
            bcmgenl_netif->sample_size = simple_strtol(ptr, NULL, 10);
            found = true;
            break;
        }
    }

    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);

    if (!found) {
        printk("Warning: Failed setting psample size on unknown network interface: '%s'\n", sample_str);
    }
    return count;
}

struct proc_ops bcmgenl_psample_proc_size_file_ops = {
    PROC_OWNER(THIS_MODULE)
    .proc_open =       bcmgenl_psample_proc_size_open,
    .proc_read =       seq_read,
    .proc_lseek =      seq_lseek,
    .proc_write =       bcmgenl_psample_proc_size_write,
    .proc_release =    single_release,
};

/*
 * psample map Proc Read Entry
 */
static int
bcmgenl_psample_proc_map_show(struct seq_file *m, void *v)
{
    struct list_head *list;
    bcmgenl_netif_t *bcmgenl_netif;
    unsigned long flags;

    seq_printf(m, "  Interface      logical port   ifindex\n");
    seq_printf(m, "-------------    ------------   -------\n");
    spin_lock_irqsave(&g_bcmgenl_psample_info.lock, flags);

    list_for_each(list, &g_bcmgenl_psample_info.netif_list) {
        bcmgenl_netif = (bcmgenl_netif_t*)list;
        seq_printf(m, "  %-14s %-14d %d\n",
                bcmgenl_netif->dev->name,
                bcmgenl_netif->port,
                bcmgenl_netif->dev->ifindex);
    }

    spin_unlock_irqrestore(&g_bcmgenl_psample_info.lock, flags);
    return 0;
}

static int
bcmgenl_psample_proc_map_open(struct inode * inode, struct file * file)
{
    return single_open(file, bcmgenl_psample_proc_map_show, NULL);
}

struct proc_ops bcmgenl_psample_proc_map_file_ops = {
    PROC_OWNER(THIS_MODULE)
    .proc_open =       bcmgenl_psample_proc_map_open,
    .proc_read =       seq_read,
    .proc_lseek =      seq_lseek,
    .proc_write =       NULL,
    .proc_release =    single_release,
};

/*
 * psample debug Proc Read Entry
 */
static int
bcmgenl_psample_proc_debug_show(struct seq_file *m, void *v)
{
    seq_printf(m, "BCM KNET %s Callback Config\n", BCMGENL_PSAMPLE_NAME);
    seq_printf(m, "  debug:           0x%x\n", debug);
    seq_printf(m, "  netif_count:     %d\n",   g_bcmgenl_psample_info.netif_count);
    seq_printf(m, "  queue length:    %d\n",   bcmgenl_psample_qlen);

    return 0;
}

static int
bcmgenl_psample_proc_debug_open(struct inode * inode, struct file * file)
{
    return single_open(file, bcmgenl_psample_proc_debug_show, NULL);
}

/*
 * psample debug Proc Write Entry
 *
 *   Syntax:
 *   debug=<mask>
 *
 *   Where <mask> corresponds to the debug module parameter.
 *
 *   Examples:
 *   debug=0x1
 */
static ssize_t
bcmgenl_psample_proc_debug_write(
    struct file *file, const char *buf,
    size_t count, loff_t *loff)
{
    char debug_str[40];
    char *ptr;

    if (count > sizeof(debug_str)) {
        count = sizeof(debug_str) - 1;
        debug_str[count] = '\0';
    }
    if (copy_from_user(debug_str, buf, count)) {
        return -EFAULT;
    }

    if ((ptr = strstr(debug_str, "debug=")) != NULL) {
        ptr += 6;
        debug = simple_strtol(ptr, NULL, 0);
    } else {
        printk("Warning: unknown configuration setting\n");
    }

    return count;
}

struct proc_ops bcmgenl_psample_proc_debug_file_ops = {
    PROC_OWNER(THIS_MODULE)
    .proc_open =       bcmgenl_psample_proc_debug_open,
    .proc_read =       seq_read,
    .proc_lseek =      seq_lseek,
    .proc_write =      bcmgenl_psample_proc_debug_write,
    .proc_release =    single_release,
};

static int
bcmgenl_psample_proc_stats_show(struct seq_file *m, void *v)
{
    seq_printf(m, "BCM KNET %s Callback Stats\n", BCMGENL_PSAMPLE_NAME);
    seq_printf(m, "  pkts filter psample cb         %10lu\n", g_bcmgenl_psample_stats.pkts_f_psample_cb);
    seq_printf(m, "  pkts sent to psample module    %10lu\n", g_bcmgenl_psample_stats.pkts_f_psample_mod);
    seq_printf(m, "  pkts handled by psample        %10lu\n", g_bcmgenl_psample_stats.pkts_f_handled);
    seq_printf(m, "  pkts pass through              %10lu\n", g_bcmgenl_psample_stats.pkts_f_pass_through);
    seq_printf(m, "  pkts with mc destination       %10lu\n", g_bcmgenl_psample_stats.pkts_f_dst_mc);
    seq_printf(m, "  pkts current queue length      %10lu\n", g_bcmgenl_psample_stats.pkts_c_qlen_cur);
    seq_printf(m, "  pkts high queue length         %10lu\n", g_bcmgenl_psample_stats.pkts_c_qlen_hi);
    seq_printf(m, "  pkts drop max queue length     %10lu\n", g_bcmgenl_psample_stats.pkts_d_qlen_max);
    seq_printf(m, "  pkts drop no memory            %10lu\n", g_bcmgenl_psample_stats.pkts_d_no_mem);
    seq_printf(m, "  pkts drop no psample group     %10lu\n", g_bcmgenl_psample_stats.pkts_d_no_group);
    seq_printf(m, "  pkts drop sampling disabled    %10lu\n", g_bcmgenl_psample_stats.pkts_d_sampling_disabled);
    seq_printf(m, "  pkts drop psample not ready    %10lu\n", g_bcmgenl_psample_stats.pkts_d_not_ready);
    seq_printf(m, "  pkts drop metadata parse error %10lu\n", g_bcmgenl_psample_stats.pkts_d_metadata);
    seq_printf(m, "  pkts drop skb error            %10lu\n", g_bcmgenl_psample_stats.pkts_d_skb);
    seq_printf(m, "  pkts drop skb cbd error        %10lu\n", g_bcmgenl_psample_stats.pkts_d_skb_cbd);
    seq_printf(m, "  pkts with invalid src port     %10lu\n", g_bcmgenl_psample_stats.pkts_d_meta_srcport);
    seq_printf(m, "  pkts with invalid dst port     %10lu\n", g_bcmgenl_psample_stats.pkts_d_meta_dstport);
    seq_printf(m, "  pkts with invalid orig pkt sz  %10lu\n", g_bcmgenl_psample_stats.pkts_d_invalid_size);
    return 0;
}

static int
bcmgenl_psample_proc_stats_open(struct inode * inode, struct file * file)
{
    return single_open(file, bcmgenl_psample_proc_stats_show, NULL);
}

/*
 * psample stats Proc Write Entry
 *
 *   Syntax:
 *   write any value to clear stats
 */
static ssize_t
bcmgenl_psample_proc_stats_write(struct file *file, const char *buf,
                    size_t count, loff_t *loff)
{
    int qlen_cur = 0;
    unsigned long flags;

    spin_lock_irqsave(&g_bcmgenl_psample_work.lock, flags);
    qlen_cur = g_bcmgenl_psample_stats.pkts_c_qlen_cur;
    memset(&g_bcmgenl_psample_stats, 0, sizeof(bcmgenl_psample_stats_t));
    g_bcmgenl_psample_stats.pkts_c_qlen_cur = qlen_cur;
    spin_unlock_irqrestore(&g_bcmgenl_psample_work.lock, flags);

    return count;
}
struct proc_ops bcmgenl_psample_proc_stats_file_ops = {
    PROC_OWNER(THIS_MODULE)
    .proc_open =       bcmgenl_psample_proc_stats_open,
    .proc_read =       seq_read,
    .proc_lseek =      seq_lseek,
    .proc_write =      bcmgenl_psample_proc_stats_write,
    .proc_release =    single_release,
};

static int
psample_cb_proc_cleanup(void)
{
    remove_proc_entry("stats", psample_proc_root);
    remove_proc_entry("rate",  psample_proc_root);
    remove_proc_entry("size",  psample_proc_root);
    remove_proc_entry("debug", psample_proc_root);
    remove_proc_entry("map"  , psample_proc_root);

    proc_remove(psample_proc_root);
    return 0;
}

static int
psample_cb_proc_init(void)
{
    char psample_procfs_path[PROCFS_MAX_PATH];
    struct proc_dir_entry *entry;

    /* create procfs for psample */
    snprintf(psample_procfs_path, PROCFS_MAX_PATH, "%s/%s",
             BCMGENL_PROCFS_PATH, BCMGENL_PSAMPLE_NAME);
    psample_proc_root = proc_mkdir(psample_procfs_path, NULL);

    /* create procfs for psample stats */
    PROC_CREATE(entry, "stats", 0666, psample_proc_root,
                &bcmgenl_psample_proc_stats_file_ops);
    if (entry == NULL) {
        printk("%s: Unable to create procfs entry '/procfs/%s/stats'\n",
               __func__, psample_procfs_path);
        return -1;
    }

    /* create procfs for setting sample rates */
    PROC_CREATE(entry, "rate", 0666, psample_proc_root,
                &bcmgenl_psample_proc_rate_file_ops);
    if (entry == NULL) {
        printk("%s: Unable to create procfs entry '/procfs/%s/rate'\n",
               __func__, psample_procfs_path);
        return -1;
    }

    /* create procfs for setting sample size */
    PROC_CREATE(entry, "size", 0666, psample_proc_root,
                &bcmgenl_psample_proc_size_file_ops);
    if (entry == NULL) {
        printk("%s: Unable to create procfs entry '/procfs/%s/size'\n",
               __func__, psample_procfs_path);
        return -1;
    }

    /* create procfs for getting netdev mapping */
    PROC_CREATE(entry, "map", 0666, psample_proc_root,
                &bcmgenl_psample_proc_map_file_ops);
    if (entry == NULL) {
        printk("%s: Unable to create procfs entry '/procfs/%s/map'\n",
               __func__, psample_procfs_path);
        return -1;
    }

    /* create procfs for debug log */
    PROC_CREATE(entry, "debug", 0666, psample_proc_root,
                &bcmgenl_psample_proc_debug_file_ops);
    if (entry == NULL) {
        printk("%s: Unable to create procfs entry '/procfs/%s/debug'\n",
               __func__, psample_procfs_path);
        return -1;
    }
    return 0;
}

static int
psample_cb_cleanup(void)
{
    psample_pkt_t *pkt;

    cancel_work_sync(&g_bcmgenl_psample_work.wq);

    while (!list_empty(&g_bcmgenl_psample_work.pkt_list)) {
        pkt = list_entry(g_bcmgenl_psample_work.pkt_list.next,
                         psample_pkt_t, list);
        list_del(&pkt->list);
        dev_kfree_skb_any(pkt->skb);
        kfree(pkt);
    }

    return 0;
}

static int
psample_cb_init(void)
{
    /* clear data structs */
    memset(&g_bcmgenl_psample_stats, 0, sizeof(bcmgenl_psample_stats_t));
    memset(&g_bcmgenl_psample_info, 0, sizeof(bcmgenl_info_t));
    memset(&g_bcmgenl_psample_work, 0, sizeof(bcmgenl_psample_work_t));

    /* setup psample_info struct */
    INIT_LIST_HEAD(&g_bcmgenl_psample_info.netif_list);
    spin_lock_init(&g_bcmgenl_psample_info.lock);

    /* setup psample work queue */
    spin_lock_init(&g_bcmgenl_psample_work.lock);
    INIT_LIST_HEAD(&g_bcmgenl_psample_work.pkt_list);
    INIT_WORK(&g_bcmgenl_psample_work.wq, bcmgenl_psample_task);

    /* get net namespace */
    g_bcmgenl_psample_info.netns = get_net_ns_by_pid(current->pid);
    if (!g_bcmgenl_psample_info.netns) {
        printk("%s: Could not get network namespace for pid %d\n",
               __func__, current->pid);
        return (-1);
    }
    BCMGENL_PSAMPLE_DBG_VERB
        ("%s: current->pid %d, netns 0x%p, sample_size %d\n",
         __func__, current->pid, g_bcmgenl_psample_info.netns, psample_size);
    return 0;
}

int bcmgenl_psample_cleanup(void)
{
    ngknet_netif_create_cb_unregister(bcmgenl_psample_netif_create_cb);
    ngknet_netif_destroy_cb_unregister(bcmgenl_psample_netif_destroy_cb);
    ngknet_filter_cb_unregister(bcmgenl_psample_filter_cb);

    psample_cb_cleanup();
    psample_cb_proc_cleanup();
    return 0;
}

int bcmgenl_psample_init(void)
{
    ngknet_netif_create_cb_register(bcmgenl_psample_netif_create_cb);
    ngknet_netif_destroy_cb_register(bcmgenl_psample_netif_destroy_cb);
    ngknet_filter_cb_register_by_name
        (bcmgenl_psample_filter_cb, BCMGENL_PSAMPLE_NAME);

    psample_cb_proc_init();
    return psample_cb_init();
}

#else
int bcmgenl_psample_cleanup(void)
{
    return 0;
}

int bcmgenl_psample_init(void)
{
    return 0;
}
#endif /* BCMGENL_PSAMPLE_SUPPORT */
EXPORT_SYMBOL(bcmgenl_psample_cleanup);
EXPORT_SYMBOL(bcmgenl_psample_init);
