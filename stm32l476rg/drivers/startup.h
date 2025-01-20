/**
 * @file startup.h
 *
 * @brief Startup code for STM32L467RG microcontroller.
 */

#ifndef STARTUP_H
#define STARTUP_H

/* ------------------------------- Include directives ------------------------------ */

#include "typedef.h"
#include "stm32l476xx.h"

/* ------------------ Structures, enumerations & type definitions ------------------ */

/**
 * @brief Interrupt handler function pointer.
 */
typedef void (*TableEntryType)(void);

/**
 * @brief Interrupt vector table.
 */
typedef struct
{
    U32* StackTop;
    TableEntryType ResetHandler;
    TableEntryType NMIHandler;
    TableEntryType HardFaultHandler;
    TableEntryType MemManageHandler;
    TableEntryType BusFaultHandler;
    TableEntryType UsageFaultHandler;
    TableEntryType ReservedSection0[4];
    TableEntryType SVCallHandler;
    TableEntryType DebugMonHandler;
    TableEntryType ReservedSection1;
    TableEntryType PendSVHandler;
    TableEntryType SysTickHandler;
    TableEntryType ISR[82];
} VectorTableType;

/* Local function declarations */
void ResetHandler(void);
void BlockingHandler(void);
void NullHandler(void);

/* Forward declaration of main */
int main(void);

/* Weak declarations of Cortex-M4 system exception handlers */
void NMIHandler(void) __attribute__((weak, alias("NullHandler")));
void HardFaultHandler(void) __attribute__((weak, alias("BlockingHandler")));
void MemManageHandler(void) __attribute__((weak, alias("NullHandler")));
void BusFaultHandler(void) __attribute__((weak, alias("NullHandler")));
void UsageFaultHandler(void) __attribute__((weak, alias("NullHandler")));
void SVCallHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DebugMonHandler(void) __attribute__((weak, alias("BlockingHandler")));
void PendSVHandler(void) __attribute__((weak, alias("BlockingHandler")));
void SysTickHandler(void) __attribute__((weak, alias("NullHandler")));

/* Weak declarations of STM32L476RG interrupt handlers */
void WWDG_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void PVD_PVM_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TAMP_STAMP_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void RTC_WKUP_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void FLASH_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void RCC_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void EXTI2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void EXTI3_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void EXTI4_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA1_Channel1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA1_Channel2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA1_Channel3_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA1_Channel4_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA1_Channel5_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA1_Channel6_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA1_Channel7_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void ADC1_2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void CAN1_TX_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void CAN1_RX0_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void CAN1_RX1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void CAN1_SCE_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void EXTI9_5_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM1_BRK_TIM15_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM1_UP_TIM16_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM1_TRG_COM_TIM17_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM1_CC_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM4_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void I2C1_EV_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void I2C1_ER_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void I2C2_EV_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void I2C2_ER_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void SPI1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void SPI2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void USART1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void USART3_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void RTC_Alarm_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DFSDM1_FLT3_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM8_BRK_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM8_UP_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM8_TRG_COM_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM8_CC_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void ADC3_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void FMC_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void SDMMC1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM5_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void SPI3_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void UART4_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void UART5_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM6_DAC_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TIM7_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA2_Channel1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA2_Channel2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA2_Channel3_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA2_Channel4_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA2_Channel5_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DFSDM1_FLT0_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DFSDM1_FLT1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DFSDM1_FLT2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void COMP_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void LPTIM1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void LPTIM2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void OTG_FS_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA2_Channel6_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void DMA2_Channel7_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void LPUART1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void QUADSPI_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void I2C3_EV_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void I2C3_ER_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void SAI1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void SAI2_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void SWPMI1_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void TSC_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void LCD_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void RNG_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));
void FPU_IRQHandler(void) __attribute__((weak, alias("BlockingHandler")));

#endif  /* STARTUP_H */
