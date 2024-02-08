#ifndef BCMPKT_HIGIG_DEFS_H
#define BCMPKT_HIGIG_DEFS_H
/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 * Tool: INTERNAL/regs/xgs/generate-chip.pl
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
 * This file provides access macros for the HiGig module header.
 *
 ******************************************************************************/

#include <bcmdrd/bcmdrd_symbols.h>

/*******************************************************************************
 *
 *                    HIGIG DEFINITIONS BEGIN HERE
 *
 ******************************************************************************/


/* Start of HiGig packet indicators */
#define BCMPKT_HIGIG_SOF                0xfb
#define BCMPKT_HIGIG2_SOF               0xfc


/* HiGig module header size (in bytes) */
#define BCMPKT_HIGIG_SIZE               12
#define BCMPKT_HIGIG2_SIZE              16


/* HiGig module header size (in words) */
#define BCMPKT_HIGIG_WSIZE              3
#define BCMPKT_HIGIG2_WSIZE             4


/*******************************************************************************
 * SWFORMAT:  HIGIG
 * BLOCKS:
 * SIZE:     96
 ******************************************************************************/
#define HIGIG_OFFSET 0x00000000

#define HIGIG_BLKACC ()

#define HIGIG_SIZE 12

/*
 * This structure should be used to declare and program HIGIG.
 *
 */
typedef union HIGIG_s {
    uint32_t v[3];
    uint32_t higig[3];
    uint32_t _higig;
} HIGIG_t;

#define HIGIG_CLR(r) sal_memset(&((r).higig[0]), 0, sizeof(HIGIG_t))
#define HIGIG_SET(r,i,d) (r).higig[i] = d
#define HIGIG_GET(r,i) (r).higig[i]

/*
 * These macros can be used to access individual fields.
 *
 */
#define HIGIG_VC_LABELf_GET(r) (((r).higig[2]) & 0xfffff)
#define HIGIG_VC_LABELf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0xfffff)) | (((uint32_t)f) & 0xfffff))
#define HIGIG_CLASSIFICATION_TAGf_GET(r) ((((r).higig[2]) >> 16) & 0xffff)
#define HIGIG_CLASSIFICATION_TAGf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0xffff << 16)) | ((((uint32_t)f) & 0xffff) << 16))
#define HIGIG_LABEL_PRESENTf_GET(r) ((((r).higig[2]) >> 20) & 0x1)
#define HIGIG_LABEL_PRESENTf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 20)) | ((((uint32_t)f) & 0x1) << 20))
#define HIGIG_L3f_GET(r) ((((r).higig[2]) >> 21) & 0x1)
#define HIGIG_L3f_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 21)) | ((((uint32_t)f) & 0x1) << 21))
#define HIGIG_DST_MODID_5f_GET(r) ((((r).higig[2]) >> 22) & 0x1)
#define HIGIG_DST_MODID_5f_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 22)) | ((((uint32_t)f) & 0x1) << 22))
#define HIGIG_SRC_MODID_5f_GET(r) ((((r).higig[2]) >> 23) & 0x1)
#define HIGIG_SRC_MODID_5f_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 23)) | ((((uint32_t)f) & 0x1) << 23))
#define HIGIG_MIRRORf_GET(r) ((((r).higig[2]) >> 24) & 0x1)
#define HIGIG_MIRRORf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 24)) | ((((uint32_t)f) & 0x1) << 24))
#define HIGIG_MIRROR_DONEf_GET(r) ((((r).higig[2]) >> 25) & 0x1)
#define HIGIG_MIRROR_DONEf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 25)) | ((((uint32_t)f) & 0x1) << 25))
#define HIGIG_MIRROR_ONLYf_GET(r) ((((r).higig[2]) >> 26) & 0x1)
#define HIGIG_MIRROR_ONLYf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 26)) | ((((uint32_t)f) & 0x1) << 26))
#define HIGIG_INGRESS_TAGGEDf_GET(r) ((((r).higig[2]) >> 27) & 0x1)
#define HIGIG_INGRESS_TAGGEDf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 27)) | ((((uint32_t)f) & 0x1) << 27))
#define HIGIG_DST_TGIDf_GET(r) ((((r).higig[2]) >> 28) & 0x7)
#define HIGIG_DST_TGIDf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x7 << 28)) | ((((uint32_t)f) & 0x7) << 28))
#define HIGIG_DST_Tf_GET(r) ((((r).higig[2]) >> 31) & 0x1)
#define HIGIG_DST_Tf_SET(r,f) (r).higig[2]=(((r).higig[2] & ~((uint32_t)0x1 << 31)) | ((((uint32_t)f) & 0x1) << 31))
#define HIGIG_DST_MODID_LSf_GET(r) (((r).higig[1]) & 0x1f)
#define HIGIG_DST_MODID_LSf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x1f)) | (((uint32_t)f) & 0x1f))
#define HIGIG_CNGf_GET(r) ((((r).higig[1]) >> 5) & 0x1)
#define HIGIG_CNGf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x1 << 5)) | ((((uint32_t)f) & 0x1) << 5))
#define HIGIG_HEADER_TYPEf_GET(r) ((((r).higig[1]) >> 6) & 0x3)
#define HIGIG_HEADER_TYPEf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x3 << 6)) | ((((uint32_t)f) & 0x3) << 6))
#define HIGIG_PRIORITYf_GET(r) ((((r).higig[1]) >> 8) & 0x7)
#define HIGIG_PRIORITYf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x7 << 8)) | ((((uint32_t)f) & 0x7) << 8))
#define HIGIG_DST_PORTf_GET(r) ((((r).higig[1]) >> 11) & 0x1f)
#define HIGIG_DST_PORTf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x1f << 11)) | ((((uint32_t)f) & 0x1f) << 11))
#define HIGIG_SRC_PORT_TGIDf_GET(r) ((((r).higig[1]) >> 16) & 0x3f)
#define HIGIG_SRC_PORT_TGIDf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x3f << 16)) | ((((uint32_t)f) & 0x3f) << 16))
#define HIGIG_PFMf_GET(r) ((((r).higig[1]) >> 22) & 0x3)
#define HIGIG_PFMf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x3 << 22)) | ((((uint32_t)f) & 0x3) << 22))
#define HIGIG_OPCODEf_GET(r) ((((r).higig[1]) >> 24) & 0x7)
#define HIGIG_OPCODEf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x7 << 24)) | ((((uint32_t)f) & 0x7) << 24))
#define HIGIG_SRC_MODID_LSf_GET(r) ((((r).higig[1]) >> 27) & 0x1f)
#define HIGIG_SRC_MODID_LSf_SET(r,f) (r).higig[1]=(((r).higig[1] & ~((uint32_t)0x1f << 27)) | ((((uint32_t)f) & 0x1f) << 27))
#define HIGIG_VTAGf_GET(r) (((r).higig[0]) & 0xffff)
#define HIGIG_VTAGf_SET(r,f) (r).higig[0]=(((r).higig[0] & ~((uint32_t)0xffff)) | (((uint32_t)f) & 0xffff))
#define HIGIG_DST_MODID_6f_GET(r) ((((r).higig[0]) >> 16) & 0x1)
#define HIGIG_DST_MODID_6f_SET(r,f) (r).higig[0]=(((r).higig[0] & ~((uint32_t)0x1 << 16)) | ((((uint32_t)f) & 0x1) << 16))
#define HIGIG_SRC_MODID_6f_GET(r) ((((r).higig[0]) >> 17) & 0x1)
#define HIGIG_SRC_MODID_6f_SET(r,f) (r).higig[0]=(((r).higig[0] & ~((uint32_t)0x1 << 17)) | ((((uint32_t)f) & 0x1) << 17))
#define HIGIG_HDR_EXT_LENf_GET(r) ((((r).higig[0]) >> 18) & 0x7)
#define HIGIG_HDR_EXT_LENf_SET(r,f) (r).higig[0]=(((r).higig[0] & ~((uint32_t)0x7 << 18)) | ((((uint32_t)f) & 0x7) << 18))
#define HIGIG_CNG1f_GET(r) ((((r).higig[0]) >> 21) & 0x1)
#define HIGIG_CNG1f_SET(r,f) (r).higig[0]=(((r).higig[0] & ~((uint32_t)0x1 << 21)) | ((((uint32_t)f) & 0x1) << 21))
#define HIGIG_HGIf_GET(r) ((((r).higig[0]) >> 22) & 0x3)
#define HIGIG_HGIf_SET(r,f) (r).higig[0]=(((r).higig[0] & ~((uint32_t)0x3 << 22)) | ((((uint32_t)f) & 0x3) << 22))
#define HIGIG_STARTf_GET(r) ((((r).higig[0]) >> 24) & 0xff)
#define HIGIG_STARTf_SET(r,f) (r).higig[0]=(((r).higig[0] & ~((uint32_t)0xff << 24)) | ((((uint32_t)f) & 0xff) << 24))

/*******************************************************************************
 * End of 'HIGIG'
 ******************************************************************************/




/*******************************************************************************
 * SWFORMAT:  HIGIG2
 * BLOCKS:
 * SIZE:     128
 ******************************************************************************/
#define HIGIG2_OFFSET 0x00000000

#define HIGIG2_BLKACC ()

#define HIGIG2_SIZE 16

/*
 * This structure should be used to declare and program HIGIG2.
 *
 */
typedef union HIGIG2_s {
    uint32_t v[4];
    uint32_t higig2[4];
    uint32_t _higig2;
} HIGIG2_t;

#define HIGIG2_CLR(r) sal_memset(&((r).higig2[0]), 0, sizeof(HIGIG2_t))
#define HIGIG2_SET(r,i,d) (r).higig2[i] = d
#define HIGIG2_GET(r,i) (r).higig2[i]

/*
 * These macros can be used to access individual fields.
 *
 */
#define HIGIG2_PPD_DATAf_GET(r,a) bcmdrd_field_be_get((r).higig2,4,0,63,a)
#define HIGIG2_PPD_DATAf_SET(r,a) bcmdrd_field_be_set((r).higig2,4,0,63,a)
#define HIGIG2_PPD0_RSVD_4_0f_GET(r) (((r).higig2[3]) & 0x1f)
#define HIGIG2_PPD0_RSVD_4_0f_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1f)) | (((uint32_t)f) & 0x1f))
#define HIGIG2_PPD0_HDR_EXT_LENf_GET(r) ((((r).higig2[3]) >> 5) & 0x7)
#define HIGIG2_PPD0_HDR_EXT_LENf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x7 << 5)) | ((((uint32_t)f) & 0x7) << 5))
#define HIGIG2_PPD0_OPCODEf_GET(r) ((((r).higig2[3]) >> 8) & 0x7)
#define HIGIG2_PPD0_OPCODEf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x7 << 8)) | ((((uint32_t)f) & 0x7) << 8))
#define HIGIG2_PPD0_PRESERVE_DOT1Pf_GET(r) ((((r).higig2[3]) >> 11) & 0x1)
#define HIGIG2_PPD0_PRESERVE_DOT1Pf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 11)) | ((((uint32_t)f) & 0x1) << 11))
#define HIGIG2_PPD0_PRESERVE_DSCPf_GET(r) ((((r).higig2[3]) >> 12) & 0x1)
#define HIGIG2_PPD0_PRESERVE_DSCPf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 12)) | ((((uint32_t)f) & 0x1) << 12))
#define HIGIG2_PPD0_SRC_Tf_GET(r) ((((r).higig2[3]) >> 13) & 0x1)
#define HIGIG2_PPD0_SRC_Tf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 13)) | ((((uint32_t)f) & 0x1) << 13))
#define HIGIG2_PPD0_PFMf_GET(r) ((((r).higig2[3]) >> 14) & 0x3)
#define HIGIG2_PPD0_PFMf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x3 << 14)) | ((((uint32_t)f) & 0x3) << 14))
#define HIGIG2_PPD0_VID_LOWf_GET(r) ((((r).higig2[3]) >> 16) & 0xff)
#define HIGIG2_PPD0_VID_LOWf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0xff << 16)) | ((((uint32_t)f) & 0xff) << 16))
#define HIGIG2_PPD0_VID_HIGHf_GET(r) ((((r).higig2[3]) >> 24) & 0xff)
#define HIGIG2_PPD0_VID_HIGHf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0xff << 24)) | ((((uint32_t)f) & 0xff) << 24))
#define HIGIG2_PPD0_REP_IDf_GET(r) (((r).higig2[2]) & 0x7ffff)
#define HIGIG2_PPD0_REP_IDf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x7ffff)) | (((uint32_t)f) & 0x7ffff))
#define HIGIG2_PPD0_VC_LABELf_GET(r) (((r).higig2[2]) & 0xffff)
#define HIGIG2_PPD0_VC_LABELf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xffff)) | (((uint32_t)f) & 0xffff))
#define HIGIG2_PPD0_MIRROR_CLASSIFICATION_TAGf_GET(r) ((((r).higig2[2]) >> 4) & 0xffff)
#define HIGIG2_PPD0_MIRROR_CLASSIFICATION_TAGf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xffff << 4)) | ((((uint32_t)f) & 0xffff) << 4))
#define HIGIG2_PPD0_ORIG_SRC_PORTf_GET(r) ((((r).higig2[2]) >> 4) & 0xff)
#define HIGIG2_PPD0_ORIG_SRC_PORTf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xff << 4)) | ((((uint32_t)f) & 0xff) << 4))
#define HIGIG2_PPD0_ORIG_SRC_MODIDf_GET(r) ((((r).higig2[2]) >> 12) & 0xff)
#define HIGIG2_PPD0_ORIG_SRC_MODIDf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xff << 12)) | ((((uint32_t)f) & 0xff) << 12))
#define HIGIG2_PPD0_VC_LABEL_19_16f_GET(r) ((((r).higig2[2]) >> 16) & 0xf)
#define HIGIG2_PPD0_VC_LABEL_19_16f_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xf << 16)) | ((((uint32_t)f) & 0xf) << 16))
#define HIGIG2_PPD0_LABEL_PRESENTf_GET(r) ((((r).higig2[2]) >> 20) & 0x1)
#define HIGIG2_PPD0_LABEL_PRESENTf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1 << 20)) | ((((uint32_t)f) & 0x1) << 20))
#define HIGIG2_PPD0_L3f_GET(r) ((((r).higig2[2]) >> 21) & 0x1)
#define HIGIG2_PPD0_L3f_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1 << 21)) | ((((uint32_t)f) & 0x1) << 21))
#define HIGIG2_PPD0_LABEL_OVERLAY_TYPEf_GET(r) ((((r).higig2[2]) >> 22) & 0x3)
#define HIGIG2_PPD0_LABEL_OVERLAY_TYPEf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x3 << 22)) | ((((uint32_t)f) & 0x3) << 22))
#define HIGIG2_PPD0_MIRRORf_GET(r) ((((r).higig2[2]) >> 24) & 0x1)
#define HIGIG2_PPD0_MIRRORf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1 << 24)) | ((((uint32_t)f) & 0x1) << 24))
#define HIGIG2_PPD0_MIRROR_DONEf_GET(r) ((((r).higig2[2]) >> 25) & 0x1)
#define HIGIG2_PPD0_MIRROR_DONEf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1 << 25)) | ((((uint32_t)f) & 0x1) << 25))
#define HIGIG2_PPD0_MIRROR_ONLYf_GET(r) ((((r).higig2[2]) >> 26) & 0x1)
#define HIGIG2_PPD0_MIRROR_ONLYf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1 << 26)) | ((((uint32_t)f) & 0x1) << 26))
#define HIGIG2_PPD0_INGRESS_TAGGEDf_GET(r) ((((r).higig2[2]) >> 27) & 0x1)
#define HIGIG2_PPD0_INGRESS_TAGGEDf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1 << 27)) | ((((uint32_t)f) & 0x1) << 27))
#define HIGIG2_PPD0_DST_TGIDf_GET(r) ((((r).higig2[2]) >> 28) & 0x7)
#define HIGIG2_PPD0_DST_TGIDf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x7 << 28)) | ((((uint32_t)f) & 0x7) << 28))
#define HIGIG2_PPD0_DST_Tf_GET(r) ((((r).higig2[2]) >> 31) & 0x1)
#define HIGIG2_PPD0_DST_Tf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1 << 31)) | ((((uint32_t)f) & 0x1) << 31))
#define HIGIG2_PPD1_RSVD_4_0f_GET(r) (((r).higig2[3]) & 0x1f)
#define HIGIG2_PPD1_RSVD_4_0f_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1f)) | (((uint32_t)f) & 0x1f))
#define HIGIG2_PPD1_HDR_EXT_LENf_GET(r) ((((r).higig2[3]) >> 5) & 0x7)
#define HIGIG2_PPD1_HDR_EXT_LENf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x7 << 5)) | ((((uint32_t)f) & 0x7) << 5))
#define HIGIG2_PPD1_OPCODEf_GET(r) ((((r).higig2[3]) >> 8) & 0x7)
#define HIGIG2_PPD1_OPCODEf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x7 << 8)) | ((((uint32_t)f) & 0x7) << 8))
#define HIGIG2_PPD1_RSVD_12_11f_GET(r) ((((r).higig2[3]) >> 11) & 0x3)
#define HIGIG2_PPD1_RSVD_12_11f_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x3 << 11)) | ((((uint32_t)f) & 0x3) << 11))
#define HIGIG2_PPD1_SRC_Tf_GET(r) ((((r).higig2[3]) >> 13) & 0x1)
#define HIGIG2_PPD1_SRC_Tf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 13)) | ((((uint32_t)f) & 0x1) << 13))
#define HIGIG2_PPD1_PFMf_GET(r) ((((r).higig2[3]) >> 14) & 0x3)
#define HIGIG2_PPD1_PFMf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x3 << 14)) | ((((uint32_t)f) & 0x3) << 14))
#define HIGIG2_PPD1_VID_LOWf_GET(r) ((((r).higig2[3]) >> 16) & 0xff)
#define HIGIG2_PPD1_VID_LOWf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0xff << 16)) | ((((uint32_t)f) & 0xff) << 16))
#define HIGIG2_PPD1_VID_HIGHf_GET(r) ((((r).higig2[3]) >> 24) & 0xff)
#define HIGIG2_PPD1_VID_HIGHf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0xff << 24)) | ((((uint32_t)f) & 0xff) << 24))
#define HIGIG2_PPD1_RSVD_48_32f_GET(r) (((r).higig2[2]) & 0xffff)
#define HIGIG2_PPD1_RSVD_48_32f_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xffff)) | (((uint32_t)f) & 0xffff))
#define HIGIG2_PPD1_CLASSIFICATION_TAGf_GET(r) ((((r).higig2[2]) >> 16) & 0xffff)
#define HIGIG2_PPD1_CLASSIFICATION_TAGf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xffff << 16)) | ((((uint32_t)f) & 0xffff) << 16))
#define HIGIG2_PPD2_SOURCE_TYPEf_GET(r) (((r).higig2[3]) & 0x1)
#define HIGIG2_PPD2_SOURCE_TYPEf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1)) | (((uint32_t)f) & 0x1))
#define HIGIG2_PPD2_DEST_TYPEf_GET(r) ((((r).higig2[3]) >> 1) & 0x1)
#define HIGIG2_PPD2_DEST_TYPEf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 1)) | ((((uint32_t)f) & 0x1) << 1))
#define HIGIG2_PPD2_PRESERVE_DOT1Pf_GET(r) ((((r).higig2[3]) >> 2) & 0x1)
#define HIGIG2_PPD2_PRESERVE_DOT1Pf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 2)) | ((((uint32_t)f) & 0x1) << 2))
#define HIGIG2_PPD2_PRESERVE_DSCPf_GET(r) ((((r).higig2[3]) >> 3) & 0x1)
#define HIGIG2_PPD2_PRESERVE_DSCPf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 3)) | ((((uint32_t)f) & 0x1) << 3))
#define HIGIG2_PPD2_VNI_HIGHf_GET(r) ((((r).higig2[3]) >> 4) & 0x3)
#define HIGIG2_PPD2_VNI_HIGHf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x3 << 4)) | ((((uint32_t)f) & 0x3) << 4))
#define HIGIG2_PPD2_RSVD_7_6f_GET(r) ((((r).higig2[3]) >> 6) & 0x3)
#define HIGIG2_PPD2_RSVD_7_6f_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x3 << 6)) | ((((uint32_t)f) & 0x3) << 6))
#define HIGIG2_PPD2_OPCODEf_GET(r) ((((r).higig2[3]) >> 8) & 0x7)
#define HIGIG2_PPD2_OPCODEf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x7 << 8)) | ((((uint32_t)f) & 0x7) << 8))
#define HIGIG2_PPD2_REP_ID_17_16f_GET(r) ((((r).higig2[3]) >> 8) & 0x3)
#define HIGIG2_PPD2_REP_ID_17_16f_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x3 << 8)) | ((((uint32_t)f) & 0x3) << 8))
#define HIGIG2_PPD2_RSVD_11f_GET(r) ((((r).higig2[3]) >> 11) & 0x1)
#define HIGIG2_PPD2_RSVD_11f_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 11)) | ((((uint32_t)f) & 0x1) << 11))
#define HIGIG2_PPD2_LAG_FAILOVERf_GET(r) ((((r).higig2[3]) >> 12) & 0x1)
#define HIGIG2_PPD2_LAG_FAILOVERf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 12)) | ((((uint32_t)f) & 0x1) << 12))
#define HIGIG2_PPD2_DO_NOT_LEARNf_GET(r) ((((r).higig2[3]) >> 13) & 0x1)
#define HIGIG2_PPD2_DO_NOT_LEARNf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 13)) | ((((uint32_t)f) & 0x1) << 13))
#define HIGIG2_PPD2_DO_NOT_MODIFYf_GET(r) ((((r).higig2[3]) >> 14) & 0x1)
#define HIGIG2_PPD2_DO_NOT_MODIFYf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 14)) | ((((uint32_t)f) & 0x1) << 14))
#define HIGIG2_PPD2_MIRRORf_GET(r) ((((r).higig2[3]) >> 15) & 0x1)
#define HIGIG2_PPD2_MIRRORf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 15)) | ((((uint32_t)f) & 0x1) << 15))
#define HIGIG2_PPD2_SOURCE_VPf_GET(r) ((((r).higig2[3]) >> 16) & 0xffff)
#define HIGIG2_PPD2_SOURCE_VPf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0xffff << 16)) | ((((uint32_t)f) & 0xffff) << 16))
#define HIGIG2_PPD2_DEST_VPf_GET(r) (((r).higig2[2]) & 0xffff)
#define HIGIG2_PPD2_DEST_VPf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xffff)) | (((uint32_t)f) & 0xffff))
#define HIGIG2_PPD2_REP_ID_15_0f_GET(r) (((r).higig2[2]) & 0xffff)
#define HIGIG2_PPD2_REP_ID_15_0f_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xffff)) | (((uint32_t)f) & 0xffff))
#define HIGIG2_PPD2_VNI_LOWf_GET(r) ((((r).higig2[2]) >> 16) & 0xff)
#define HIGIG2_PPD2_VNI_LOWf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0xff << 16)) | ((((uint32_t)f) & 0xff) << 16))
#define HIGIG2_PPD2_VNI_MIDf_GET(r) ((((r).higig2[2]) >> 24) & 0x3)
#define HIGIG2_PPD2_VNI_MIDf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x3 << 24)) | ((((uint32_t)f) & 0x3) << 24))
#define HIGIG2_PPD2_FWD_TYPEf_GET(r) ((((r).higig2[2]) >> 26) & 0x1f)
#define HIGIG2_PPD2_FWD_TYPEf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1f << 26)) | ((((uint32_t)f) & 0x1f) << 26))
#define HIGIG2_PPD2_MULTIPOINTf_GET(r) ((((r).higig2[2]) >> 31) & 0x1)
#define HIGIG2_PPD2_MULTIPOINTf_SET(r,f) (r).higig2[2]=(((r).higig2[2] & ~((uint32_t)0x1 << 31)) | ((((uint32_t)f) & 0x1) << 31))
#define HIGIG2_PPD3_PPD3_CONTAINER_TYPEf_GET(r) (((r).higig2[3]) & 0xf)
#define HIGIG2_PPD3_PPD3_CONTAINER_TYPEf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0xf)) | (((uint32_t)f) & 0xf))
#define HIGIG2_PPD3_RSVD_7_4f_GET(r) ((((r).higig2[3]) >> 4) & 0xf)
#define HIGIG2_PPD3_RSVD_7_4f_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0xf << 4)) | ((((uint32_t)f) & 0xf) << 4))
#define HIGIG2_PPD3_OPCODEf_GET(r) ((((r).higig2[3]) >> 8) & 0x7)
#define HIGIG2_PPD3_OPCODEf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x7 << 8)) | ((((uint32_t)f) & 0x7) << 8))
#define HIGIG2_PPD3_SOURCE_TYPEf_GET(r) ((((r).higig2[3]) >> 11) & 0x1)
#define HIGIG2_PPD3_SOURCE_TYPEf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 11)) | ((((uint32_t)f) & 0x1) << 11))
#define HIGIG2_PPD3_RSVD_28f_GET(r) ((((r).higig2[3]) >> 12) & 0x1)
#define HIGIG2_PPD3_RSVD_28f_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 12)) | ((((uint32_t)f) & 0x1) << 12))
#define HIGIG2_PPD3_DO_NOT_LEARNf_GET(r) ((((r).higig2[3]) >> 13) & 0x1)
#define HIGIG2_PPD3_DO_NOT_LEARNf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 13)) | ((((uint32_t)f) & 0x1) << 13))
#define HIGIG2_PPD3_PRESERVE_DOT1Pf_GET(r) ((((r).higig2[3]) >> 14) & 0x1)
#define HIGIG2_PPD3_PRESERVE_DOT1Pf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 14)) | ((((uint32_t)f) & 0x1) << 14))
#define HIGIG2_PPD3_PRESERVE_DSCPf_GET(r) ((((r).higig2[3]) >> 15) & 0x1)
#define HIGIG2_PPD3_PRESERVE_DSCPf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0x1 << 15)) | ((((uint32_t)f) & 0x1) << 15))
#define HIGIG2_PPD3_SOURCE_INFOf_GET(r) ((((r).higig2[3]) >> 16) & 0xffff)
#define HIGIG2_PPD3_SOURCE_INFOf_SET(r,f) (r).higig2[3]=(((r).higig2[3] & ~((uint32_t)0xffff << 16)) | ((((uint32_t)f) & 0xffff) << 16))
#define HIGIG2_PPD3_PPD3_DATA_CONTAINERf_GET(r) ((r).higig2[2])
#define HIGIG2_PPD3_PPD3_DATA_CONTAINERf_SET(r,f) (r).higig2[2]=((uint32_t)f)
#define HIGIG2_PPD_TYPEf_GET(r) (((r).higig2[1]) & 0x7)
#define HIGIG2_PPD_TYPEf_SET(r,f) (r).higig2[1]=(((r).higig2[1] & ~((uint32_t)0x7)) | (((uint32_t)f) & 0x7))
#define HIGIG2_CCf_GET(r) ((((r).higig2[1]) >> 3) & 0x3)
#define HIGIG2_CCf_SET(r,f) (r).higig2[1]=(((r).higig2[1] & ~((uint32_t)0x3 << 3)) | ((((uint32_t)f) & 0x3) << 3))
#define HIGIG2_EHVf_GET(r) ((((r).higig2[1]) >> 5) & 0x1)
#define HIGIG2_EHVf_SET(r,f) (r).higig2[1]=(((r).higig2[1] & ~((uint32_t)0x1 << 5)) | ((((uint32_t)f) & 0x1) << 5))
#define HIGIG2_DPf_GET(r) ((((r).higig2[1]) >> 6) & 0x3)
#define HIGIG2_DPf_SET(r,f) (r).higig2[1]=(((r).higig2[1] & ~((uint32_t)0x3 << 6)) | ((((uint32_t)f) & 0x3) << 6))
#define HIGIG2_LBIDf_GET(r) ((((r).higig2[1]) >> 8) & 0xff)
#define HIGIG2_LBIDf_SET(r,f) (r).higig2[1]=(((r).higig2[1] & ~((uint32_t)0xff << 8)) | ((((uint32_t)f) & 0xff) << 8))
#define HIGIG2_SRC_PIDf_GET(r) ((((r).higig2[1]) >> 16) & 0xff)
#define HIGIG2_SRC_PIDf_SET(r,f) (r).higig2[1]=(((r).higig2[1] & ~((uint32_t)0xff << 16)) | ((((uint32_t)f) & 0xff) << 16))
#define HIGIG2_SRC_MODIDf_GET(r) ((((r).higig2[1]) >> 24) & 0xff)
#define HIGIG2_SRC_MODIDf_SET(r,f) (r).higig2[1]=(((r).higig2[1] & ~((uint32_t)0xff << 24)) | ((((uint32_t)f) & 0xff) << 24))
#define HIGIG2_DST_PORT_MGIDLf_GET(r) (((r).higig2[0]) & 0xff)
#define HIGIG2_DST_PORT_MGIDLf_SET(r,f) (r).higig2[0]=(((r).higig2[0] & ~((uint32_t)0xff)) | (((uint32_t)f) & 0xff))
#define HIGIG2_DST_MODID_MGIDHf_GET(r) ((((r).higig2[0]) >> 8) & 0xff)
#define HIGIG2_DST_MODID_MGIDHf_SET(r,f) (r).higig2[0]=(((r).higig2[0] & ~((uint32_t)0xff << 8)) | ((((uint32_t)f) & 0xff) << 8))
#define HIGIG2_TCf_GET(r) ((((r).higig2[0]) >> 16) & 0xf)
#define HIGIG2_TCf_SET(r,f) (r).higig2[0]=(((r).higig2[0] & ~((uint32_t)0xf << 16)) | ((((uint32_t)f) & 0xf) << 16))
#define HIGIG2_MCSTf_GET(r) ((((r).higig2[0]) >> 20) & 0x1)
#define HIGIG2_MCSTf_SET(r,f) (r).higig2[0]=(((r).higig2[0] & ~((uint32_t)0x1 << 20)) | ((((uint32_t)f) & 0x1) << 20))
#define HIGIG2_STARTf_GET(r) ((((r).higig2[0]) >> 24) & 0xff)
#define HIGIG2_STARTf_SET(r,f) (r).higig2[0]=(((r).higig2[0] & ~((uint32_t)0xff << 24)) | ((((uint32_t)f) & 0xff) << 24))

/*******************************************************************************
 * End of 'HIGIG2'
 ******************************************************************************/




/*******************************************************************************
 *
 *                         HIGIG SYMBOL TABLE
 *
 ******************************************************************************/

extern bcmdrd_symbols_t higig_symbols;


#endif /* BCMPKT_HIGIG_DEFS_H */
