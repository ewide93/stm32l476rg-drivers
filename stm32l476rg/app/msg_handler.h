/**
 * @file msg_handler.h
 *
 * @brief Message handling for UART protocol.
 *
 */

#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"
#include "protocol_cfg.h"

/* -------------------------- Public function prototypes --------------------------- */

/**
 * @brief Handle the given message & assemble the given response message.
 * @param RxMsg Recived message to be handled.
 * @param TxMsg Response message to be assembled.
 */
void MsgHandler_HandleMessage(const Protocol_MessageType* RxMsg, Protocol_MessageType* TxMsg);

#endif /* MSG_HANDLER_H */
