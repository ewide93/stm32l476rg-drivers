/**
 * @file startup.c
 *
 * @brief Startup code for STM32L467RG microcontroller.
 *        Initializes stack pointer, the interrupt vector table and the
 *        different sections of flash and RAM.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "startup.h"
#include "osal.h"

/* Symbols defined in linker script */
extern U32 _data_loadaddr;
extern U32 _data;
extern U32 _edata;
extern U32 _ebss;
extern U32 _stack;

/* Vector table definition */
VectorTableType VectorTable __attribute__((section(".isr_vector"))) =
{
    .StackTop = &_stack,
    .ResetHandler = ResetHandler,
    .NMIHandler = NMIHandler,
    .HardFaultHandler = HardFaultHandler,
    .MemManageHandler = MemManageHandler,
    .BusFaultHandler = BusFaultHandler,
    .UsageFaultHandler = UsageFaultHandler,
    .ReservedSection0 = { 0, 0, 0, 0 },
    /* OSAL configuration */
    #if defined(OSAL_CONFIG_USE_FREERTOS) && (OSAL_CONFIG_USE_FREERTOS == 1)
        .SVCallHandler = vPortSVCHandler,
        .PendSVHandler = xPortPendSVHandler,
        .SysTickHandler = xPortSysTickHandler,
    #elif defined(OSAL_CONFIG_USE_BARE_METAL) && (OSAL_CONFIG_USE_BARE_METAL == 1)
        .SVCallHandler = SVCallHandler,
        .PendSVHandler = PendSVHandler,
        .SysTickHandler = SysTickHandler,
    #else
        #error "Missing configuration for OSAL_CONFIG_USE_FREERTOS, OSAL_CONFIG_USE_BARE_METAL"
    #endif /* OSAL configuration */
    .DebugMonHandler = DebugMonHandler,
    .ReservedSection1 = 0,
    .ISR[WWDG_IRQn] = WWDG_IRQHandler,
    .ISR[PVD_PVM_IRQn] = PVD_PVM_IRQHandler,
    .ISR[TAMP_STAMP_IRQn] = TAMP_STAMP_IRQHandler,
    .ISR[RTC_WKUP_IRQn] = RTC_WKUP_IRQHandler,
    .ISR[FLASH_IRQn] = FLASH_IRQHandler,
    .ISR[RCC_IRQn] = RCC_IRQHandler,
    .ISR[EXTI0_IRQn] = EXTI0_IRQHandler,
    .ISR[EXTI1_IRQn] = EXTI1_IRQHandler,
    .ISR[EXTI2_IRQn] = EXTI2_IRQHandler,
    .ISR[EXTI3_IRQn] = EXTI3_IRQHandler,
    .ISR[EXTI4_IRQn] = EXTI4_IRQHandler,
    .ISR[DMA1_Channel1_IRQn] = DMA1_Channel1_IRQHandler,
    .ISR[DMA1_Channel2_IRQn] = DMA1_Channel2_IRQHandler,
    .ISR[DMA1_Channel3_IRQn] = DMA1_Channel3_IRQHandler,
    .ISR[DMA1_Channel4_IRQn] = DMA1_Channel4_IRQHandler,
    .ISR[DMA1_Channel5_IRQn] = DMA1_Channel5_IRQHandler,
    .ISR[DMA1_Channel6_IRQn] = DMA1_Channel6_IRQHandler,
    .ISR[DMA1_Channel7_IRQn] = DMA1_Channel7_IRQHandler,
    .ISR[ADC1_2_IRQn] = ADC1_2_IRQHandler,
    .ISR[CAN1_TX_IRQn] = CAN1_TX_IRQHandler,
    .ISR[CAN1_RX0_IRQn] = CAN1_RX0_IRQHandler,
    .ISR[CAN1_RX1_IRQn] = CAN1_RX1_IRQHandler,
    .ISR[CAN1_SCE_IRQn] = CAN1_SCE_IRQHandler,
    .ISR[EXTI9_5_IRQn] = EXTI9_5_IRQHandler,
    .ISR[TIM1_BRK_TIM15_IRQn] = TIM1_BRK_TIM15_IRQHandler,
    .ISR[TIM1_UP_TIM16_IRQn] = TIM1_UP_TIM16_IRQHandler,
    .ISR[TIM1_TRG_COM_TIM17_IRQn] = TIM1_TRG_COM_TIM17_IRQHandler,
    .ISR[TIM1_CC_IRQn] = TIM1_CC_IRQHandler,
    .ISR[TIM2_IRQn] = TIM2_IRQHandler,
    .ISR[TIM3_IRQn] = TIM3_IRQHandler,
    .ISR[TIM4_IRQn] = TIM4_IRQHandler,
    .ISR[I2C1_EV_IRQn] = I2C1_EV_IRQHandler,
    .ISR[I2C1_ER_IRQn] = I2C1_ER_IRQHandler,
    .ISR[I2C2_EV_IRQn] = I2C2_EV_IRQHandler,
    .ISR[I2C2_ER_IRQn] = I2C2_EV_IRQHandler,
    .ISR[SPI1_IRQn] = SPI1_IRQHandler,
    .ISR[SPI2_IRQn] = SPI2_IRQHandler,
    .ISR[USART1_IRQn] = USART1_IRQHandler,
    .ISR[USART2_IRQn] = USART2_IRQHandler,
    .ISR[USART3_IRQn] = USART3_IRQHandler,
    .ISR[EXTI15_10_IRQn] = EXTI15_10_IRQHandler,
    .ISR[RTC_Alarm_IRQn] = RTC_Alarm_IRQHandler,
    .ISR[DFSDM1_FLT3_IRQn] = DFSDM1_FLT3_IRQHandler,
    .ISR[TIM8_BRK_IRQn] = TIM8_BRK_IRQHandler,
    .ISR[TIM8_UP_IRQn] = TIM8_UP_IRQHandler,
    .ISR[TIM8_TRG_COM_IRQn] = TIM8_TRG_COM_IRQHandler,
    .ISR[TIM8_CC_IRQn] = TIM8_CC_IRQHandler,
    .ISR[ADC3_IRQn] = ADC3_IRQHandler,
    .ISR[FMC_IRQn] = FMC_IRQHandler,
    .ISR[SDMMC1_IRQn] = SDMMC1_IRQHandler,
    .ISR[TIM5_IRQn] = TIM5_IRQHandler,
    .ISR[SPI3_IRQn] = SPI3_IRQHandler,
    .ISR[UART4_IRQn] = UART4_IRQHandler,
    .ISR[UART5_IRQn] = UART5_IRQHandler,
    .ISR[TIM6_DAC_IRQn] = TIM6_DAC_IRQHandler,
    .ISR[TIM7_IRQn] = TIM7_IRQHandler,
    .ISR[DMA2_Channel1_IRQn] = DMA2_Channel1_IRQHandler,
    .ISR[DMA2_Channel2_IRQn] = DMA2_Channel2_IRQHandler,
    .ISR[DMA2_Channel3_IRQn] = DMA2_Channel3_IRQHandler,
    .ISR[DMA2_Channel4_IRQn] = DMA2_Channel4_IRQHandler,
    .ISR[DMA2_Channel5_IRQn] = DMA2_Channel5_IRQHandler,
    .ISR[DFSDM1_FLT0_IRQn] = DFSDM1_FLT0_IRQHandler,
    .ISR[DFSDM1_FLT1_IRQn] = DFSDM1_FLT1_IRQHandler,
    .ISR[DFSDM1_FLT2_IRQn] = DFSDM1_FLT2_IRQHandler,
    .ISR[COMP_IRQn] = COMP_IRQHandler,
    .ISR[LPTIM1_IRQn] = LPTIM1_IRQHandler,
    .ISR[LPTIM2_IRQn] = LPTIM2_IRQHandler,
    .ISR[OTG_FS_IRQn] = OTG_FS_IRQHandler,
    .ISR[DMA2_Channel6_IRQn] = DMA2_Channel6_IRQHandler,
    .ISR[DMA2_Channel7_IRQn] = DMA2_Channel7_IRQHandler,
    .ISR[LPUART1_IRQn] = LPUART1_IRQHandler,
    .ISR[QUADSPI_IRQn] = QUADSPI_IRQHandler,
    .ISR[I2C3_EV_IRQn] = I2C3_EV_IRQHandler,
    .ISR[I2C3_ER_IRQn] = I2C3_ER_IRQHandler,
    .ISR[SAI1_IRQn] = SAI1_IRQHandler,
    .ISR[SAI2_IRQn] = SAI2_IRQHandler,
    .ISR[SWPMI1_IRQn] = SWPMI1_IRQHandler,
    .ISR[TSC_IRQn] = TSC_IRQHandler,
    .ISR[LCD_IRQn] = LCD_IRQHandler,
    .ISR[79] = 0,
    .ISR[RNG_IRQn] = RNG_IRQHandler,
    .ISR[FPU_IRQn] = FPU_IRQHandler,
};

/**
 * @brief System reset handler. First code to execute upon reset.
 */
void ResetHandler(void)
{
    /* Initialize .data section */
    volatile U32* SrcPtr = &_data_loadaddr;
    volatile U32* DestPtr = &_data;
    for (; DestPtr < &_edata; SrcPtr++, DestPtr++)
    {
        *DestPtr = *SrcPtr;
    }

    /* Zero-fill .bss section */
    while (DestPtr < &_ebss)
    {
        *DestPtr++ = 0;
    }

    /* Ensure 8-byte stack alignment on exception entry */
    SCB->CCR |= SCB_CCR_STKALIGN_Msk;

    /* Enable floating point co-processor */
    SCB->CPACR |= (0xFU << 20U);

    (void)main();
}

/**
 * @brief Blocking placeholder exception/interrupt handler.
 */
void BlockingHandler(void)
{
    while (1);
}

/**
 * @brief Placeholder exception/interrupt handler that does nothing.
 */
void NullHandler(void)
{
    /* Do nothing */
}
