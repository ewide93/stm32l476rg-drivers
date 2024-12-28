/**
 * @file protocol_cfg.h
 *
 * @brief Common preprocessor definitions and type definitions for protocol
 *        related stuff.
 */

#ifndef PROTOCOL_CFG_H
#define PROTOCOL_CFG_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"
#include "cmsis_compiler.h"

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
#define INVALID_ID_RESPONSE (0x04U)

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
StaticAssert(sizeof(Protocol_MessageType) == 10U, "Unwanted padding Protocol_MessageType");

#endif /* PROTOCOL_CFG_H */
