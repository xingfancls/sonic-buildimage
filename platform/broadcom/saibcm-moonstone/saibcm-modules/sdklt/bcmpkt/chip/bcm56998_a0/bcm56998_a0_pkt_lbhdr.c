/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 * Tool: INTERNAL/regs/xgs/generate-pmd.pl
 *
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
 *
 * This file provides LBHDR access functions for BCM56998_A0.
 *
 ******************************************************************************/

#include <sal/sal_types.h>
#include <bcmpkt/bcmpkt_lbhdr_internal.h>

#define MASK(_bn) (((uint32_t)0x1<<(_bn))-1)
#define WORD_FIELD_GET(_d,_s,_l) (((_d) >> (_s)) & MASK(_l))
#define WORD_FIELD_SET(_d,_s,_l,_v) (_d)=(((_d) & ~(MASK(_l) << (_s))) | (((_v) & MASK(_l)) << (_s)))
#define WORD_FIELD_MASK(_d,_s,_l) (_d)=((_d) | (MASK(_l) << (_s)))
/*******************************************************************************
 * SWFORMAT:  LBHDR
 * BLOCKS:
 * SIZE:     128
 ******************************************************************************/
static void bcmpkt_lbhdr_start_set(uint32_t *data, uint32_t val)
{
    WORD_FIELD_SET(data[0], 24, 8, val);
}

static uint32_t bcmpkt_lbhdr_start_get(uint32_t *data)
{
    uint32_t val;
    val = WORD_FIELD_GET(data[0], 24, 8);
    return val;
}

static void bcmpkt_lbhdr_header_type_set(uint32_t *data, uint32_t val)
{
    WORD_FIELD_SET(data[0], 15, 5, val);
}

static uint32_t bcmpkt_lbhdr_header_type_get(uint32_t *data)
{
    uint32_t val;
    val = WORD_FIELD_GET(data[0], 15, 5);
    return val;
}

static void bcmpkt_lbhdr_input_priority_set(uint32_t *data, uint32_t val)
{
    WORD_FIELD_SET(data[0], 20, 4, val);
}

static uint32_t bcmpkt_lbhdr_input_priority_get(uint32_t *data)
{
    uint32_t val;
    val = WORD_FIELD_GET(data[0], 20, 4);
    return val;
}

static void bcmpkt_lbhdr_pkt_profile_set(uint32_t *data, uint32_t val)
{
    WORD_FIELD_SET(data[1], 26, 3, val);
}

static uint32_t bcmpkt_lbhdr_pkt_profile_get(uint32_t *data)
{
    uint32_t val;
    val = WORD_FIELD_GET(data[1], 26, 3);
    return val;
}

static void bcmpkt_lbhdr_visibility_pkt_set(uint32_t *data, uint32_t val)
{
    WORD_FIELD_SET(data[1], 29, 1, val);
}

static uint32_t bcmpkt_lbhdr_visibility_pkt_get(uint32_t *data)
{
    uint32_t val;
    val = WORD_FIELD_GET(data[1], 29, 1);
    return val;
}

static void bcmpkt_lbhdr_source_set(uint32_t *data, uint32_t val)
{
    WORD_FIELD_SET(data[1], 30, 2, val & MASK(2));
    WORD_FIELD_SET(data[0], 0, 14, (val >> 2) & MASK(14));
}

static uint32_t bcmpkt_lbhdr_source_get(uint32_t *data)
{
    uint32_t val;
    val = (WORD_FIELD_GET(data[1], 30, 2) | (WORD_FIELD_GET(data[0], 0, 14) << 2)) ;
    return val;
}

static void bcmpkt_lbhdr_source_type_set(uint32_t *data, uint32_t val)
{
    WORD_FIELD_SET(data[0], 14, 1, val);
}

static uint32_t bcmpkt_lbhdr_source_type_get(uint32_t *data)
{
    uint32_t val;
    val = WORD_FIELD_GET(data[0], 14, 1);
    return val;
}

static void bcmpkt_lbhdr_trill_ac_cng_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_TRILL_AC) {
        return;
    }
    WORD_FIELD_SET(data[3], 4, 2, val);
}

static uint32_t bcmpkt_lbhdr_trill_ac_cng_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_TRILL_AC) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 4, 2);
    return val;
}

static void bcmpkt_lbhdr_trill_ac_tc_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_TRILL_AC) {
        return;
    }
    WORD_FIELD_SET(data[3], 0, 4, val);
}

static uint32_t bcmpkt_lbhdr_trill_ac_tc_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_TRILL_AC) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 0, 4);
    return val;
}

static void bcmpkt_lbhdr_trill_nw_cng_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_TRILL_NW) {
        return;
    }
    WORD_FIELD_SET(data[3], 4, 2, val);
}

static uint32_t bcmpkt_lbhdr_trill_nw_cng_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_TRILL_NW) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 4, 2);
    return val;
}

static void bcmpkt_lbhdr_trill_nw_tc_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_TRILL_NW) {
        return;
    }
    WORD_FIELD_SET(data[3], 0, 4, val);
}

static uint32_t bcmpkt_lbhdr_trill_nw_tc_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_TRILL_NW) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 0, 4);
    return val;
}

static void bcmpkt_lbhdr_eth_dest_type_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[3], 11, 4, val);
}

static uint32_t bcmpkt_lbhdr_eth_dest_type_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 11, 4);
    return val;
}

static void bcmpkt_lbhdr_eth_dest_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[3], 15, 16, val);
}

static uint32_t bcmpkt_lbhdr_eth_dest_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 15, 16);
    return val;
}

static void bcmpkt_lbhdr_eth_dp_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[2], 7, 2, val);
}

static uint32_t bcmpkt_lbhdr_eth_dp_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[2], 7, 2);
    return val;
}

static void bcmpkt_lbhdr_eth_ecmp_member_id_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[2], 9, 14, val);
}

static uint32_t bcmpkt_lbhdr_eth_ecmp_member_id_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[2], 9, 14);
    return val;
}

static void bcmpkt_lbhdr_eth_int_cn_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[2], 1, 2, val);
}

static uint32_t bcmpkt_lbhdr_eth_int_cn_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[2], 1, 2);
    return val;
}

static void bcmpkt_lbhdr_eth_int_pri_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[2], 3, 4, val);
}

static uint32_t bcmpkt_lbhdr_eth_int_pri_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[2], 3, 4);
    return val;
}

static void bcmpkt_lbhdr_eth_mcast_lb_index_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[1], 6, 8, val);
}

static uint32_t bcmpkt_lbhdr_eth_mcast_lb_index_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[1], 6, 8);
    return val;
}

static void bcmpkt_lbhdr_eth_mcast_lb_index_vld_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[1], 14, 1, val);
}

static uint32_t bcmpkt_lbhdr_eth_mcast_lb_index_vld_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[1], 14, 1);
    return val;
}

static void bcmpkt_lbhdr_eth_pp_port_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[3], 0, 9, val);
}

static uint32_t bcmpkt_lbhdr_eth_pp_port_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 0, 9);
    return val;
}

static void bcmpkt_lbhdr_eth_qos_fields_vld_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[2], 0, 1, val);
}

static uint32_t bcmpkt_lbhdr_eth_qos_fields_vld_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[2], 0, 1);
    return val;
}

static void bcmpkt_lbhdr_eth_routed_pkt_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[3], 31, 1, val);
}

static uint32_t bcmpkt_lbhdr_eth_routed_pkt_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 31, 1);
    return val;
}

static void bcmpkt_lbhdr_eth_subflow_type_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[3], 9, 2, val);
}

static uint32_t bcmpkt_lbhdr_eth_subflow_type_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[3], 9, 2);
    return val;
}

static void bcmpkt_lbhdr_ethernet_vrf_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[2], 23, 9, val & MASK(9));
    WORD_FIELD_SET(data[1], 0, 4, (val >> 9) & MASK(4));
}

static uint32_t bcmpkt_lbhdr_ethernet_vrf_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = (WORD_FIELD_GET(data[2], 23, 9) | (WORD_FIELD_GET(data[1], 0, 4) << 9)) ;
    return val;
}

static void bcmpkt_lbhdr_ethernet_vrf_valid_set(uint32_t *data, uint32_t val)
{
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return;
    }
    WORD_FIELD_SET(data[1], 4, 1, val);
}

static uint32_t bcmpkt_lbhdr_ethernet_vrf_valid_get(uint32_t *data)
{
    uint32_t val;
    if (bcmpkt_lbhdr_header_type_get(data) != BCMPKT_LBHDR_HEADER_T_ETHERNET) {
        return 0;
    }
    val = WORD_FIELD_GET(data[1], 4, 1);
    return val;
}

static uint32_t bcmpkt_lbhdr_i_size_get(uint32_t *data, uint32_t **addr)
{
    return 4;
}


const bcmpkt_lbhdr_fget_t bcm56998_a0_lbhdr_fget = {
    {
        bcmpkt_lbhdr_start_get,
        bcmpkt_lbhdr_header_type_get,
        bcmpkt_lbhdr_input_priority_get,
        bcmpkt_lbhdr_pkt_profile_get,
        bcmpkt_lbhdr_visibility_pkt_get,
        bcmpkt_lbhdr_source_get,
        bcmpkt_lbhdr_source_type_get,
        bcmpkt_lbhdr_trill_ac_cng_get,
        bcmpkt_lbhdr_trill_ac_tc_get,
        NULL,
        bcmpkt_lbhdr_trill_nw_cng_get,
        bcmpkt_lbhdr_trill_nw_tc_get,
        bcmpkt_lbhdr_eth_dest_type_get,
        bcmpkt_lbhdr_eth_dest_get,
        bcmpkt_lbhdr_eth_dp_get,
        bcmpkt_lbhdr_eth_ecmp_member_id_get,
        bcmpkt_lbhdr_eth_int_cn_get,
        bcmpkt_lbhdr_eth_int_pri_get,
        bcmpkt_lbhdr_eth_mcast_lb_index_get,
        bcmpkt_lbhdr_eth_mcast_lb_index_vld_get,
        bcmpkt_lbhdr_eth_pp_port_get,
        bcmpkt_lbhdr_eth_qos_fields_vld_get,
        bcmpkt_lbhdr_eth_routed_pkt_get,
        bcmpkt_lbhdr_eth_subflow_type_get,
        bcmpkt_lbhdr_ethernet_vrf_get,
        bcmpkt_lbhdr_ethernet_vrf_valid_get,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    }
};


const bcmpkt_lbhdr_fset_t bcm56998_a0_lbhdr_fset = {
    {
        bcmpkt_lbhdr_start_set,
        bcmpkt_lbhdr_header_type_set,
        bcmpkt_lbhdr_input_priority_set,
        bcmpkt_lbhdr_pkt_profile_set,
        bcmpkt_lbhdr_visibility_pkt_set,
        bcmpkt_lbhdr_source_set,
        bcmpkt_lbhdr_source_type_set,
        bcmpkt_lbhdr_trill_ac_cng_set,
        bcmpkt_lbhdr_trill_ac_tc_set,
        NULL,
        bcmpkt_lbhdr_trill_nw_cng_set,
        bcmpkt_lbhdr_trill_nw_tc_set,
        bcmpkt_lbhdr_eth_dest_type_set,
        bcmpkt_lbhdr_eth_dest_set,
        bcmpkt_lbhdr_eth_dp_set,
        bcmpkt_lbhdr_eth_ecmp_member_id_set,
        bcmpkt_lbhdr_eth_int_cn_set,
        bcmpkt_lbhdr_eth_int_pri_set,
        bcmpkt_lbhdr_eth_mcast_lb_index_set,
        bcmpkt_lbhdr_eth_mcast_lb_index_vld_set,
        bcmpkt_lbhdr_eth_pp_port_set,
        bcmpkt_lbhdr_eth_qos_fields_vld_set,
        bcmpkt_lbhdr_eth_routed_pkt_set,
        bcmpkt_lbhdr_eth_subflow_type_set,
        bcmpkt_lbhdr_ethernet_vrf_set,
        bcmpkt_lbhdr_ethernet_vrf_valid_set,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    }
};


const bcmpkt_lbhdr_figet_t bcm56998_a0_lbhdr_figet = {
    {
        bcmpkt_lbhdr_i_size_get
    }
};

static shr_enum_map_t bcm56998_a0_lbhdr_view_types[] = {
    {"trill_nw", 1},
    {"trill_ac", 2},
    {"ethernet", 3},
    {NULL, -1},
};

/* -2: unsupported, -1: global, others: view's value */
static int bcm56998_a0_lbhdr_view_infos[BCMPKT_LBHDR_FID_COUNT] = {
     -1, -1, -1, -1, -1, -1, -1,  2,  2, -2,  1,  1,  3,  3,  3,  3,
      3,  3,  3,  3,  3,  3,  3,  3,  3,  3, -2, -2, -2, -2, -2, -2,
     -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
     -2,
};


void bcm56998_a0_lbhdr_view_info_get(bcmpkt_pmd_view_info_t *info)
{
    info->view_infos = bcm56998_a0_lbhdr_view_infos;
    info->view_types = bcm56998_a0_lbhdr_view_types;
    info->view_type_get = bcmpkt_lbhdr_header_type_get;
}
