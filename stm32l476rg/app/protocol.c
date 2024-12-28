/**
 * @file protocol.c
 *
 * @brief Messaging protocol using UART.
 *
 */

/* ------------------------------- Include directives ------------------------------ */
#include "protocol.h"
#include "protocol_cfg.h"
#include "msg_handler.h"


/* --------------------------------- Local variables ------------------------------- */
static Bool ProtocolInitialized = False;
static Uart_HandleType UartHandle = NULL;
static Protocol_MessageType TxMsg = { 0 };
static Protocol_MessageType RxMsg = { 0 };

/* -------------------------- Private function definitions ------------------------- */

/**
 * @brief Recieve a message.
 * @param Message Pointer to message structure.
 * @return True = message was recieved, false = something went wrong.
 */
static Bool Protocol_RecieveMessage(Protocol_MessageType* Message)
{
    return Uart_Recieve(UartHandle, (U8*)Message, MSG_SIZE);
}

/**
 * @brief Transmit a response message.
 */
static void Protocol_TransmitMessage(const Protocol_MessageType* Message)
{
    Uart_Transmit(UartHandle, (U8*)Message, MSG_SIZE);
}

/* -------------------------- Public function definitions -------------------------- */

void Protocol_Init(USART_TypeDef* Uart, U32 BaudRate, Pin_PortPinEnum TxPin, Pin_PortPinEnum RxPin)
{
    if ( !ProtocolInitialized )
    {
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
        MsgHandler_HandleMessage(&RxMsg, &TxMsg);
        Protocol_TransmitMessage(&TxMsg);
        MsgHandlerBusy = False;
    }
}
