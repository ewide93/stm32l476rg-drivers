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
#include "limit.h"

/* ----------------------------------- Move this! ---------------------------------- */

typedef struct
{
    Digital_InputType PinA;
    Digital_InputType PinB;
    Bool CurrentA;
    Bool PreviousA;
    Bool CurrentB;
    Bool Invert;
} RotEnc_SimpleRotEncType;

typedef enum
{
    ENCODER_EVENT_NONE = 0x00U,
    ENCODER_EVENT_CLOCKWISE = 0x01U,
    ENCODER_EVENT_ANTI_CLOCKWISE = 0x02U,
    ENCODER_EVENT_ENUM_LIMIT
} RotEnc_EventEnum;

void RotEnc_Init(RotEnc_SimpleRotEncType* RotEnc, Pin_PortPinEnum PinA, Pin_PortPinEnum PinB, Bool Invert)
{
    /* Initialize digital inputs */
    RotEnc->PinA.PortPin = PinA;
    RotEnc->PinA.Resistor = PIN_RES_NONE;
    RotEnc->PinB.PortPin = PinB;
    RotEnc->PinB.Resistor = PIN_RES_NONE;
    Digital_InputInit(&RotEnc->PinA);
    Digital_InputInit(&RotEnc->PinB);

    /* Read initial state of pins */
    RotEnc->CurrentA = Digital_Read(&RotEnc->PinA);
    RotEnc->PreviousA = RotEnc->CurrentA;
    RotEnc->CurrentB = Digital_Read(&RotEnc->PinB);

    /* Set rotation polarity */
    RotEnc->Invert = Invert;
}

RotEnc_EventEnum RotEnc_Poll(RotEnc_SimpleRotEncType* RotEnc)
{
    RotEnc_EventEnum Event = ENCODER_EVENT_NONE;
    RotEnc->PreviousA = RotEnc->CurrentA;
    RotEnc->CurrentA = Digital_Read(&RotEnc->PinA);
    RotEnc->CurrentB = Digital_Read(&RotEnc->PinB);

    if (RotEnc->CurrentA != RotEnc->PreviousA)
    {
        if (RotEnc->CurrentA) /* Rising edge */
        {
            if (!RotEnc->CurrentB) { Event = RotEnc->Invert ? ENCODER_EVENT_ANTI_CLOCKWISE : ENCODER_EVENT_CLOCKWISE; }
            else                   { Event = RotEnc->Invert ? ENCODER_EVENT_CLOCKWISE : ENCODER_EVENT_ANTI_CLOCKWISE; }
        }
        else /* Falling edge */
        {
            if (RotEnc->CurrentB) { Event = RotEnc->Invert ? ENCODER_EVENT_ANTI_CLOCKWISE : ENCODER_EVENT_CLOCKWISE; }
            else                  { Event = RotEnc->Invert ? ENCODER_EVENT_CLOCKWISE : ENCODER_EVENT_ANTI_CLOCKWISE; }
        }
    }

    return Event;
}


/* -------------------------------- Global variables ------------------------------- */

Digital_OutputType OutputA5 =
{
    .PortPin = PIN_A5,
    .OutputType = PIN_OUT_TYPE_PUSH_PULL,
    .Speed = PIN_SPEED_LOW,
    .InitVal = DIGITAL_STATE_HIGH
};

RotEnc_SimpleRotEncType Encoder = { 0 };

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
    RotEnc_Init(&Encoder, PIN_A0, PIN_A1, True);

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
    U32 Period_Ticks = Osal_msToTicks(2UL);
    U32 Timekeeping = Osal_GetTickCount();
    U16 Cnt = 0U;
    U16 Threshold = 250U;

    while (True)
    {
        /* Poll rotary encoder for event */
        RotEnc_EventEnum EncoderEvent = RotEnc_Poll(&Encoder);
        if (EncoderEvent == ENCODER_EVENT_CLOCKWISE)
        {
            Threshold += 5;
            Threshold = Limit_U16(Threshold, 25, 500);
        }
        else if (EncoderEvent == ENCODER_EVENT_ANTI_CLOCKWISE)
        {
            Threshold -= 5;
            Threshold = Limit_U16(Threshold, 25, 500);
        }
        else
        {
            __NOP();
        }

        /* Toggle LED */
        if (++Cnt >= Threshold)
        {
            Digital_Toggle(&OutputA5);
            Cnt = 0U;
        }

        /* Put thread to sleep until next iteration */
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
