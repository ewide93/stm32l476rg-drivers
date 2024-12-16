/**
 * @file protocol.c
 *
 * @brief Messaging protocol using UART.
 *
 */

/* ------------------------------- Include directives ------------------------------ */
#include "protocol.h"

/* --------------------------------- Local variables ------------------------------- */
static Bool ProtocolInitialized = False;
static Uart_HandleType UartHandle = NULL;
static Protocol_MessageType TxMsg = { 0 };
static Protocol_MessageType RxMsg = { 0 };

/* -------------------------- Private function definitions ------------------------- */

/**
 * @brief Calculate the CRC-8 for the given message.
 * @param Message Pointer to message structure.
 * @return CRC-8 digest.
 */
U8 Protocol_CalcMessageCrc(const Protocol_MessageType* Message)
{
    return Crc_CalcCrc8((const U8*)Message, (MSG_ID_SIZE + MSG_PAYLOAD_SIZE));
}

/**
 * @brief Recieve a message.
 * @param Message Pointer to message structure.
 * @return True = message was recieved, false = something went wrong.
 */
Bool Protocol_RecieveMessage(Protocol_MessageType* Message)
{
    return Uart_Recieve(UartHandle, (U8*)Message, MSG_SIZE);
}

/**
 * @brief Construct a CRC error response message.
 * @param Message Pointer to message structure.
 */
void Protocol_ConstructCrcErrorResponse(void)
{
    TxMsg.Id = CRC_ERROR_RESPONSE;
    for (U8 i = 0; i < MSG_PAYLOAD_SIZE; i++)
    {
        TxMsg.Payload[i] = 0;
    }
    TxMsg.Crc = Protocol_CalcMessageCrc(&TxMsg);
}

/**
 * @brief Construct an ACK or NACK response message.
 * @param Message Pointer to message structure.
 */
void Protocol_ConstructMessageResponse(Bool SendAck)
{
    TxMsg.Id = (SendAck == True) ? (ACK_RESPONSE) : (NACK_RESPONSE);
    TxMsg.Crc = Protocol_CalcMessageCrc(&TxMsg);
}

/**
 * @brief Handle the recived message.
 * @return True = all is fine, send ACK,
 *         False = Ivalid ID or malformed data, send NACK.
 */
Bool Protocol_HandleMessage(void)
{
    Bool Rv = False;
    for (U8 i = 0; i < MSG_PAYLOAD_SIZE; i++)
    {
        TxMsg.Payload[i] = RxMsg.Payload[i];
    }
    Rv = True;
    return Rv;
}

/**
 * @brief Transmit a response message.
 */
void Protocol_TransmitResponse(void)
{
    Uart_Transmit(UartHandle, &TxMsg.Id, MSG_ID_SIZE);
    Uart_Transmit(UartHandle, TxMsg.Payload, MSG_PAYLOAD_SIZE);
    Uart_Transmit(UartHandle, &TxMsg.Crc, MSG_CRC_SIZE);

}

/* -------------------------- Public function definitions -------------------------- */

void Protocol_Init(USART_TypeDef* Uart, U32 BaudRate, Pin_PortPinEnum TxPin, Pin_PortPinEnum RxPin)
{
    if ( !ProtocolInitialized )
    {
        TxMsg.Id = 0;
        RxMsg.Id = 0;
        for (U8 i = 0; i < MSG_PAYLOAD_SIZE; i++)
        {
            TxMsg.Payload[i] = 0;
            RxMsg.Payload[i] = 0;
        }
        TxMsg.Crc = 0;
        RxMsg.Crc = 0;

        Uart_ConfigType UartCfg =
        {
            .BaudRate = BaudRate,
            .Oversampling = UART_OVERSAMPLING_16,
            .SamplingMethod = UART_SAMPLING_3_BITS,
            .Parity = UART_PARITY_NONE,
            .WordLength = UART_WORD_LEN_8,
            .StopBits = UART_STOP_BITS_1,
            .RxPin = RxPin,
            .TxPin = TxPin
        };
        UartHandle = Uart_Init(Uart, &UartCfg);
        Uart_TxEnable(UartHandle);
        Uart_RxEnable(UartHandle);
        Uart_Enable(UartHandle);
        ProtocolInitialized = True;
    }
}

void Protocol_Run(void)
{
    static Bool MsgHandlerBusy = False;
    if ( Uart_GetNofInputBufferBytes(UartHandle) >= MSG_SIZE && !MsgHandlerBusy )
    {
        MsgHandlerBusy = True;
        Protocol_RecieveMessage(&RxMsg);
        const Bool CrcOk = (Protocol_CalcMessageCrc(&RxMsg) == RxMsg.Crc);
        if ( CrcOk )
        {
            const Bool SendAck = Protocol_HandleMessage();
            Protocol_ConstructMessageResponse(SendAck);
        }
        else
        {
            Protocol_ConstructCrcErrorResponse();
        }
        Protocol_TransmitResponse();
        MsgHandlerBusy = False;
    }
}