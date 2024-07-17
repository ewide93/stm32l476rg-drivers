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
            GPIOA->ODR ^= GPIO_ODR_OD5;
            TargetTime = Timestamp + 500U;
        }
    }

    return 0;
}

/* Local function definitions */
void Setup(void)
{
    /* Enable high speed internal oscillator (16 MHz) */
    RCC->CR |= RCC_CR_HSION;

    /* Enable flash prefetch. */
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    /* Set number of flash wait states */
    U32 TempACR = FLASH->ACR;
    TempACR &= ~FLASH_ACR_LATENCY_Msk;
    TempACR |= FLASH_ACR_LATENCY_4WS;
    FLASH->ACR = TempACR;

    /* Enable instruction & data caches */
    FLASH->ACR |= (FLASH_ACR_DCEN | FLASH_ACR_ICEN);

    /* PLL configuration */
    U32 TempPLLCFGR = RCC->PLLCFGR;

    /* Set clock source for PLL to HSI oscillator */
    TempPLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;

    /* Set PLL multiplication factor to 40 */
    TempPLLCFGR &= ~(0x3F << 8U);
    TempPLLCFGR |= (40U << RCC_PLLCFGR_PLLN_Pos);

    /* Set PLL division factor to 4 */
    TempPLLCFGR |= 3U << RCC_PLLCFGR_PLLM_Pos;

    /* Enable PLL output R */
    TempPLLCFGR |= RCC_PLLCFGR_PLLREN;

    /* Divide PLL output by 2 */
    TempPLLCFGR &= ~RCC_PLLCFGR_PLLR_Msk;
    RCC->PLLCFGR = TempPLLCFGR;

    /* Enable the PLL & wait for it to be ready */
    RCC->CR |= RCC_CR_PLLON;
    while ( !(RCC->CR & RCC_CR_PLLRDY) ) { __NOP(); }

    /* Enable clock for GPIO port A */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /* Set system clock source to main PLL */
    U32 TempCFGR = RCC->CFGR;
    TempCFGR &= !RCC_CFGR_SW_Msk;
    TempCFGR |= RCC_CFGR_SW_PLL;
    RCC->CFGR = TempCFGR;

    /* Wait for clock to be ready */
    while ( !(RCC->CFGR & RCC_CFGR_SWS_PLL) ) { __NOP(); }

    /* Initialize SysTick with default configuration */
    const SysTick_ConfigType SysTickCfg = SysTick_GetDefaultConfig();
    SysTick_Init(&SysTickCfg);
}