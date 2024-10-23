/**
 * @file protocol.h
 *
 * @brief Messaging protocol using UART.
 *
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* ------------------------------- Include directives ------------------------------ */
#include "uart.h"

/*  --------------------------- Preprocessor definitions --------------------------- */
#define MSG_ID_SIZE         (1U)
#define MSG_PAYLOAD_SIZE    (8U)
#define MSG_CHECKSUM_SIZE   (1U)
#define MSG_SIZE            (MSG_ID_SIZE + MSG_PAYLOAD_SIZE + MSG_CHECKSUM_SIZE)

#endif /* PROTOCOL_H */