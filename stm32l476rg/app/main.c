/**
 * @file main.c
 *
 * @brief Application level entry point. Upon reaching main()
 *        only the bare necessities for operation has been performed.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"
#include "stm32l4xx.h"
#include "systick.h"
#include "pin.h"
#include "clock_control.h"
#include "flash.h"
#include "digital.h"

/* -------------------------- Local function declarations -------------------------- */

/**
 * @brief Configure embedded flash memory, clocks & system timer
 * @todo Move into separate board support package module
 */
void Setup(void);

/* ------------------------------ Program entry-point ------------------------------ */

int main(void)
{
    Setup();
    Digital_OutputType OutputA5 =
    {
        .PortPin = PIN_A5,
        .OutputType = PIN_OUT_TYPE_PUSH_PULL,
        .Speed = PIN_SPEED_LOW,
        .InitValHigh = True
    };
    Digital_OutputInit(&OutputA5);

    U64 TargetTime = SysTick_GetTicks() + 500U;

    while (1)
    {
        const U64 Timestamp = SysTick_GetTicks();
        if (Timestamp >= TargetTime)
        {
            TargetTime = Timestamp + 500U;
            if (OutputA5.State == HIGH)
            {
                Digital_Clear(&OutputA5);
            }
            else
            {
                Digital_Set(&OutputA5);
            }
        }
    }

    return 0;
}

/* --------------------------- Local function definitions -------------------------- */

void Setup(void)
{
    Flash_PrefetchEnable();
    Flash_InstructionCacheEnable();
    Flash_DataCacheEnable();
    Flash_SetFlashLatency(FLASH_WS_4);

    ClkCtrl_HsiEnable(True);
    ClkCtrl_SetPllInput(PLL_INPUT_HSI, PLL_M_4);
    ClkCtrl_SetPllN(PLL_MAIN, 40);
    ClkCtrl_SetPllR(PLL_MAIN, PLL_R_2);
    ClkCtrl_PllOutputEnable(PLL_MAIN, PLL_OUTPUT_R);
    ClkCtrl_PllEnable(PLL_MAIN, True);
    ClkCtrl_SetSysclkInput(SYSCLK_INPUT_PLL);
    ClkCtrl_SetAhbPrescaler(AHB_PS_1);
    ClkCtrl_SetApbPrescaler(APB1, APB_PS_1);
    ClkCtrl_SetApbPrescaler(APB2, APB_PS_1);

    ClkCtrl_PeripheralClockEnable(PCLK_GPIOA);

    /* Initialize SysTick with default configuration */
    const SysTick_ConfigType SysTickCfg = SysTick_GetDefaultConfig();
    SysTick_Init(&SysTickCfg);
}