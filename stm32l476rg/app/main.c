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

    Uart_ConfigType UartCfg =
    {
        .BaudRate = 115200,
        .Oversampling = UART_OVERSAMPLING_16,
        .SamplingMethod = UART_SAMPLING_3_BITS,
        .Parity = UART_PARITY_NONE,
        .WordLength = UART_WORD_LEN_8,
        .StopBits = UART_STOP_BITS_1,
        .RxPin = PIN_A3,
        .TxPin = PIN_A2
    };
    Uart_HandleType Uart2 = Uart_Init(USART2, &UartCfg);
    Uart_TxEnable(Uart2);
    Uart_RxEnable(Uart2);
    Uart_Enable(Uart2);

    Digital_OutputType OutputA5 =
    {
        .PortPin = PIN_A5,
        .OutputType = PIN_OUT_TYPE_PUSH_PULL,
        .Speed = PIN_SPEED_LOW,
        .InitValHigh = True
    };
    Digital_OutputInit(&OutputA5);

    Uart_TransmitString(Uart2, "Booted up!\n", 11);

    Crc_Enable();
    Crc_Crc8ConfigType Crc8Cfg = Crc_GetSAEJ1850Config();
    Crc_Crc8Init(&Crc8Cfg);

    const U8 DummyBuffer[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
    U8 DummyCrc = Crc_CalcCrc8(DummyBuffer, sizeof(DummyBuffer) / sizeof(DummyBuffer[0]));
    Uart_TransmitChar(Uart2, DummyCrc);

    while (1)
    {
        Char RxData[16] = { 0 };
        if (Uart_Recieve(Uart2, (U8*)RxData, 3))
        {
            if (RxData[0] =='S' && RxData[1] == 'e' && RxData[2] == 't')
            {
                Digital_Set(&OutputA5);
            }
            if (RxData[0] =='C' && RxData[1] == 'l' && RxData[2] == 'r')
            {
                Digital_Clear(&OutputA5);
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

    /* Basic setup for USART2 */
    ClkCtrl_PeripheralClockEnable(PCLK_USART2);

    /* Initialize SysTick with default configuration */
    const SysTick_ConfigType SysTickCfg = SysTick_GetDefaultConfig();
    SysTick_Init(&SysTickCfg);
}