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

Digital_OutputType OutputA5 =
{
    .PortPin = PIN_A5,
    .OutputType = PIN_OUT_TYPE_PUSH_PULL,
    .Speed = PIN_SPEED_LOW,
    .InitVal = DIGITAL_STATE_HIGH
};

Digital_InputType InputC13 =
{
    .PortPin = PIN_C13,
    .Resistor = PIN_RES_NONE,
};

static Bool WatchpointTriggered = False;
U32 GlobalVar[4] ALIGN(4) = { 0 };

/* -------------------------- Local function declarations -------------------------- */

/**
 * @brief Configure embedded flash memory, clocks & system timer
 * @todo Move into separate board support package module
 */
void Setup(void);

void TestFunc(void* Arg);
void TestFunc2(void* Arg);
void DummyButtonFunc(void);
void DummyCompHandler0(void);
void DummyCompHandler1(void);
void DummyCompHandler2(void);
void DummyCompHandler3(void);

/* ------------------------------ Program entry-point ------------------------------ */

int main(void)
{
    Setup();
    MemPool_Init();
    Crc_Enable();
    Crc_Crc8ConfigType Crc8Cfg = Crc_GetSAEJ1850Config();
    Crc_Crc8Init(&Crc8Cfg);
    Protocol_Init(USART2, 115200, PIN_A2, PIN_A3);
    WatchpointComparatorsInit();
    NVIC_SetPriority(DebugMonitor_IRQn, 0UL);
    DebugMonitorExceptionEnable();
    DataWatchpointSet(&GlobalVar[0], DummyCompHandler0);
    DataWatchpointSet(&GlobalVar[1], DummyCompHandler1);
    DataWatchpointSet(&GlobalVar[2], DummyCompHandler2);
    DataWatchpointSet(&GlobalVar[3], DummyCompHandler3);

    Digital_OutputInit(&OutputA5);
    Digital_InputInit(&InputC13);
    Exti_GpioInit(InputC13.PortPin, DummyButtonFunc, EXTI_TRIGGER_FALLING_EDGE);

    Osal_ThreadCreate(TestFunc, NULL, 1024, THREAD_PRIORITY_MEDIUM);
    Osal_ThreadCreate(TestFunc2, NULL, 1024, THREAD_PRIORITY_MEDIUM);
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

void TestFunc(void* Arg)
{
    UNUSED(Arg);
    while (True)
    {
        if (WatchpointTriggered) { Digital_Set(&OutputA5); }
        else { Digital_Clear(&OutputA5); }
    }
}

void TestFunc2(void* Arg)
{
    UNUSED(Arg);
    while (True)
    {
        Protocol_Run();
    }
}

void DummyButtonFunc(void)
{
    static U8 ButtonCnt = 0U;
    GlobalVar[ButtonCnt] = 0xDEADBEEFUL;
    ButtonCnt++;
    if (ButtonCnt > 3) { ButtonCnt = 0; }
}

void DummyCompHandler0(void)
{
    WatchpointTriggered = True;
}

void DummyCompHandler1(void)
{
    WatchpointTriggered = False;
}

void DummyCompHandler2(void)
{
    WatchpointTriggered = True;
}

void DummyCompHandler3(void)
{
    WatchpointTriggered = False;
}
