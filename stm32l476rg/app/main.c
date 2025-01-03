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

Digital_OutputType OutputA5 =
{
    .PortPin = PIN_A5,
    .OutputType = PIN_OUT_TYPE_PUSH_PULL,
    .Speed = PIN_SPEED_LOW,
    .InitVal = HIGH
};

Digital_InputType InputC13 =
{
    .PortPin = PIN_C13,
    .Resistor = PIN_RES_NONE,
};

/* -------------------------- Local function declarations -------------------------- */

/**
 * @brief Configure embedded flash memory, clocks & system timer
 * @todo Move into separate board support package module
 */
void Setup(void);

void TestFunc(void);

/* ------------------------------ Program entry-point ------------------------------ */

int main(void)
{
    Setup();
    Crc_Enable();
    Crc_Crc8ConfigType Crc8Cfg = Crc_GetSAEJ1850Config();
    Crc_Crc8Init(&Crc8Cfg);
    Protocol_Init(USART2, 115200, PIN_A2, PIN_A3);

    Digital_OutputInit(&OutputA5);
    Digital_InputInit(&InputC13);
    Exti_GpioInit(InputC13.PortPin, TestFunc, EXTI_TRIGGER_FALLING_EDGE);

    while (1)
    {
        Protocol_Run();
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
    ClkCtrl_PeripheralClockEnable(PCLK_GPIOC);
    ClkCtrl_PeripheralClockEnable(PCLK_SYSCFG);

    /* Basic setup for USART2 */
    ClkCtrl_PeripheralClockEnable(PCLK_USART2);

    /* Initialize SysTick with default configuration */
    const SysTick_ConfigType SysTickCfg = SysTick_GetDefaultConfig();
    SysTick_Init(&SysTickCfg);
}

void TestFunc(void)
{
    Digital_Toggle(&OutputA5);
}
