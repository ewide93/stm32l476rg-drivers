/**
 * @file msg_handler.c
 *
 * @brief Message handling for UART protocol.
 *
 */

/* ------------------------------- Include directives ------------------------------ */
#include "msg_handler.h"
#include "crc.h"
#include "systick.h"

/*  ----------------- Structures, enumerations & type definitions ------------------ */

/**
 * @brief Message handling function pointer typedef.
 */
typedef void (*MessageHandler)(const Protocol_MessageType*, Protocol_MessageType*);

/* --------------------------- Private function prototypes ------------------------- */

/**
 * @brief Dummy message handler.
 */
void DummyMessageHandler(const Protocol_MessageType* RxMsg, Protocol_MessageType* TxMsg);

/**
 * @brief Calculate the CRC-8 for the given message.
 * @param Message Pointer to message structure.
 * @return CRC-8 digest.
 */
U8 MsgHandler_CalcCrc(const Protocol_MessageType* Message);

/**
 * @brief Construct a CRC error response message.
 * @param Message Pointer to message structure.
 */
void MsgHandler_ConstructCrcErrorResponse(Protocol_MessageType* TxMsg);

/**
 * @brief Construct an invalid message ID error response message.
 * @param Message Pointer to message structure.
 */
void MsgHandler_ConstructMsgIdErrorResponse(Protocol_MessageType* TxMsg);

/**
 * @brief Handler for message with ID: 0x00. Get the ms tick counter.
 */
void MsgHandler_0x00(const Protocol_MessageType* RxMsg, Protocol_MessageType* TxMsg);

/* --------------------------------- Local variables ------------------------------- */

/**
 * @brief Message handling function table.
 */
static const MessageHandler MsgHandlerTable[] =
{
    MsgHandler_0x00,
};
static const U8 NofMsgHandlers = (U8)(sizeof(MsgHandlerTable) / sizeof(MsgHandlerTable[0]));

/* --------------------------- Private function definitions ------------------------ */

void DummyMessageHandler(const Protocol_MessageType* RxMsg, Protocol_MessageType* TxMsg)
{
    (void)RxMsg;
    (void)TxMsg;
}

U8 MsgHandler_CalcCrc(const Protocol_MessageType* Message)
{
    return Crc_CalcCrc8((const U8*)Message, (MSG_ID_SIZE + MSG_PAYLOAD_SIZE));
}

void MsgHandler_ConstructCrcErrorResponse(Protocol_MessageType* TxMsg)
{
    TxMsg->Id = CRC_ERROR_RESPONSE;
    for (U8 i = 0; i < MSG_PAYLOAD_SIZE; i++)
    {
        TxMsg->Payload[i] = 0;
    }
    TxMsg->Crc = MsgHandler_CalcCrc(TxMsg);
}

void MsgHandler_ConstructMsgIdErrorResponse(Protocol_MessageType* TxMsg)
{
    TxMsg->Id = INVALID_ID_RESPONSE;
    for (U8 i = 0; i < MSG_PAYLOAD_SIZE; i++)
    {
        TxMsg->Payload[i] = 0;
    }
    TxMsg->Crc = MsgHandler_CalcCrc(TxMsg);
}

/* ------------------------ Message handler function definitions -------------------- */

void MsgHandler_0x00(const Protocol_MessageType* RxMsg, Protocol_MessageType* TxMsg)
{
    /* RxMsg parameter unused */
    (void)RxMsg;

    TxMsg->Id = ACK_RESPONSE;
    *((U32*)(&TxMsg->Payload[0])) = SysTick_GetTicks();
    *((U32*)(&TxMsg->Payload[4])) = SysTick_GetNofWrapArounds();
    TxMsg->Crc = MsgHandler_CalcCrc(TxMsg);
}


/* -------------------------- Public function definitions -------------------------- */

void MsgHandler_HandleMessage(const Protocol_MessageType* RxMsg, Protocol_MessageType* TxMsg)
{
    const Bool CrcOk = (MsgHandler_CalcCrc(RxMsg) == RxMsg->Crc);
    const Bool IdOk = (RxMsg->Id < NofMsgHandlers);

    if (!CrcOk) { MsgHandler_ConstructCrcErrorResponse(TxMsg); }
    if (!IdOk) { MsgHandler_ConstructMsgIdErrorResponse(TxMsg); }

    if (CrcOk && IdOk)
    {
        MsgHandlerTable[RxMsg->Id](RxMsg, TxMsg);
    }

}
