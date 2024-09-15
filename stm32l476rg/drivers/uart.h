/**
 * @file uart.h
 *
 * @brief UART peripheral driver module.
 */

#ifndef UART_H
#define UART_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"
#include "pin.h"

/*  -------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of the available word lengths in bits.
 */
typedef enum
{
    UART_WORD_LEN_8 = 0x0U,
    UART_WORD_LEN_9 = 0x1U,
    UART_WORD_LEN_7 = 0x2U,
} Uart_WordLengthEnum;

/**
 * @brief Enumeration of the available oversampling modes.
 */
typedef enum
{
    UART_OVERSAMPLING_16 = 0x0U,
    UART_OVERSAMPLING_8 = 0x1U
} Uart_OversamplingEnum;

/**
 * @brief Enumeration of the available settings for parity generation/detection.
 */
typedef enum
{
    UART_PARITY_EVEN = 0x0U,
    UART_PARITY_ODD = 0x1U,
    UART_PARITY_NONE = 0x2U,
} Uart_ParityEnum;

/**
 * Enumeration of the available settings for number of stop bits.
 */
typedef enum
{
    UART_STOP_BITS_1 = 0x0U,        /* 1 stop bit    */
    UART_STOP_BITS_HALF = 0x1U,     /* 0.5 stop bits */
    UART_STOP_BITS_2 = 0x2U,        /* 2 stop bits   */
    UART_STOP_BITS_1_HALF = 0x3U,   /* 1.5 stop bits */
} Uart_StopBitsEnum;

/**
 * @brief Enumeration of the available settings for data bit sampling.
 */
typedef enum
{
    UART_SAMPLING_3_BITS = 0x0U,    /* Majority vote of 3 bit samples */
    UART_SAMPLING_1_BIT = 0x1U      /* Sampling of a single bit       */
} Uart_SamplingMethodEnum;

/**
 * @brief UART peripheral configuration structure.
 */
typedef struct
{
    U32 BaudRate;
    Pin_PortPinEnum RxPin;
    Pin_PortPinEnum TxPin;
    Uart_WordLengthEnum WordLength;
    Uart_OversamplingEnum Oversampling;
    Uart_SamplingMethodEnum SamplingMethod;
    Uart_ParityEnum Parity;
    Uart_StopBitsEnum StopBits;
} Uart_ConfigType;

/* ------------------------- Public function declarations -------------------------- */

/* ------------------------- Public function definitions --------------------------- */

/**
 * @brief Enable the transmitter for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 */
static inline void Uart_TxEnable(USART_TypeDef* Uart)
{
    Uart->CR1 |= USART_CR1_TE;
}


/**
 * @brief Disable the transmitter for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 */
static inline void Uart_TxDisable(USART_TypeDef* Uart)
{
    Uart->CR1 &= ~USART_CR1_TE;
}


/**
 * @brief Enable the reciever for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 */
static inline void Uart_RxEnable(USART_TypeDef* Uart)
{
    Uart->CR1 |= USART_CR1_RE;
}


/**
 * @brief Disable the reciever for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 */
static inline void Uart_RxDisable(USART_TypeDef* Uart)
{
    Uart->CR1 &= ~USART_CR1_RE;
}


/**
 * @brief Enable the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 */
static inline void Uart_Enable(USART_TypeDef* Uart)
{
    Uart->CR1 |= USART_CR1_UE;
}


/**
 * @brief Disable the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 */
static inline void Uart_Disable(USART_TypeDef* Uart)
{
    Uart->CR1 &= ~USART_CR1_UE;
}


/**
 * @brief Initialize the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param Config Pointer to configuration structure.
 */
void Uart_Init(USART_TypeDef* Uart, const Uart_ConfigType* Config);


/**
 * @brief Transmit one byte of data from the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param Data The byte to be transmitted.
 */
void Uart_TransmitByte(USART_TypeDef* Uart, U8 Data);


/**
 * @brief Transmit the given null-terminated string from
 *        the given UART peripheral in a blocking fashion.
 * @param Uart Pointer to USART peripheral structure.
 * @param Data String to be transmitted
 */
void Uart_TransmitStringBlocking(USART_TypeDef* Uart, Char* Data);

#endif /* UART_H */