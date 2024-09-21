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
#include "fifo.h"

/* ---------------------------- Preprocessor directives ---------------------------- */
#define UART_IRQ_PRIO           (6U)
#define UART_TX_BUFFER_SIZE     (128U)
#define UART_RX_BUFFER_SIZE     (128U)

/**
 * @note In order to save RAM only USART peripherals for which these defines
 *       are set are usable.
 */
// #define USART1_ENABLE
#define USART2_ENABLE
// #define USART3_ENABLE
// #define UART4_ENABLE
// #define UART5_ENABLE
// #define LPUART1_ENABLE

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

/**
 * @brief Opaque UART handle type.
 */
typedef struct Uart_OpaqueHandleType* Uart_HandleType;

/* ------------------------- Public function definitions --------------------------- */

/**
 * @brief Enable the transmitter for the given UART peripheral.
 * @param Uart Uart peripheral handle.
 */
void Uart_TxEnable(Uart_HandleType Uart);


/**
 * @brief Disable the transmitter for the given UART peripheral.
 * @param Uart UART peripheral handle.
 */
void Uart_TxDisable(Uart_HandleType Uart);


/**
 * @brief Enable the reciever for the given UART peripheral.
 * @param Uart UART peripheral handle.
 */
void Uart_RxEnable(Uart_HandleType Uart);


/**
 * @brief Disable the reciever for the given UART peripheral.
 * @param Uart UART peripheral handle.
 */
void Uart_RxDisable(Uart_HandleType Uart);


/**
 * @brief Enable the given UART peripheral.
 * @param Uart UART peripheral handle.
 */
void Uart_Enable(Uart_HandleType Uart);

/**
 * @brief Disable the given UART peripheral.
 * @param Uart UART peripheral handle.
 */
void Uart_Disable(Uart_HandleType Uart);


/**
 * @brief Initialize the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param Config Pointer to configuration structure.
 * @return Handle to the initialized peripheral.
 */
Uart_HandleType Uart_Init(USART_TypeDef* Uart, const Uart_ConfigType* Config);


/**
 * @brief Transmit one byte of data from the given UART peripheral
 *        in a blocking fashion.
 * @param Uart UART peripheral handle.
 * @param Data The byte to be transmitted.
 */
void Uart_TransmitByteBlocking(Uart_HandleType Uart, U8 Data);


/**
 * @brief Transmit the given null-terminated string from
 *        the given UART peripheral in a blocking fashion.
 * @param Uart UART peripheral handle.
 * @param Data String to be transmitted
 */
void Uart_TransmitStringBlocking(Uart_HandleType Uart, const Char* Data);


/**
 * @brief Transmit the given string from the given UART peripheral
 *        in an interrupt-driven fashion.
 * @param Uart UART peripheral handle.
 * @param Data Data to be transmitted.
 * @param Length Number of characters to transmit.
 * @return Boolean indicating if the data was successfully
 *         added to the transmission queue.
 */
Bool Uart_TransmitString(Uart_HandleType Uart, const Char* Data, U8 Length);

#endif /* UART_H */