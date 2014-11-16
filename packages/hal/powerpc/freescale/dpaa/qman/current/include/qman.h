#ifndef CYGONCE_HAL_POWERPC_FREESCALE_DPAA_QMAN_H
#define CYGONCE_HAL_POWERPC_FREESCALE_DPAA_QMAN_H
//==========================================================================
//
//      qman.h
//
//      HAL support for Freescale DPAA Queue Manager
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2013 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later
// version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with eCos; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// As a special exception, if other files instantiate templates or use
// macros or inline functions from this file, or you compile this file
// and link it with other works to produce a work based on this file,
// this file does not by itself cause the resulting work to be covered by
// the GNU General Public License. However the source code for this file
// must still be made available in accordance with section (3) of the GNU
// General Public License v2.
//
// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.
// -------------------------------------------
// ####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    ccoutand
// Contributors:
// Date:         2013-01-20
// Purpose:
// Description:  Freescale DPAA Queue Manager
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#define QMAN_MAX_SWP                    10     // Software portals
#define QMAN_MAX_DCP                    5      // Direct Connect Portal
#define QMAN_MAX_WQ_CLASS_SCHED         7      // Work Queue Class Scheduler
#define QMAN_MAX_WQ_SC_DD               5      // WQ S/W Channel Dynamic Debug
#define QMAN_MAX_WQ_PC_DD               8      // WQ Pool Channel Dynamic Debug
#define QMAN_MAX_WQ_CDCP_DD             6

#if defined(CYGHWR_HAL_POWERPC_QORIQ_P1023)
# define QMAN_NUM_SWP                   3
# define QMAN_NUM_DCP                   1
# define QMAN_NUM_WQ_CLASS_SCHED        3
# define QMAN_NUM_WQ_SC_DD              2
# define QMAN_NUM_WQ_PC_DD              2
# define QMAN_NUM_WQ_CDCP_DD            4
# define CYGARC_REG_IMM_DPAA_BMAN       0x088000
#else
# define QMAN_NUM_WQ_SC_DD              5
# define QMAN_NUM_WQ_PC_DD              8
# define QMAN_NUM_WQ_CDCP_DD            6
# define QMAN_NUM_SWP                   10
# define CYGARC_REG_IMM_DPAA_BMAN       0x318000
#endif

#if defined(CYGHWR_HAL_POWERPC_QORIQ_P5020) ||   \
    defined(CYGHWR_HAL_POWERPC_QORIQ_P5040)
# define QMAN_NUM_DCP                   5
# define QMAN_NUM_WQ_CLASS_SCHED        7
#endif

#if defined(CYGHWR_HAL_POWERPC_QORIQ_P2040) ||   \
    defined(CYGHWR_HAL_POWERPC_QORIQ_P2041) ||   \
    defined(CYGHWR_HAL_POWERPC_QORIQ_P3040) ||   \
    defined(CYGHWR_HAL_POWERPC_QORIQ_P3041) ||   \
    defined(CYGHWR_HAL_POWERPC_QORIQ_P4040) ||   \
    defined(CYGHWR_HAL_POWERPC_QORIQ_P4080)
# define QMAN_NUM_DCP                   4
# define QMAN_NUM_WQ_CLASS_SCHED        6
#endif


// Software Portal Configuration Registers
struct qman_spc
{
  cyg_uint32 lio_cfg;
  cyg_uint32 io_cfg;
  cyg_uint32 _reserved0[0x01];
  cyg_uint32 dd_cfg;
};

// Direct Connect Portal (DCP) Configuration Registers
struct qman_dcp
{
  cyg_uint32 cfg;
  cyg_uint32 dd_cfg;
  cyg_uint32 dlm_cfg;
  cyg_uint32 dlm_avg;
};

// BMan Configuration and Control Register Memory Map
struct qman_cr_reg
{
  struct qman_spc spc[QMAN_MAX_SWP];
  cyg_uint32 _reserved0[0x58];
  cyg_uint32 qm_dd_cfg;
  cyg_uint32 _reserved1[0x03];
  cyg_uint32 qcsp_dd_ihrsr;
  cyg_uint32 qcsp_dd_ihrfr;
  cyg_uint32 qcsp_dd_hasr;
  cyg_uint32 _reserved2;
  cyg_uint32 dcp_dd_ihrsr;
  cyg_uint32 dcp_dd_ihrfr;
  cyg_uint32 dcp_dd_hasr;
  cyg_uint32 _reserved3[0x35];
  struct qman_dcp dcp[QMAN_MAX_DCP];
  cyg_uint32 _reserved4[0x2C];
  cyg_uint32 pfdr_fpc;
  cyg_uint32 pfdr_fp_head;
  cyg_uint32 pfdr_fp_tail;
  cyg_uint32 _reserved5;
  cyg_uint32 pfdr_fp_lwit;
  cyg_uint32 pfdr_cfg;
  cyg_uint32 _reserved6[0x3A];
  cyg_uint32 sfdr_cfg;
  cyg_uint32 sfdr_in_use;
  cyg_uint32 _reserved7[0x3E];
  cyg_uint32 wq_cs_cfg[QMAN_MAX_WQ_CLASS_SCHED];
  cyg_uint32 _reserved8[0x05];
  cyg_uint32 wq_def_enq_wqid;
  cyg_uint32 _reserved9[0x03];
  cyg_uint32 wq_sc_dd_cfg[QMAN_MAX_WQ_SC_DD];
  cyg_uint32 _reserved10[0x0B];
  cyg_uint32 wq_pc_dd_cs_cfg[QMAN_MAX_WQ_PC_DD];
  cyg_uint32 _reserved11[0x08];
  cyg_uint32 wq_dc0_dd_cs_cfg[QMAN_MAX_WQ_CDCP_DD];
  cyg_uint32 _reserved12[0x0A];
  cyg_uint32 wq_dc1_dd_cs_cfg[QMAN_MAX_WQ_CDCP_DD];
  cyg_uint32 _reserved13[0x0A];
  cyg_uint32 wq_dc2_dd_cs_cfg;
  cyg_uint32 _reserved14[0x0F];
  cyg_uint32 wq_dc3_dd_cs_cfg;        // N/A for P1023
  cyg_uint32 _reserved15[0x0F];
  cyg_uint32 wq_dc4_dd_cs_cfg;        // N/A for P1023, P2041, P3041 and P4080
  cyg_uint32 _reserved16[0x0F];
  cyg_uint32 cm_cfg;
  cyg_uint32 _reserved17[0x7F];
  cyg_uint32 qm_ecsr;
  cyg_uint32 qm_ecir;
  cyg_uint32 qm_eadr;
  cyg_uint32 _reserved18;
  cyg_uint32 qm_edata[16];
  cyg_uint32 _reserved19[0x08];
  cyg_uint32 qm_sbet;
  cyg_uint32 _reserved20[0x03];
  cyg_uint32 qm_sbec[7];
  cyg_uint32 _reserved21[0x19];
  cyg_uint32 qm_mcr;
  cyg_uint32 qm_mcp0;
  cyg_uint32 qm_mcp1;
  cyg_uint32 _reserved22[0x05];
  cyg_uint32 qm_mr[16];
  cyg_uint32 _reserved23[0x20];
  cyg_uint32 qm_misc_cfg;              // N/A for P2041, P3041, P4080 and P5020
  cyg_uint32 _reserved24[0x04];
  cyg_uint32 qm_idle_stat;
  cyg_uint32 qm_ip_rev_1;
  cyg_uint32 qm_ip_rev_2;
  cyg_uint32 fqd_bare;
  cyg_uint32 fqd_bar;
  cyg_uint32 _reserved25[0x02];
  cyg_uint32 fqd_ar;
  cyg_uint32 _reserved26[0x03];
  cyg_uint32 pfdr_bare;
  cyg_uint32 pfdr_bar;
  cyg_uint32 _reserved27[0x02];
  cyg_uint32 pfdr_ar;
  cyg_uint32 _reserved28[0x13];
  cyg_uint32 qcsp_bare;
  cyg_uint32 qcsp_bar;
  cyg_uint32 _reserved29[0x1E];
  cyg_uint32 ci_sched_cfg;
  cyg_uint32 qm_srcidr;
  cyg_uint32 qm_liodnr;
  cyg_uint32 _reserved30;
  cyg_uint32 ci_rlm_cfg;
  cyg_uint32 ci_rlm_avg;
  cyg_uint32 _reserved31[0x3A];
  cyg_uint32 qm_err_isr;
  cyg_uint32 qm_err_ier;
  cyg_uint32 qm_err_isdr;
  cyg_uint32 qm_err_iir;
  cyg_uint32 _reserved32;
  cyg_uint32 qm_err_her;
};

//
// Software Portal Configuration Registers

// QCSPi_LIO_CFG
#define QMAN_SPC_LIO_DLIODN_MASK                 0x00000fff
#define QMAN_SPC_LIO_DLIODN(_x)                 ((_x & QMAN_SPC_LIO_DLIODN_MASK))
#define QMAN_SPC_LIO_ELIODN_OFFSET_MASK          0x03c00000
#define QMAN_SPC_LIO_LIODN_OFFSET_MASK           0x003f0000
#define QMAN_SPC_LIO_SET_ELIODN_OFFSET(_x)      ((_x << 22) & QMAN_SPC_LIO_ELIODN_OFFSET_MASK)
#define QMAN_SPC_LIO_SET_LIODN_OFFSET(_x)       ((_x << 16) &  QMAN_SPC_LIO_LIODN_OFFSET_MASK)
#define QMAN_SPC_LIO_GET_ELIODN_OFFSET(_x)      ((_x & QMAN_SPC_LIO_ELIODN_OFFSET_MASK) >> 22)
#define QMAN_SPC_LIO_GET_LIODN_OFFSET(_x)       ((_x & QMAN_SPC_LIO_LIODN_OFFSET_MASK)  >> 16)
// QCSPi_IO_CFG
#define QMAN_SPC_IO_FLIODN_MASK                  0x00000fff
#define QMAN_SPC_IO_SDEST_MASK                   0x00070000
#define QMAN_SPC_IO_FLIODN(_x)                  ((_x & QMAN_SPC_IO_FLIODN_MASK))
#define QMAN_SPC_IO_SET_SDEST(_x)               ((_x << 16) & QMAN_SPC_IO_SDEST_MASK)
#define QMAN_SPC_IO_GET_SDEST(_x)               ((_x & QMAN_SPC_IO_SDEST_MASK) >> 16)
// QCSPi_DD_CFG
#define QMAN_SPC_DD_E_TP_CFG_MASK                0x000001ff
#define QMAN_SPC_DD_ED_TP_CFG_MASK               0x01ff0000
#define QMAN_SPC_DD_E_TP_CFG(_x)                ((_x & QMAN_SPC_DD_E_TP_CFG_MASK))
#define QMAN_SPC_DD_SET_ED_TP_CFG(_x)           ((_x << 16) & QMAN_SPC_DD_ED_TP_CFG_MASK)
#define QMAN_SPC_DD_GET_ED_TP_CFG(_x)           ((_x & QMAN_SPC_DD_ED_TP_CFG_MASK) >> 16)


//
// Direct Connect Portal (DCP) Configuration Registers

// DCPi_CFG
#define QMAN_DCP_CFG_ED                          0x00000100
#define QMAN_DCP_CFG_SERND_MASK                  0x0000001f
#define QMAN_DCP_CFG_SERND(_x)                  ((_x & QMAN_DCP_CFG_SERND_MASK))

// DCPi_DLM_CFG
#define QMAN_DCP_SET_DLM_SM_MASK                 0x0000fff0
#define QMAN_DCP_SET_DLM_TH_D_MASK               0x000f0000
#define QMAN_DCP_SET_DLM_TH_C_MASK               0x00f00000
#define QMAN_DCP_SET_DLM_TH_B_MASK               0x0f000000
#define QMAN_DCP_SET_DLM_TH_A_MASK               0xf0000000
#define QMAN_DCP_SET_DLM_SM(_x)                 ((_x <<  4) &   QMAN_DCP_SET_DLM_SM_MASK)
#define QMAN_DCP_SET_DLM_TH_D(_x)               ((_x << 16) & QMAN_DCP_SET_DLM_TH_D_MASK)
#define QMAN_DCP_SET_DLM_TH_C(_x)               ((_x << 20) & QMAN_DCP_SET_DLM_TH_C_MASK)
#define QMAN_DCP_SET_DLM_TH_B(_x)               ((_x << 24) & QMAN_DCP_SET_DLM_TH_B_MASK)
#define QMAN_DCP_SET_DLM_TH_A(_x)               ((_x << 28) & QMAN_DCP_SET_DLM_TH_A_MASK)
#define QMAN_DCP_GET_DLM_SM(_x)                 ((_x & QMAN_DCP_SET_DLM_SM_MASK)   >>  4)
#define QMAN_DCP_GET_DLM_TH_D(_x)               ((_x & QMAN_DCP_SET_DLM_TH_D_MASK) >> 16)
#define QMAN_DCP_GET_DLM_TH_C(_x)               ((_x & QMAN_DCP_SET_DLM_TH_C_MASK) >> 20)
#define QMAN_DCP_GET_DLM_TH_B(_x)               ((_x & QMAN_DCP_SET_DLM_TH_B_MASK) >> 24)
#define QMAN_DCP_GET_DLM_TH_A(_x)               ((_x & QMAN_DCP_SET_DLM_TH_A_MASK) >> 28)
// DCPi_DLM_AVG
#define QMAN_DCP_DLM_AVG_FRACT_MASK              0x000000ff
#define QMAN_DCP_DLM_AVG_FRACT(_x)              ((_x & QMAN_DCP_DLM_AVG_FRACT_MASK))
#define QMAN_DCP_DLM_AVG_INT                     0x000fff00
#define QMAN_DCP_SET_DLM_AVG_INT(_x)            ((_x << 8) & QMAN_DCP_DLM_AVG_INT)
#define QMAN_DCP_GET_DLM_AVG_INT(_x)            ((_x & QMAN_DCP_DLM_AVG_INT) >> 8)

//
// The rest of QMAN internal register

// QMAN_DD_CFG
#define QMAN_DD_CFG_M_CFG_MASK                   0x0000000f
#define QMAN_DD_CFG_MDD_MASK                     0x00000030
#define QMAN_DD_CFG_M_CFG(_x)                   ((_x & QMAN_DD_CFG_M_CFG_MASK))
#define QMAN_DD_CFG_SET_MDD(_x)                 ((_x << 4) & QMAN_DD_CFG_MDD_MASK)
#define QMAN_DD_CFG_GET_MDD(_x)                 ((_x & QMAN_DD_CFG_MDD_MASK) >> 4)
#define QMAN_DD_CFG_F                            0x40000000
#define QMAN_DD_CFG_L                            0x80000000
// PFDR_FPC
#define QMAN_PFDR_FPC_MASK                       0x00ffffff
#define QMAN_PFDR_FPC(_x)                       ((_x & QMAN_PFDR_FPC_MASK))
// PFDR_FP_HEAD
#define QMAN_PFDR_FP_HEAD_MASK                   0x00ffffff
#define QMAN_PFDR_FP_HEAD(_x)                   ((_x & QMAN_PFDR_FP_HEAD_MASK))
// PFDR_FP_TAIL
#define QMAN_PFDR_FP_TAIL_MASK                   0x00ffffff
#define QMAN_PFDR_FP_TAIL(_x)                   ((_x & QMAN_PFDR_FP_TAIL_MASK))
// PFDR_FP_LWIT
#define QMAN_PFDR_FP_LWIT_TH_MASK                0x00ffffff
#define QMAN_PFDR_FP_LWIT_TH(_x)                ((_x & QMAN_PFDR_FP_LWIT_TH_MASK))
// PFDR_CFG
#define QMAN_PFDR_CFG_K_MASK                     0x000000ff
#define QMAN_PFDR_CFG_K(_x)                     ((_x & QMAN_PFDR_CFG_K_MASK))
// SFDR_CFG
#define QMAN_SFDR_CFG_MASK                       0x000003ff
#define QMAN_SFDR_CFG(_x)                       ((_x & QMAN_SFDR_CFG_MASK))
// SFDR_IN_USE
#define QMAN_SFDR_IN_USE_NUM_MASK                0x00000fff
#define QMAN_SFDR_IN_USE_NUM(_x)                ((_x & QMAN_SFDR_IN_USE_NUM_MASK))
// WQ_CS_CFGi
#define QMAN_WQ_CS_CFG_CS_ELEV_MASK              0xff000000
#define QMAN_WQ_CS_CFG_CSWx_MASK(_q)            (     (0x00000007) << ((_q-2) << 4))
#define QMAN_WQ_CS_CFG_SET_CSWx(_x, _q)         ((_x & 0x00000007) << ((_q-2) << 4))
#define QMAN_WQ_CS_CFG_GET_CSWx(_x, _q)         ((_x >> ((_q-2) << 4)) & 0x00000007)
#define QMAN_WQ_CS_CFG_SET_CS_ELEV(_x)          ((_x << 24) & QMAN_WQ_CS_CFG_CS_ELEV_MASK)
#define QMAN_WQ_CS_CFG_GET_CS_ELEV(_x)          ((_x & QMAN_WQ_CS_CFG_CS_ELEV_MASK) >> 24)
// WQ_DEF_ENQ_WQID
#define QMAN_WQ_DEF_ENQ_WQID_MASK                0x0000ffff
#define QMAN_WQ_DEF_ENQ_WQID(_x)                ((_x & QMAN_WQ_DEF_ENQ_WQID_MASK))
// CM_CFG
#define QMAN_CM_CFG_PRES_MASK                    0x0000ffff
#define QMAN_CM_CFG_PRES(_x)                    ((_x & QMAN_CM_CFG_PRES_MASK))
// QMAN_ECSR
#define QMAN_ECSR_ME                             0x80000000
#define QMAN_ECSR_MBEI                           0x02000000
#define QMAN_ECSR_SBEI                           0x01000000
#define QMAN_ECSR_IFSI                           0x00020000
#define QMAN_ECSR_ICVI                           0x00010000
#define QMAN_ECSR_IDDI                           0x00000800
#define QMAN_ECSR_IDFI                           0x00000400
#define QMAN_ECSR_IDSI                           0x00000200
#define QMAN_ECSR_IDQI                           0x00000100
#define QMAN_ECSR_IEOI                           0x00000008
#define QMAN_ECSR_IESI                           0x00000004
#define QMAN_ECSR_IECI                           0x00000002
#define QMAN_ECSR_IEQI                           0x00000001
// QMAN_ECIR
#define QMAN_ECIR_FQID_MASK                      0x00ffffff
#define QMAN_ECIR_PORTAL_MASK                    0x1f000000
#define QMAN_ECIR_T                              0x02000000
#define QMAN_ECIR_FQID(_x)                      ((_x & QMAN_ECIR_FQID_MASK))
#define QMAN_ECIR_SET_PORTAL(_x)                ((_x << 24) & QMAN_ECIR_PORTAL_MASK)
#define QMAN_ECIR_GET_PORTAL(_x)                ((_x & QMAN_ECIR_PORTAL_MASK) >> 24)
// QMAN_EADR
#define QMAN_EADR_EADR_MASK                      0x00000fff
#define QMAN_EADR_MEM_ID_MASK                    0x0f000000
#define QMAN_EADR_EADR(_x)                      ((_x & QMAN_EADR_EADR_MASK))
#define QMAN_EADR_SET_MEM_ID(_x)                ((_x << 24) & QMAN_EADR_MEM_ID_MASK)
#define QMAN_EADR_GET_MEM_ID(_x)                ((_x & QMAN_EADR_MEM_ID_MASK) >> 24)
// QMAN_SBET
#define QMAN_SBET_SBET_MASK                      0x000000ff
#define QMAN_SBET_ECDD_MASK                      0xfe000000
#define QMAN_SBET_SBET(_x)                      ((_x & QMAN_SBET_SBET_MASK))
#define QMAN_SBET_SET_ECDD(_x)                  ((_x << 25) & QMAN_SBET_ECDD_MASK)
#define QMAN_SBET_GET_ECDD(_x)                  ((_x & QMAN_SBET_ECDD_MASK) >> 25)
// QMAN_SBECi
#define QMAN_SBEC_SBEC_MASK                      0x000000ff
#define QMAN_SBEC_SBEC(_x)                      ((_x & QMAN_SBEC_SBEC_MASK))
// QMAN_MCR
#define QMAN_MCR_CMD_RSLT_MASK                   0xff000000
#define QMAN_MCR_SET_CMD_RSLT(_x)               ((_x << 24) & QMAN_MCR_CMD_RSLT_MASK)
#define QMAN_MCR_GET_CMD_RSLT(_x)               ((_x & QMAN_MCR_CMD_RSLT_MASK) >> 24)
// QMAN_MISC_CFG
#define QMAN_MISC_CFG_FCM                        0x00000001
// QMAN_IDLE_STAT
#define QMAN_IDLE_STAT_I                         0x00000001
#define QMAN_IDLE_STAT_E                         0x00000002
// QMAN_IP_REV_1
#define QMAN_IP_REV_1_GET_IP_MN(_x)             ((_x & 0x000000ff))
#define QMAN_IP_REV_1_GET_IP_MJ(_x)             ((_x & 0x0000ff00) >> 8)
#define QMAN_IP_REV_1_GET_IP_ID(_x)             ((_x & 0xffff0000) >> 16)
// FQD_BARE
#define QMAN_FQD_BARE_EBA_MASK                   0x0000ffff
#define QMAN_FQD_BARE_EBA(_x)                   ((_x & QMAN_FQD_BARE_EBA_MASK))
// PFDR_BARE
#define QMAN_PFDR_BARE_EBA_MASK                  0x0000ffff
#define QMAN_PFDR_BARE_EBA(_x)                  ((_x & QMAN_PFDR_BARE_EBA_MASK))
// FQD_BAR
#define QMAN_FQD_BAR_BA_MASK                     0xfffff000
#define QMAN_FQD_SET_BAR_BA(_x)                 ((_x << 12) & QMAN_FQD_BAR_BA_MASK)
#define QMAN_FQD_GET_BAR_BA(_x)                 ((_x & QMAN_FQD_BAR_BA_MASK) >> 12)
// PFDR_BAR
#define QMAN_PFDR_BAR_BA_MASK                    0xfffff000
#define QMAN_PFDR_SET_BAR_BA(_x)                ((_x << 12) & QMAN_PFDR_BAR_BA_MASK)
#define QMAN_PFDR_GET_BAR_BA(_x)                ((_x & QMAN_PFDR_BAR_BA_MASK) >> 12)
// FQD_AR
#define QMAN_FQD_AR_SIZE_MASK                    0x0000003f
#define QMAN_FQD_AR_SIZE(_x)                    ((_x & QMAN_FQD_AR_SIZE_MASK))
#define QMAN_FQD_AR_SE                           0x20000000
#define QMAN_FQD_AR_P                            0x40000000
#define QMAN_FQD_AR_EN                           0x80000000
// QCSP_BARE
#define QMAN_QCSP_BARE_EBA_MASK                  0x0000ffff
#define QMAN_QCSP_BARE_EBA(_x)                  ((_x & QMAN_QCSP_BARE_EBA_MASK))
// QCSP_BAR
#define QMAN_QCSP_BAR_BA_MASK                    0xffe00000
#define QMAN_QCSP_SET_BAR_BA(_x)                ((_x << 21) & QMAN_QCSP_BAR_BA_MASK)
#define QMAN_QCSP_GET_BAR_BA(_x)                ((_x & QMAN_QCSP_BAR_BA_MASK) >> 21)
// CI_SCHED_CFG
#define QMAN_CI_SCHED_CFG_SW                     0x80000000
#define QMAN_CI_SCHED_CFG_BMAN_W_MASK            0x00000007
#define QMAN_CI_SCHED_CFG_RW_W_MASK              0x00000070
#define QMAN_CI_SCHED_CFG_SRQ_W_MASK             0x00000700
#define QMAN_CI_SCHED_CFG_SRCCIV_MASK            0xf0000000
#define QMAN_CI_SCHED_CFG_BMAN_W                ((_x & QMAN_CI_SCHED_CFG_BMAN_W_MASK))
#define QMAN_CI_SCHED_CFG_SET_RW_W              ((_x <<  4) &   QMAN_CI_SCHED_CFG_RW_W_MASK)
#define QMAN_CI_SCHED_CFG_SET_SRQ_W             ((_x <<  8) &  QMAN_CI_SCHED_CFG_SRQ_W_MASK)
#define QMAN_CI_SCHED_CFG_SET_SRCCIV            ((_x << 24) & QMAN_CI_SCHED_CFG_SRCCIV_MASK)
#define QMAN_CI_SCHED_CFG_GET_RW_W              ((_x & QMAN_CI_SCHED_CFG_RW_W_MASK)   >>  4)
#define QMAN_CI_SCHED_CFG_GET_SRQ_W             ((_x & QMAN_CI_SCHED_CFG_SRQ_W_MASK)  >>  8)
#define QMAN_CI_SCHED_CFG_GET_SRCCIV            ((_x & QMAN_CI_SCHED_CFG_SRCCIV_MASK) >> 24)
// QMAN_SRCIDR
#define QMAN_SRCIDR_SRCIDR_MASK                  0x000000ff
#define QMAN_SRCIDR_SRCIDR(_x)                  ((_x & QMAN_SRCIDR_SRCIDR_MASK))
// QMAN_LIODNR
#define QMAN_LIODNR_LIODNR_MASK                  0x00000fff
#define QMAN_LIODNR_LIODNR(_x)                  ((_x & QMAN_LIODNR_LIODNR_MASK))
// CI_RLM_CFG
#define QMAN_CI_RLM_CFG_RLM_TH_D_MASK            0x000f0000
#define QMAN_CI_RLM_CFG_RLM_TH_C_MASK            0x00f00000
#define QMAN_CI_RLM_CFG_RLM_TH_B_MASK            0x0f000000
#define QMAN_CI_RLM_CFG_RLM_TH_A_MASK            0xf0000000
#define QMAN_CI_RLM_CFG_SET_RLM_TH_D(_x)        ((_x << 16) & QMAN_CI_RLM_CFG_RLM_TH_D_MASK)
#define QMAN_CI_RLM_CFG_SET_RLM_TH_C(_x)        ((_x << 20) & QMAN_CI_RLM_CFG_RLM_TH_C_MASK)
#define QMAN_CI_RLM_CFG_SET_RLM_TH_B(_x)        ((_x << 24) & QMAN_CI_RLM_CFG_RLM_TH_B_MASK)
#define QMAN_CI_RLM_CFG_SET_RLM_TH_A(_x)        ((_x << 28) & QMAN_CI_RLM_CFG_RLM_TH_A_MASK)
#define QMAN_CI_RLM_CFG_GET_RLM_TH_D(_x)        ((_x & QMAN_CI_RLM_CFG_RLM_TH_D_MASK) >> 16)
#define QMAN_CI_RLM_CFG_GET_RLM_TH_C(_x)        ((_x & QMAN_CI_RLM_CFG_RLM_TH_C_MASK) >> 20)
#define QMAN_CI_RLM_CFG_GET_RLM_TH_B(_x)        ((_x & QMAN_CI_RLM_CFG_RLM_TH_B_MASK) >> 24)
#define QMAN_CI_RLM_CFG_GET_RLM_TH_A(_x)        ((_x & QMAN_CI_RLM_CFG_RLM_TH_A_MASK) >> 28)
// CI_RLM_AVG
#define QMAN_CI_RLM_AVG_FRAC_MASK                0x000000ff
#define QMAN_CI_RLM_AVG_INT_MASK                 0x000fff00
#define QMAN_CI_RLM_AVG_FRACT(_x)               ((_x & 0x000000ff))
#define QMAN_CI_RLM_AVG_SET_INT(_x)             ((_x << 8) & QMAN_CI_RLM_AVG_INT_MASK)
#define QMAN_CI_RLM_AVG_GET_INT(_x)             ((_x & QMAN_CI_RLM_AVG_INT_MASK) >> 8)
// QMAN_ERR_ISR,QMAN_ERR_IER, QMAN_ERR_ISDR, QMAN_ERR_HER
#define QMAN_ERR_IxR_CIDE                        0x20000000
#define QMAN_ERR_IxR_CTDE                        0x10000000
#define QMAN_ERR_IxR_CITT                        0x08000000
#define QMAN_ERR_IxR_PLWI                        0x04000000
#define QMAN_ERR_IxR_MBEI                        0x02000000
#define QMAN_ERR_IxR_SBEI                        0x01000000
#define QMAN_ERR_IxR_PEBI                        0x00800000
#define QMAN_ERR_IxR_IFSI                        0x00020000
#define QMAN_ERR_IxR_ICVI                        0x00010000
#define QMAN_ERR_IxR_IDDI                        0x00000800
#define QMAN_ERR_IxR_IDFI                        0x00000400
#define QMAN_ERR_IxR_IDSI                        0x00000200
#define QMAN_ERR_IxR_IDQI                        0x00000100
#define QMAN_ERR_IxR_IEOI                        0x00000008
#define QMAN_ERR_IxR_IESI                        0x00000004
#define QMAN_ERR_IxR_IECI                        0x00000002
#define QMAN_ERR_IxR_IEQI                        0x00000001

#endif
