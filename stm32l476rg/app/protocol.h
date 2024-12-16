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
#include "cmsis_compiler.h"
#include "uart.h"
#include "pin.h"
#include "crc.h"

/*  --------------------------- Preprocessor definitions --------------------------- */
#define MSG_ID_SIZE         (1U)
#define MSG_PAYLOAD_SIZE    (8U)
#define MSG_CRC_SIZE        (1U)
#define MSG_SIZE            (MSG_ID_SIZE + MSG_PAYLOAD_SIZE + MSG_CRC_SIZE)

/**
 * @brief Response IDs.
 */
#define ACK_RESPONSE        (0x00U)
#define NACK_RESPONSE       (0x01U)
#define CRC_ERROR_RESPONSE  (0x02U)

/*  ----------------- Structures, enumerations & type definitions ------------------ */

/**
 * @brief Protocol message structure.
 */
typedef __PACKED_STRUCT
{
    U8 Id;
    U8 Payload[MSG_PAYLOAD_SIZE];
    U8 Crc;
} Protocol_MessageType;

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