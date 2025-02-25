/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2020 - 2024 Intel Corporation */

#ifndef IPU_PLATFORM_ISYS_CSI2_REG_H
#define IPU_PLATFORM_ISYS_CSI2_REG_H

#define CSI_REG_BASE			0x220000
#define CSI_REG_BASE_PORT(id)		((id) * 0x1000)

#define IPU_CSI_PORT_A_ADDR_OFFSET	\
		(CSI_REG_BASE + CSI_REG_BASE_PORT(0))
#define IPU_CSI_PORT_B_ADDR_OFFSET	\
		(CSI_REG_BASE + CSI_REG_BASE_PORT(1))
#define IPU_CSI_PORT_C_ADDR_OFFSET	\
		(CSI_REG_BASE + CSI_REG_BASE_PORT(2))
#define IPU_CSI_PORT_D_ADDR_OFFSET	\
		(CSI_REG_BASE + CSI_REG_BASE_PORT(3))
#define IPU_CSI_PORT_E_ADDR_OFFSET	\
		(CSI_REG_BASE + CSI_REG_BASE_PORT(4))
#define IPU_CSI_PORT_F_ADDR_OFFSET	\
		(CSI_REG_BASE + CSI_REG_BASE_PORT(5))
#define IPU_CSI_PORT_G_ADDR_OFFSET	\
		(CSI_REG_BASE + CSI_REG_BASE_PORT(6))
#define IPU_CSI_PORT_H_ADDR_OFFSET	\
		(CSI_REG_BASE + CSI_REG_BASE_PORT(7))

/* CSI Port Genral Purpose Registers */
#define CSI_REG_PORT_GPREG_SRST                 0x0
#define CSI_REG_PORT_GPREG_CSI2_SLV_REG_SRST    0x4
#define CSI_REG_PORT_GPREG_CSI2_PORT_CONTROL    0x8

/*
 * Port IRQs mapping events:
 * IRQ0 - CSI_FE event
 * IRQ1 - CSI_SYNC
 * IRQ2 - S2M_SIDS0TO7
 * IRQ3 - S2M_SIDS8TO15
 */
#define CSI_PORT_REG_BASE_IRQ_CSI               0x80
#define CSI_PORT_REG_BASE_IRQ_CSI_SYNC          0xA0
#define CSI_PORT_REG_BASE_IRQ_S2M_SIDS0TOS7     0xC0
#define CSI_PORT_REG_BASE_IRQ_S2M_SIDS8TOS15    0xE0

#define CSI_PORT_REG_BASE_IRQ_EDGE_OFFSET	0x0
#define CSI_PORT_REG_BASE_IRQ_MASK_OFFSET	0x4
#define CSI_PORT_REG_BASE_IRQ_STATUS_OFFSET	0x8
#define CSI_PORT_REG_BASE_IRQ_CLEAR_OFFSET	0xc
#define CSI_PORT_REG_BASE_IRQ_ENABLE_OFFSET	0x10
#define CSI_PORT_REG_BASE_IRQ_LEVEL_NOT_PULSE_OFFSET	0x14

#define IPU6SE_CSI_RX_ERROR_IRQ_MASK		0x7ffff
#define IPU6_CSI_RX_ERROR_IRQ_MASK		0xfffff

#define CSI_RX_NUM_ERRORS_IN_IRQ		20
#define CSI_RX_NUM_IRQ				32

#define IPU_CSI_RX_IRQ_FS_VC(chn)	(1 << ((chn) * 2))
#define IPU_CSI_RX_IRQ_FE_VC(chn)	(2 << ((chn) * 2))

/* PPI2CSI */
#define CSI_REG_PPI2CSI_ENABLE                  0x200
#define CSI_REG_PPI2CSI_CONFIG_PPI_INTF         0x204
#define PPI_INTF_CONFIG_NOF_ENABLED_DATALANES_SHIFT	3
#define PPI_INTF_CONFIG_RX_AUTO_CLKGATING_SHIFT		5
#define CSI_REG_PPI2CSI_CONFIG_CSI_FEATURE      0x208

enum CSI_PPI2CSI_CTRL {
	CSI_PPI2CSI_DISABLE = 0,
	CSI_PPI2CSI_ENABLE = 1,
};

/* CSI_FE */
#define CSI_REG_CSI_FE_ENABLE                   0x280
#define CSI_REG_CSI_FE_MODE                     0x284
#define CSI_REG_CSI_FE_MUX_CTRL                 0x288
#define CSI_REG_CSI_FE_SYNC_CNTR_SEL            0x290

enum CSI_FE_ENABLE_TYPE {
	CSI_FE_DISABLE = 0,
	CSI_FE_ENABLE = 1,
};

enum CSI_FE_MODE_TYPE {
	CSI_FE_DPHY_MODE = 0,
	CSI_FE_CPHY_MODE = 1,
};

enum CSI_FE_INPUT_SELECTOR {
	CSI_SENSOR_INPUT = 0,
	CSI_MIPIGEN_INPUT = 1,
};

enum CSI_FE_SYNC_CNTR_SEL_TYPE {
	CSI_CNTR_SENSOR_LINE_ID = (1 << 0),
	CSI_CNTR_INT_LINE_PKT_ID = ~CSI_CNTR_SENSOR_LINE_ID,
	CSI_CNTR_SENSOR_FRAME_ID = (1 << 1),
	CSI_CNTR_INT_FRAME_PKT_ID = ~CSI_CNTR_SENSOR_FRAME_ID,
};

/* MIPI_PKT_GEN */
#define CSI_REG_PIXGEN_COM_BASE_OFFSET		0x300

#define IPU_CSI_PORT_A_PIXGEN_ADDR_OFFSET	\
	(CSI_REG_BASE + CSI_REG_BASE_PORT(0) + CSI_REG_PIXGEN_COM_BASE_OFFSET)
#define IPU_CSI_PORT_B_PIXGEN_ADDR_OFFSET	\
	(CSI_REG_BASE + CSI_REG_BASE_PORT(1) + CSI_REG_PIXGEN_COM_BASE_OFFSET)
#define IPU_CSI_PORT_C_PIXGEN_ADDR_OFFSET	\
	(CSI_REG_BASE + CSI_REG_BASE_PORT(2) + CSI_REG_PIXGEN_COM_BASE_OFFSET)
#define IPU_CSI_PORT_D_PIXGEN_ADDR_OFFSET	\
	(CSI_REG_BASE + CSI_REG_BASE_PORT(3) + CSI_REG_PIXGEN_COM_BASE_OFFSET)
#define IPU_CSI_PORT_E_PIXGEN_ADDR_OFFSET	\
	(CSI_REG_BASE + CSI_REG_BASE_PORT(4) + CSI_REG_PIXGEN_COM_BASE_OFFSET)
#define IPU_CSI_PORT_F_PIXGEN_ADDR_OFFSET	\
	(CSI_REG_BASE + CSI_REG_BASE_PORT(5) + CSI_REG_PIXGEN_COM_BASE_OFFSET)
#define IPU_CSI_PORT_G_PIXGEN_ADDR_OFFSET	\
	(CSI_REG_BASE + CSI_REG_BASE_PORT(6) + CSI_REG_PIXGEN_COM_BASE_OFFSET)
#define IPU_CSI_PORT_H_PIXGEN_ADDR_OFFSET	\
	(CSI_REG_BASE + CSI_REG_BASE_PORT(7) + CSI_REG_PIXGEN_COM_BASE_OFFSET)

#define CSI_REG_PIXGEN_COM_ENABLE_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x300)
#define CSI_REG_PIXGEN_COM_DTYPE_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x304)
#define CSI_REG_PIXGEN_COM_VTYPE_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x308)
#define CSI_REG_PIXGEN_COM_VCHAN_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x30C)
#define CSI_REG_PIXGEN_COM_WCOUNT_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x310)
/* PRBS */
#define CSI_REG_PIXGEN_PRBS_RSTVAL_REG0_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x314)
#define CSI_REG_PIXGEN_PRBS_RSTVAL_REG1_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x318)
/* SYNC_GENERATOR_CONFIG */
#define CSI_REG_PIXGEN_SYNG_FREE_RUN_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x31C)
#define CSI_REG_PIXGEN_SYNG_PAUSE_REG_IDX(id)		\
	(CSI_REG_BASE_PORT(id) + 0x320)
#define CSI_REG_PIXGEN_SYNG_NOF_FRAME_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x324)
#define CSI_REG_PIXGEN_SYNG_NOF_PIXEL_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x328)
#define CSI_REG_PIXGEN_SYNG_NOF_LINE_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x32C)
#define CSI_REG_PIXGEN_SYNG_HBLANK_CYC_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x330)
#define CSI_REG_PIXGEN_SYNG_VBLANK_CYC_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x334)
#define CSI_REG_PIXGEN_SYNG_STAT_HCNT_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x338)
#define CSI_REG_PIXGEN_SYNG_STAT_VCNT_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x33C)
#define CSI_REG_PIXGEN_SYNG_STAT_FCNT_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x340)
#define CSI_REG_PIXGEN_SYNG_STAT_DONE_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x344)
/* TPG */
#define CSI_REG_PIXGEN_TPG_MODE_REG_IDX(id)		\
	(CSI_REG_BASE_PORT(id) + 0x348)
/* TPG: mask_cfg */
#define CSI_REG_PIXGEN_TPG_HCNT_MASK_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x34C)
#define CSI_REG_PIXGEN_TPG_VCNT_MASK_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x350)
#define CSI_REG_PIXGEN_TPG_XYCNT_MASK_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x354)
/* TPG: delta_cfg */
#define CSI_REG_PIXGEN_TPG_HCNT_DELTA_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x358)
#define CSI_REG_PIXGEN_TPG_VCNT_DELTA_REG_IDX(id)	\
	(CSI_REG_BASE_PORT(id) + 0x35C)
/* TPG: color_cfg */
#define CSI_REG_PIXGEN_TPG_R1_REG_IDX(id)	(CSI_REG_BASE_PORT(id) + 0x360)
#define CSI_REG_PIXGEN_TPG_G1_REG_IDX(id)	(CSI_REG_BASE_PORT(id) + 0x364)
#define CSI_REG_PIXGEN_TPG_B1_REG_IDX(id)	(CSI_REG_BASE_PORT(id) + 0x368)
#define CSI_REG_PIXGEN_TPG_R2_REG_IDX(id)	(CSI_REG_BASE_PORT(id) + 0x36C)
#define CSI_REG_PIXGEN_TPG_G2_REG_IDX(id)	(CSI_REG_BASE_PORT(id) + 0x370)
#define CSI_REG_PIXGEN_TPG_B2_REG_IDX(id)	(CSI_REG_BASE_PORT(id) + 0x374)

#define CSI_REG_PIXGEN_PRBS_RSTVAL_REG0	CSI_REG_PIXGEN_PRBS_RSTVAL_REG0_IDX(0)
#define CSI_REG_PIXGEN_PRBS_RSTVAL_REG1	CSI_REG_PIXGEN_PRBS_RSTVAL_REG1_IDX(0)
#define CSI_REG_PIXGEN_COM_ENABLE_REG	CSI_REG_PIXGEN_COM_ENABLE_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_MODE_REG	CSI_REG_PIXGEN_TPG_MODE_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_R1_REG	CSI_REG_PIXGEN_TPG_R1_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_G1_REG	CSI_REG_PIXGEN_TPG_G1_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_B1_REG	CSI_REG_PIXGEN_TPG_B1_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_R2_REG	CSI_REG_PIXGEN_TPG_R2_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_G2_REG	CSI_REG_PIXGEN_TPG_G2_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_B2_REG	CSI_REG_PIXGEN_TPG_B2_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_HCNT_MASK_REG CSI_REG_PIXGEN_TPG_HCNT_MASK_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_VCNT_MASK_REG CSI_REG_PIXGEN_TPG_VCNT_MASK_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_XYCNT_MASK_REG	\
	CSI_REG_PIXGEN_TPG_XYCNT_MASK_REG_IDX(0)

#define CSI_REG_PIXGEN_SYNG_NOF_FRAME_REG	\
	CSI_REG_PIXGEN_SYNG_NOF_FRAME_REG_IDX(0)
#define CSI_REG_PIXGEN_SYNG_NOF_LINE_REG	\
	CSI_REG_PIXGEN_SYNG_NOF_LINE_REG_IDX(0)
#define CSI_REG_PIXGEN_SYNG_HBLANK_CYC_REG	\
	CSI_REG_PIXGEN_SYNG_HBLANK_CYC_REG_IDX(0)
#define CSI_REG_PIXGEN_SYNG_VBLANK_CYC_REG	\
	CSI_REG_PIXGEN_SYNG_VBLANK_CYC_REG_IDX(0)
#define CSI_REG_PIXGEN_SYNG_NOF_PIXEL_REG	\
	CSI_REG_PIXGEN_SYNG_NOF_PIXEL_REG_IDX(0)
#define CSI_REG_PIXGEN_COM_WCOUNT_REG	CSI_REG_PIXGEN_COM_WCOUNT_REG_IDX(0)
#define CSI_REG_PIXGEN_COM_DTYPE_REG	CSI_REG_PIXGEN_COM_DTYPE_REG_IDX(0)
#define CSI_REG_PIXGEN_COM_VTYPE_REG	CSI_REG_PIXGEN_COM_VTYPE_REG_IDX(0)
#define CSI_REG_PIXGEN_COM_VCHAN_REG	CSI_REG_PIXGEN_COM_VCHAN_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_HCNT_DELTA_REG	\
	CSI_REG_PIXGEN_TPG_HCNT_DELTA_REG_IDX(0)
#define CSI_REG_PIXGEN_TPG_VCNT_DELTA_REG	\
	CSI_REG_PIXGEN_TPG_VCNT_DELTA_REG_IDX(0)

/* CSI HUB General Purpose Registers */
#define CSI_REG_HUB_GPREG_SRST			(CSI_REG_BASE + 0x18000)
#define CSI_REG_HUB_GPREG_SLV_REG_SRST		(CSI_REG_BASE + 0x18004)
#define CSI_REG_HUB_GPREG_PHY_CONTROL(id)	\
	(CSI_REG_BASE + 0x18008 + (id) * 0x8)
#define CSI_REG_HUB_GPREG_PHY_CONTROL_RESET		BIT(4)
#define CSI_REG_HUB_GPREG_PHY_CONTROL_PWR_EN		BIT(0)
#define CSI_REG_HUB_GPREG_PHY_STATUS(id)	\
	(CSI_REG_BASE + 0x1800c + (id) * 0x8)
#define CSI_REG_HUB_GPREG_PHY_STATUS_POWER_ACK		BIT(0)
#define CSI_REG_HUB_GPREG_PHY_STATUS_PHY_READY		BIT(4)

#define CSI_REG_HUB_DRV_ACCESS_PORT(id)	(CSI_REG_BASE + 0x18018 + (id) * 4)
#define CSI_REG_HUB_FW_ACCESS_PORT(id)	(CSI_REG_BASE + 0x17000 + (id) * 4)
#define IPU6V6_CSI_REG_HUB_FW_ACCESS_PORT(id)	\
	(CSI_REG_BASE + 0x16000 + (id) * 4)

enum CSI_PORT_CLK_GATING_SWITCH {
	CSI_PORT_CLK_GATING_OFF = 0,
	CSI_PORT_CLK_GATING_ON = 1,
};

#define CSI_REG_BASE_HUB_IRQ                        0x18200

#define IPU_NUM_OF_DLANE_REG_PORT0      2
#define IPU_NUM_OF_DLANE_REG_PORT1      4
#define IPU_NUM_OF_DLANE_REG_PORT2      4
#define IPU_NUM_OF_DLANE_REG_PORT3      2
#define IPU_NUM_OF_DLANE_REG_PORT4      2
#define IPU_NUM_OF_DLANE_REG_PORT5      4
#define IPU_NUM_OF_DLANE_REG_PORT6      4
#define IPU_NUM_OF_DLANE_REG_PORT7      2

/* ipu6se support 2 SIPs, 2 port per SIP, 4 ports 0..3
 * sip0 - 0, 1
 * sip1 - 2, 3
 * 0 and 2 support 4 data lanes, 1 and 3 support 2 data lanes
 * all offset are base from isys base address
 */

#define CSI2_HUB_GPREG_SIP_SRST(sip)			(0x238038 + (sip) * 4)
#define CSI2_HUB_GPREG_SIP_FB_PORT_CFG(sip)		(0x238050 + (sip) * 4)

#define CSI2_HUB_GPREG_DPHY_TIMER_INCR			(0x238040)
#define CSI2_HUB_GPREG_HPLL_FREQ			(0x238044)
#define CSI2_HUB_GPREG_IS_CLK_RATIO			(0x238048)
#define CSI2_HUB_GPREG_HPLL_FREQ_ISCLK_RATE_OVERRIDE	(0x23804c)
#define CSI2_HUB_GPREG_PORT_CLKGATING_DISABLE		(0x238058)
#define CSI2_HUB_GPREG_SIP0_CSI_RX_A_CONTROL		(0x23805c)
#define CSI2_HUB_GPREG_SIP0_CSI_RX_B_CONTROL		(0x238088)
#define CSI2_HUB_GPREG_SIP1_CSI_RX_A_CONTROL		(0x2380a4)
#define CSI2_HUB_GPREG_SIP1_CSI_RX_B_CONTROL		(0x2380d0)

#define CSI2_SIP_TOP_CSI_RX_BASE(sip)		(0x23805c + (sip) * 0x48)
#define CSI2_SIP_TOP_CSI_RX_PORT_BASE_0(port)	(0x23805c + ((port) / 2) * 0x48)
#define CSI2_SIP_TOP_CSI_RX_PORT_BASE_1(port)	(0x238088 + ((port) / 2) * 0x48)

/* offset from port base */
#define CSI2_SIP_TOP_CSI_RX_PORT_CONTROL		(0x0)
#define CSI2_SIP_TOP_CSI_RX_DLY_CNT_TERMEN_CLANE	(0x4)
#define CSI2_SIP_TOP_CSI_RX_DLY_CNT_SETTLE_CLANE	(0x8)
#define CSI2_SIP_TOP_CSI_RX_DLY_CNT_TERMEN_DLANE(lane)	(0xc + (lane) * 8)
#define CSI2_SIP_TOP_CSI_RX_DLY_CNT_SETTLE_DLANE(lane)	(0x10 + (lane) * 8)

#endif /* IPU6_ISYS_CSI2_REG_H */
