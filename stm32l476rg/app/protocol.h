/**
 * @file protocol.h
 *
 * @brief Messaging protocol using UART.
 *
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"
#include "uart.h"
#include "pin.h"

/* -------------------------- Public function prototypes --------------------------- */

/**
 * @brief Initialize the protocol handler.
 * @param Uart Pointer to USART peripheral structure.
 * @param BaudRate Desired baud rate for communication.
 */
void Protocol_Init(USART_TypeDef* Uart, U32 BaudRate, Pin_PortPinEnum TxPin, Pin_PortPinEnum RxPin);

/**
 * @brief Execute the protocol handler.
 * @note Should be called periodically.
 */
void Protocol_Run(void);

#endif /* PROTOCOL_H */