/**
 * @file main.c
 *
 * @brief Application level entry point. Upon reaching main()
 *        only the bare necessities for operation has been performed.
 */

/* Include directives */
#include "typedef.h"
#include "ST/stm32l4xx.h"
#include "systick.h"
#include "pin.h"
#include "clock_control.h"

/* Local function declarations */
void Setup(void);

/**
 * @brief Starting point for normal program execution.
 */
int main(void)
{
    Setup();
    Pin_Init();

    U64 TargetTime = SysTick_GetTicks() + 500U;

    while (1)
    {
        const U64 Timestamp = SysTick_GetTicks();
        if (Timestamp >= TargetTime)
        {
            Pin_GetPort(PIN_A5)->ODR ^= (1 << 5);
            TargetTime = Timestamp + 500U;
        }
    }

    return 0;
}

/* Local function definitions */
void Setup(void)
{
    /* Enable flash prefetch. */
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    /* Set number of flash wait states */
    U32 TempACR = FLASH->ACR;
    TempACR &= ~FLASH_ACR_LATENCY_Msk;
    TempACR |= FLASH_ACR_LATENCY_4WS;
    FLASH->ACR = TempACR;

    /* Enable instruction & data caches */
    FLASH->ACR |= (FLASH_ACR_DCEN | FLASH_ACR_ICEN);

    ClkCtrl_HsiEnable(true);
    ClkCtrl_SetPllInput(PLL_INPUT_HSI, PLL_M_4);
    ClkCtrl_SetPllN(PLL_MAIN, 40);
    ClkCtrl_SetPllR(PLL_MAIN, PLL_R_2);
    ClkCtrl_PllOutputEnable(PLL_MAIN, PLL_OUTPUT_R);
    ClkCtrl_PllEnable(PLL_MAIN, true);
    ClkCtrl_SetSysclkInput(SYSCLK_INPUT_PLL);
    ClkCtrl_SetAbhPrescaler(AHB_PS_1);
    ClkCtrl_SetApbPrescaler(APB1, APB_PS_1);
    ClkCtrl_SetApbPrescaler(APB2, APB_PS_1);

    ClkCtrl_PeripheralClockEnable(PCLK_GPIOA);

    /* Initialize SysTick with default configuration */
    const SysTick_ConfigType SysTickCfg = SysTick_GetDefaultConfig();
    SysTick_Init(&SysTickCfg);
}