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
#include "uart.h"
#include "crc.h"
#include "protocol.h"
#include "exti.h"
#include "mempool.h"
#include "osal.h"
#include "core_debug.h"

/* -------------------------------- Global variables ------------------------------- */

Digital_OutputType OutputA5 =
{
    .PortPin = PIN_A5,
    .OutputType = PIN_OUT_TYPE_PUSH_PULL,
    .Speed = PIN_SPEED_LOW,
    .InitVal = DIGITAL_STATE_HIGH
};

/* -------------------------- Local function declarations -------------------------- */

/**
 * @brief Configure embedded flash memory, clocks & system timer
 * @todo Move into separate board support package module
 */
void Setup(void);

void BlinkThreadFunc(void* Arg);
void CommThreadFunc(void* Arg);

/* ------------------------------ Program entry-point ------------------------------ */

int main(void)
{
    Setup();
    MemPool_Init();
    Crc_Enable();
    Crc_Crc8ConfigType Crc8Cfg = Crc_GetSAEJ1850Config();
    Crc_Crc8Init(&Crc8Cfg);
    Protocol_Init(USART2, 115200, PIN_A2, PIN_A3);

    Digital_OutputInit(&OutputA5);

    Osal_ThreadCreate(BlinkThreadFunc, NULL, 1024, THREAD_PRIORITY_MEDIUM);
    Osal_ThreadCreate(CommThreadFunc, NULL, 1024, THREAD_PRIORITY_MEDIUM);
    Osal_StartScheduler();

    while (1)
    {
        /* Should never get here after OS scheduler is started. */
    }

    return 0;
}

/* --------------------------- Local function definitions -------------------------- */

void Setup(void)
{
    /**
     * Enable data & instruction caches and set an appropriate number
     * of wait states based on a system clock frequency of 80 MHz.
     */
    Flash_PrefetchEnable();
    Flash_InstructionCacheEnable();
    Flash_DataCacheEnable();
    Flash_SetFlashLatency(FLASH_WS_4);

    /**
     * Configure the clock tree to yield the following result:
     * - System clock frequency (SYSCLK & HCLK): 80 MHz
     * - Peripheral bus 1 (PCLK1): 80 MHz
     * - Peripheral bus 2 (PCLK2): 80 MHz
     */
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

    /**
     * Enable clocks for used peripherals
     */
    ClkCtrl_PeripheralClockEnable(PCLK_GPIOA);
    ClkCtrl_PeripheralClockEnable(PCLK_GPIOC);
    ClkCtrl_PeripheralClockEnable(PCLK_USART2);

    /**
     * Initialize SysTick with default configuration in
     * cases where we're running without FreeRTOS.
     */
    #if defined(OSAL_CONFIG_USE_BARE_METAL) && (OSAL_CONFIG_USE_BARE_METAL == 1)
        const SysTick_ConfigType SysTickCfg = SysTick_GetDefaultConfig();
        SysTick_Init(&SysTickCfg);
    #elif defined(OSAL_CONFIG_USE_FREERTOS) && (OSAL_CONFIG_USE_FREERTOS == 1)
        NVIC_SetPriorityGrouping(0U);
    #endif
}

void BlinkThreadFunc(void* Arg)
{
    UNUSED(Arg);
    U32 Period_Ticks = Osal_msToTicks(10UL);
    U32 Timekeeping = Osal_GetTickCount();
    U8 Cnt = 0U;
    while (True)
    {
        if (++Cnt > 10)
        {
            Digital_Toggle(&OutputA5);
            Cnt = 0U;
        }
        Osal_DelayUntil(&Timekeeping, Period_Ticks);
    }
}

void CommThreadFunc(void* Arg)
{
    UNUSED(Arg);
    while (True)
    {
        Protocol_Run();
    }
}
