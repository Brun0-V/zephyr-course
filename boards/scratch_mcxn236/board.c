#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <fsl_clock.h>
#include <fsl_spc.h>
#include <soc.h>

extern uint32_t SystemCoreClock;

/* Core clock: 150 MHz from PLL0, console UART on FLEXCOMM4 */
void board_early_init_hook(void)
{
	spc_active_mode_dcdc_option_t dcdc = {
		.DCDCVoltage       = kSPC_DCDC_OverdriveVoltage,
		.DCDCDriveStrength = kSPC_DCDC_NormalDriveStrength,
	};
	spc_active_mode_core_ldo_option_t ldo = {
		.CoreLDOVoltage       = kSPC_CoreLDO_OverDriveVoltage,
		.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength,
	};
	spc_sram_voltage_config_t sram = {
		.operateVoltage       = kSPC_sramOperateAt1P2V,
		.requestVoltageUpdate = true,
	};
	const pll_setup_t pll0 = {
		.pllctrl = SCG_APLLCTRL_SOURCE(1U) | SCG_APLLCTRL_SELI(27U) |
			   SCG_APLLCTRL_SELP(13U),
		.pllndiv = SCG_APLLNDIV_NDIV(8U),
		.pllpdiv = SCG_APLLPDIV_PDIV(1U),
		.pllmdiv = SCG_APLLMDIV_MDIV(50U),
		.pllRate = 150000000U,
	};

	SPC_SetActiveModeDCDCRegulatorConfig(SPC0, &dcdc);
	SPC_SetActiveModeCoreLDORegulatorConfig(SPC0, &ldo);
	SPC_SetSRAMOperateVoltage(SPC0, &sram);

	CLOCK_EnableClock(kCLOCK_Scg);
	CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);
	FMU0->FCTRL = (FMU0->FCTRL & ~FMU_FCTRL_RWSC_MASK) | FMU_FCTRL_RWSC(0x3U);
	CLOCK_SetupFROHFClocking(48000000U);
	CLOCK_SetPLL0Freq(&pll0);
	CLOCK_SetPll0MonitorMode(kSCG_Pll0MonitorDisable);
	CLOCK_AttachClk(kPLL0_to_MAIN_CLK);
	CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U);

	CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1U);
	CLOCK_AttachClk(kFRO_HF_DIV_to_FLEXCOMM4);

	SystemCoreClock = 150000000U;
}

static int board_scratch_mcxn236_init(void)
{
	printk("Board Initialized\n");
	return 0;
}

SYS_INIT(board_scratch_mcxn236_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
