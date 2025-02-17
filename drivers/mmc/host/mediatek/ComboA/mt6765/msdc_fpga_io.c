// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#ifdef pr_fmt
#undef pr_fmt
#endif

#define pr_fmt(fmt) "["KBUILD_MODNAME"]" fmt
#include <linux/io.h>
#include "mtk_sd.h"

#define FPGA_PWR_GPIO                   (0x10001E84)
#define FPGA_PWR_GPIO_EO                (0x10001E88)

static void __iomem *fpga_pwr_gpio;
static void __iomem *fpga_pwr_gpio_eo;

#define PWR_GPIO                         (fpga_pwr_gpio)
#define PWR_GPIO_EO                      (fpga_pwr_gpio_eo)
#define PWR_MASK_VOL_33                  (1 << 10)
#define PWR_MASK_VOL_18                  (1 << 9)
#define PWR_MASK_CARD                    (1 << 8)
#define PWR_MASK_VOL_33_MASK             (~(1 << 10))
#define PWR_MASK_CARD_MASK               (~(1 << 8))
#define PWR_MASK_VOL_18_MASK             (~(1 << 9))

void msdc_set_pwr_gpio_dir(void __iomem *fpga_pwr_gpio,
	void __iomem *fpga_pwr_gpio_eo)
{
	u16 l_val;

	l_val = MSDC_READ16(PWR_GPIO_EO);
	MSDC_WRITE16(PWR_GPIO_EO,
		(l_val | /* PWR_GPIO_L4_DIR | */
		 PWR_MASK_CARD | PWR_MASK_VOL_33 | PWR_MASK_VOL_18));

	l_val = MSDC_READ16(PWR_GPIO_EO);

	pr_no_debug("[%s]: pwr gpio dir = 0x%x\n", __func__, l_val);
}

void msdc_fpga_pwr_init(void)
{
	if (fpga_pwr_gpio == NULL) {
		fpga_pwr_gpio = ioremap(FPGA_PWR_GPIO, 8);
		if (fpga_pwr_gpio == NULL) {
			pr_notice("[%s] msdc ioremap error\n", __func__);
			WARN_ON(1);
		}
		fpga_pwr_gpio_eo = fpga_pwr_gpio + 0x4;
		pr_notice("FPGA PWR_GPIO, PWR_GPIO_EO address 0x%p, 0x%p\n",
			fpga_pwr_gpio, fpga_pwr_gpio_eo);
	}
	msdc_set_pwr_gpio_dir(fpga_pwr_gpio, fpga_pwr_gpio_eo);
}

bool hwPowerOn_fpga(void)
{
	u16 l_val;

	l_val = MSDC_READ16(PWR_GPIO);
#ifdef CONFIG_MTK_EMMC_SUPPORT
	MSDC_WRITE16(PWR_GPIO, (l_val | PWR_MASK_VOL_18 | PWR_MASK_CARD));
		/* | PWR_GPIO_L4_DIR)); */
#else
	MSDC_WRITE16(PWR_GPIO, (l_val | PWR_MASK_VOL_33 | PWR_MASK_CARD));
		/* | PWR_GPIO_L4_DIR)); */
#endif
	l_val = MSDC_READ16(PWR_GPIO);
	pr_no_debug("[%s]: pwr gpio = 0x%x\n", __func__, l_val);
	return true;
}
EXPORT_SYMBOL(hwPowerOn_fpga);

bool hwPowerSwitch_fpga(void)
{
	u16 l_val;

	l_val = MSDC_READ16(PWR_GPIO);
	MSDC_WRITE16(PWR_GPIO, (l_val & ~(PWR_MASK_VOL_33)));
	l_val = MSDC_READ16(PWR_GPIO);
	MSDC_WRITE16(PWR_GPIO, (l_val | PWR_MASK_VOL_18));

	l_val = MSDC_READ16(PWR_GPIO);
	pr_no_debug("[%s]: pwr gpio = 0x%x\n", __func__, l_val);
	return true;
}
EXPORT_SYMBOL(hwPowerSwitch_fpga);

bool hwPowerDown_fpga(void)
{
	u16 l_val;

	l_val = MSDC_READ16(PWR_GPIO);
#ifdef CONFIG_MTK_EMMC_SUPPORT
	MSDC_WRITE16(PWR_GPIO,
		(l_val & ~(PWR_MASK_VOL_18 | PWR_MASK_CARD)));
#else
	MSDC_WRITE16(PWR_GPIO,
		(l_val & ~(PWR_MASK_VOL_18 | PWR_MASK_VOL_33 | PWR_MASK_CARD)));
#endif
	l_val = MSDC_READ16(PWR_GPIO);
	pr_no_debug("[%s]: pwr gpio = 0x%x\n", __func__, l_val);
	return true;
}
EXPORT_SYMBOL(hwPowerDown_fpga);

void msdc_fpga_power(struct msdc_host *host, u32 on)
{
	if (on)
		hwPowerOn_fpga();
	else
		hwPowerDown_fpga();
}

void msdc_sd_power_switch(struct msdc_host *host, u32 on)
{
	if (on)
		hwPowerSwitch_fpga();
}

void msdc_select_clksrc(struct msdc_host *host, int clksrc)
{
	host->hclk = msdc_get_hclk(host->id, clksrc);
	host->hw->clk_src = clksrc;

	pr_notice("[%s]: msdc%d select clk_src as %d(%dKHz)\n", __func__,
		host->id, clksrc, host->hclk/1000);
}

/* do we need sync object or not */
void msdc_clk_status(int *status)
{

}
