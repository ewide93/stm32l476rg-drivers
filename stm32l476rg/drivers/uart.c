/**
 * @file uart.c
 *
 * @brief UART peripheral driver module.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "uart.h"
#include "clock_control.h"
#include "pin.h"

/* -------------------------- Private function definitions ------------------------- */

/**
 * @brief Determine the alternate function mapping based on the
 *        given UART peripheral instance.
 * @param Uart Pointer to USART peripheral structure.
 * @return Alternate function mapping.
 */
static inline Pin_AlternateFunctionEnum Uart_InstanceToAltFunc(const USART_TypeDef* Uart)
{
    if (Uart == USART1 || Uart == USART2 || Uart == USART3)
    {
        return PIN_AF7;
    }
    else
    {
        return PIN_AF8;
    }
}


/**
 * @brief Set the word length for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param WordLength Desired word length.
 */
static inline void Uart_SetWordLength(USART_TypeDef* Uart, Uart_WordLengthEnum WordLength)
{
    Uart->CR1 &= ~USART_CR1_M;
    Uart->CR1 |= ( ((WordLength & 0x2U) << (USART_CR1_M1_Pos - 1U)) |
                   ((WordLength & 0x1U) << USART_CR1_M0_Pos) );
}


/**
 * @brief Set the oversampling mode of the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param Oversampling Desired oversampling mode.
 */
static inline void Uart_SetOversampling(USART_TypeDef* Uart, Uart_OversamplingEnum Oversampling)
{
    Uart->CR1 &= ~USART_CR1_OVER8;
    Uart->CR1 |= (Oversampling << USART_CR1_OVER8_Pos);
}


/**
 * @brief Set the parity for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param Parity Desired parity setting.
 */
static inline void Uart_SetParity(USART_TypeDef* Uart, Uart_ParityEnum Parity)
{
    Uart->CR1 &= ~USART_CR1_PS;
    if (Parity != UART_PARITY_NONE)
    {
        Uart->CR1 |= (Parity << USART_CR1_PS_Pos);
        Uart->CR1 |= USART_CR1_PCE;
    }
}


/**
 * @brief Set the bit sampling method for the UART reciever.
 * @param Uart Pointer to USART peripheral structure.
 * @param SampleMethod The desired sampling method.
 */
static inline void Uart_SetSamplingMethod(USART_TypeDef* Uart, Uart_SamplingMethodEnum SamplingMethod)
{
    Uart->CR3 &= ~USART_CR3_ONEBIT;
    Uart->CR3 |= (SamplingMethod << USART_CR3_ONEBIT_Pos);
}


/**
 * @brief Set the number of stop bits for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param StopBits The desired number of stop bits.
 */
static inline void Uart_SetStopBits(USART_TypeDef* Uart, Uart_StopBitsEnum StopBits)
{
    Uart->CR2 &= ~USART_CR2_STOP;
    Uart->CR2 |= (StopBits << USART_CR2_STOP_Pos);
}


/**
 * @brief Configure the baud rate for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param BaudRate The desired baud rate.
 * @param Oversampling The oversampling setting for the given UART peripheral.
 */
static inline void Uart_SetBaudRate(USART_TypeDef* Uart, U32 BaudRate, Uart_OversamplingEnum Oversampling)
{
    /**
     * @note For now it is assumed that the clock source to the USART peripheral clock domains
     *       remain at their reset values (PCLK1, PCLK2).
     */
    const U32 PclkFreq_Hz = (Uart == USART1) ? ClkCtrl_GetNodeFreq(CLK_NODE_PCLK2) : ClkCtrl_GetNodeFreq(CLK_NODE_PCLK1);
    const U32 ClkDiv = (Oversampling == UART_OVERSAMPLING_16) ? (U32)(PclkFreq_Hz / BaudRate) : (U32)((PclkFreq_Hz << 1) / BaudRate);
    if (ClkDiv <= UINT16_MAX) { Uart->BRR = (U16)ClkDiv; }
}


/**
 * @brief Await the transmission complete flag of the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 */
static inline void Uart_AwaitTxComplete(const USART_TypeDef* Uart)
{
    while ( !(Uart->ISR & USART_ISR_TC) ) { __NOP(); }
}

/* -------------------------- Public function definitions -------------------------- */

void Uart_Init(USART_TypeDef* Uart, const Uart_ConfigType* Config)
{
    const Pin_AlternateFunctionEnum AltFunc = Uart_InstanceToAltFunc(Uart);
    Pin_SetMode(Config->TxPin, PIN_MODE_AF);
    Pin_SetMode(Config->RxPin, PIN_MODE_AF);
    Pin_SetAltFunc(Config->TxPin, AltFunc);
    Pin_SetAltFunc(Config->RxPin, AltFunc);
    Uart_SetWordLength(Uart, Config->WordLength);
    Uart_SetParity(Uart, Config->Parity);
    Uart_SetStopBits(Uart, Config->StopBits);
    Uart_SetOversampling(Uart, Config->Oversampling);
    Uart_SetSamplingMethod(Uart, Config->SamplingMethod);
    Uart_SetBaudRate(Uart, Config->BaudRate, Config->Oversampling);
    /**
     * @todo Enable interrupts?
     */
}


void Uart_TransmitByte(USART_TypeDef* Uart, U8 Data)
{
    Uart_AwaitTxComplete(Uart);
    Uart->TDR = Data;
}


void Uart_TransmitStringBlocking(USART_TypeDef* Uart, Char* Data)
{
    for (U8* DataPtr = (U8*)Data; *DataPtr != '\0'; DataPtr++)
    {
        Uart_TransmitByte(Uart, *DataPtr);
        if (*DataPtr == '\n') { Uart_TransmitByte(Uart, '\r'); }
    }
}
