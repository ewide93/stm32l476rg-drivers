/**
 * @file uart.c
 *
 * @brief UART peripheral driver module.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "uart.h"
#include "clock_control.h"
#include "pin.h"
#include "fifo.h"
#include "critical_section.h"

/* ------------------------- Local preprocessor definitions ------------------------ */
#define INVALID_IRQn    ((IRQn_Type)0xFFU)

/*  -------------------------- Structures & enumerations --------------------------- */

struct Uart_OpaqueHandleType
{
    USART_TypeDef* Instance;
    FifoType* TxFifo;
    U8* TxBuffer;
    FifoType* RxFifo;
    U8* RxBuffer;
    Bool TxBusy;
    Bool RxBusy;
};

/* --------------------------------- Local variables ------------------------------- */

#if defined(USART1_ENABLE)
static U8 Usart1TxBuffer[UART_TX_BUFFER_SIZE] = { 0 };
static U8 Usart1RxBuffer[UART_RX_BUFFER_SIZE] = { 0 };
static FifoType Usart1TxFifo = { 0 };
static FifoType Usart1RxFifo = { 0 };
static struct Uart_OpaqueHandleType Usart1Handle =
{
    .Instance = USART1,
    .TxFifo = &Usart1TxFifo,
    .TxBuffer = Usart1TxBuffer,
    .TxBusy = False,
    .RxFifo = &Usart1RxFifo,
    .RxBuffer = Usart1RxBuffer,
    .RxBusy = False,
};
#endif /* USART1_ENABLE */

#if defined(USART2_ENABLE)
static U8 Usart2TxBuffer[UART_TX_BUFFER_SIZE] = { 0 };
static U8 Usart2RxBuffer[UART_RX_BUFFER_SIZE] = { 0 };
static FifoType Usart2TxFifo = { 0 };
static FifoType Usart2RxFifo = { 0 };
static struct Uart_OpaqueHandleType Usart2Handle =
{
    .Instance = USART2,
    .TxFifo = &Usart2TxFifo,
    .TxBuffer = Usart2TxBuffer,
    .TxBusy = False,
    .RxFifo = &Usart2RxFifo,
    .RxBuffer = Usart2RxBuffer,
    .RxBusy = False,
};
#endif /* USART2_ENABLE */

#if defined(USART3_ENABLE)
static U8 Usart3TxBuffer[UART_TX_BUFFER_SIZE] = { 0 };
static U8 Usart3RxBuffer[UART_RX_BUFFER_SIZE] = { 0 };
static FifoType Usart3TxFifo = { 0 };
static FifoType Usart3RxFifo = { 0 };
static struct Uart_OpaqueHandleType Usart3Handle =
{
    .Instance = USART3,
    .TxFifo = &Usart3TxFifo,
    .TxBuffer = Usart3TxBuffer,
    .TxBusy = False,
    .RxFifo = &Usart3RxFifo,
    .RxBuffer = Usart3RxBuffer,
    .RxBusy = False,
};
#endif /* USART3_ENABLE */

#if defined(UART4_ENABLE)
static U8 Uart4TxBuffer[UART_TX_BUFFER_SIZE] = { 0 };
static U8 Uart4RxBuffer[UART_RX_BUFFER_SIZE] = { 0 };
static FifoType Uart4TxFifo = { 0 };
static FifoType Uart4RxFifo = { 0 };
static struct Uart_OpaqueHandleType Uart4Handle =
{
    .Instance = UART4,
    .TxFifo = &Uart4TxFifo,
    .TxBuffer = Uart4TxBuffer,
    .TxBusy = False,
    .RxFifo = &Uart4RxFifo,
    .RxBuffer = Uart4RxBuffer,
    .RxBusy = False,
};
#endif /* UART4_ENABLE */

#if defined(UART5_ENABLE)
static U8 Uart5TxBuffer[UART_TX_BUFFER_SIZE] = { 0 };
static U8 Uart5RxBuffer[UART_RX_BUFFER_SIZE] = { 0 };
static FifoType Uart5TxFifo = { 0 };
static FifoType Uart5RxFifo = { 0 };
static struct Uart_OpaqueHandleType Uart5Handle =
{
    .Instance = UART5,
    .TxFifo = &Uart5TxFifo,
    .TxBuffer = Uart5TxBuffer,
    .TxBusy = False,
    .RxFifo = &Uart5RxFifo,
    .RxBuffer = Uart5RxBuffer,
    .RxBusy = False,
};
#endif /* UART5_ENABLE */

#if defined(LPUART1_ENABLE)
static U8 Lpuart1TxBuffer[UART_TX_BUFFER_SIZE] = { 0 };
static U8 Lpuart1RxBuffer[UART_RX_BUFFER_SIZE] = { 0 };
static FifoType Lpuart1TxFifo = { 0 };
static FifoType Lpuart1RxFifo = { 0 };
static struct Uart_OpaqueHandleType Lpuart1Handle =
{
    .Instance = LPUART1,
    .TxFifo = &Lpuart1TxFifo,
    .TxBuffer = Lpuart1TxBuffer,
    .TxBusy = False,
    .RxFifo = &Lpuart1RxFifo,
    .RxBuffer = Lpuart1RxBuffer,
    .RxBusy = False,
};
#endif /* LPUART1_ENABLE */


/* -------------------------- Private function definitions ------------------------- */

/**
 * @brief Determine the alternate function mapping based on the
 *        given UART peripheral instance.
 * @param Uart UART peripheral handle.
 * @return Alternate function mapping.
 */
static inline Pin_AlternateFunctionEnum Uart_InstanceToAltFunc(const USART_TypeDef* Uart)
{
    if (Uart == USART1 || Uart == USART2 || Uart == USART3)
    {
        return PIN_AF7;
    }
    else
    {
        return PIN_AF8;
    }
}


/**
 * @brief Determine the IRQ number matching the given UART
 *        peripheral instance.
 * @param Uart Pointer to USART peripheral structure.
 * @return Matching IRQ number.
 */
static IRQn_Type Uart_InstanceToIrqNum(const USART_TypeDef* Uart)
{
    if      (Uart == USART1)  { return USART1_IRQn;  }
    else if (Uart == USART2)  { return USART2_IRQn;  }
    else if (Uart == USART3)  { return USART3_IRQn;  }
    else if (Uart == UART4)   { return UART4_IRQn;   }
    else if (Uart == UART5)   { return UART5_IRQn;   }
    else if (Uart == LPUART1) { return LPUART1_IRQn; }
    else                      { return INVALID_IRQn; }
}


/**
 * @brief Determine the local handle to use for the given
 *        USART peripheral instance.
 * @param Uart Pointer to USART peripheral structure.
 * @return Matching UART handle.
 */
static Uart_HandleType Uart_InstanceToHandle(const USART_TypeDef* Uart)
{
    #if defined(USART1_ENABLE)
    if (Uart == USART1) { return &Usart1Handle; }
    #endif /* USART1_ENABLE */

    #if defined(USART2_ENABLE)
    if (Uart == USART2) { return &Usart2Handle; }
    #endif /* USART2_ENABLE */

    #if defined(USART3_ENABLE)
    if (Uart == USART3) { return &Usart3Handle; }
    #endif /* USART#_ENABLE */

    #if defined(UART4_ENABLE)
    if (Uart == UART4) { return &Uart4Handle; }
    #endif /* UART4_ENABLE */

    #if defined(UART5_ENABLE)
    if (Uart == UART5) { return &Uart5Handle; }
    #endif /* UART5_ENABLE */

    #if defined(LPUART1_ENABLE)
    if (Uart == LPUART1) { return &Lpuart1Handle; }
    #endif /* LPUART1_ENABLE */

    /* Returns NULL if the desired peripheral is not enabled by preprocessor define */
    return (struct Uart_OpaqueHandleType*)NULL;
}


/**
 * @brief Set the word length for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param WordLength Desired word length.
 */
static inline void Uart_SetWordLength(USART_TypeDef* Uart, Uart_WordLengthEnum WordLength)
{
    Uart->CR1 &= ~USART_CR1_M;
    Uart->CR1 |= ( ((WordLength & 0x2U) << (USART_CR1_M1_Pos - 1U)) |
                   ((WordLength & 0x1U) << USART_CR1_M0_Pos) );
}


/**
 * @brief Set the oversampling mode of the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param Oversampling Desired oversampling mode.
 */
static inline void Uart_SetOversampling(USART_TypeDef* Uart, Uart_OversamplingEnum Oversampling)
{
    Uart->CR1 &= ~USART_CR1_OVER8;
    Uart->CR1 |= (Oversampling << USART_CR1_OVER8_Pos);
}


/**
 * @brief Set the parity for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param Parity Desired parity setting.
 */
static inline void Uart_SetParity(USART_TypeDef* Uart, Uart_ParityEnum Parity)
{
    Uart->CR1 &= ~USART_CR1_PS;
    if (Parity != UART_PARITY_NONE)
    {
        Uart->CR1 |= (Parity << USART_CR1_PS_Pos);
        Uart->CR1 |= USART_CR1_PCE;
    }
}


/**
 * @brief Set the bit sampling method for the UART reciever.
 * @param Uart Pointer to USART peripheral structure.
 * @param SampleMethod The desired sampling method.
 */
static inline void Uart_SetSamplingMethod(USART_TypeDef* Uart, Uart_SamplingMethodEnum SamplingMethod)
{
    Uart->CR3 &= ~USART_CR3_ONEBIT;
    Uart->CR3 |= (SamplingMethod << USART_CR3_ONEBIT_Pos);
}


/**
 * @brief Set the number of stop bits for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param StopBits The desired number of stop bits.
 */
static inline void Uart_SetStopBits(USART_TypeDef* Uart, Uart_StopBitsEnum StopBits)
{
    Uart->CR2 &= ~USART_CR2_STOP;
    Uart->CR2 |= (StopBits << USART_CR2_STOP_Pos);
}


/**
 * @brief Configure the baud rate for the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 * @param BaudRate The desired baud rate.
 * @param Oversampling The oversampling setting for the given UART peripheral.
 */
static inline void Uart_SetBaudRate(USART_TypeDef* Uart, U32 BaudRate, Uart_OversamplingEnum Oversampling)
{
    /**
     * @note For now it is assumed that the clock source to the USART peripheral clock domains
     *       remain at their reset values (PCLK1, PCLK2).
     */
    const U32 PclkFreq_Hz = (Uart == USART1) ? ClkCtrl_GetNodeFreq(CLK_NODE_PCLK2) : ClkCtrl_GetNodeFreq(CLK_NODE_PCLK1);
    const U32 ClkDiv = (Oversampling == UART_OVERSAMPLING_16) ? (U32)(PclkFreq_Hz / BaudRate) : (U32)((PclkFreq_Hz << 1) / BaudRate);
    if (ClkDiv <= UINT16_MAX) { Uart->BRR = (U16)ClkDiv; }
}


/**
 * @brief Await the transmission complete flag of the given UART peripheral.
 * @param Uart Pointer to USART peripheral structure.
 */
static inline void Uart_AwaitTxComplete(const USART_TypeDef* Uart)
{
    while ( !(Uart->ISR & USART_ISR_TC) ) { __NOP(); }
}


/**
 * @brief Enable the transmit data register empty interrupt
 *        for the given UART peripheral.
 */

/* -------------------------- Public function definitions -------------------------- */

void Uart_TxEnable(Uart_HandleType Uart)
{
    Uart->Instance->CR1 |= USART_CR1_TE;
}


void Uart_TxDisable(Uart_HandleType Uart)
{
    Uart->Instance->CR1 &= ~USART_CR1_TE;
}


void Uart_RxEnable(Uart_HandleType Uart)
{
    Uart->Instance->CR1 |= USART_CR1_RE;
}


void Uart_RxDisable(Uart_HandleType Uart)
{
    Uart->Instance->CR1 &= ~USART_CR1_RE;
}


void Uart_Enable(Uart_HandleType Uart)
{
    Uart->Instance->CR1 |= USART_CR1_UE;
}


void Uart_Disable(Uart_HandleType Uart)
{
    Uart->Instance->CR1 &= ~USART_CR1_UE;
}


Uart_HandleType Uart_Init(USART_TypeDef* Uart, const Uart_ConfigType* Config)
{
    const Pin_AlternateFunctionEnum AltFunc = Uart_InstanceToAltFunc(Uart);
    Pin_SetMode(Config->TxPin, PIN_MODE_AF);
    Pin_SetMode(Config->RxPin, PIN_MODE_AF);
    Pin_SetAltFunc(Config->TxPin, AltFunc);
    Pin_SetAltFunc(Config->RxPin, AltFunc);
    Uart_SetWordLength(Uart, Config->WordLength);
    Uart_SetParity(Uart, Config->Parity);
    Uart_SetStopBits(Uart, Config->StopBits);
    Uart_SetOversampling(Uart, Config->Oversampling);
    Uart_SetSamplingMethod(Uart, Config->SamplingMethod);
    Uart_SetBaudRate(Uart, Config->BaudRate, Config->Oversampling);

    /* Initialize Tx & Rx buffers */
    struct Uart_OpaqueHandleType* const TempHandle = Uart_InstanceToHandle(Uart);
    if (TempHandle == NULL) { return TempHandle; }
    Fifo_Init(TempHandle->TxFifo, TempHandle->TxBuffer, UART_TX_BUFFER_SIZE);
    Fifo_Init(TempHandle->RxFifo, TempHandle->RxBuffer, UART_RX_BUFFER_SIZE);

    /* UART peripheral interrupt configuration */
    Uart->CR1 |= USART_CR1_RXNEIE;

    /* NVIC configuration */
    const IRQn_Type Irq = Uart_InstanceToIrqNum(Uart);
    NVIC_SetPriority(Irq, UART_IRQ_PRIO);
    NVIC_EnableIRQ(Irq);
    return TempHandle;
}


void Uart_TransmitByteBlocking(Uart_HandleType Uart, U8 Data)
{
    Uart_AwaitTxComplete(Uart->Instance);
    Uart->Instance->TDR = Data;
}


void Uart_TransmitStringBlocking(Uart_HandleType Uart, const Char* Data)
{
    for (U8* DataPtr = (U8*)Data; *DataPtr != '\0'; DataPtr++)
    {
        Uart_TransmitByteBlocking(Uart, *DataPtr);
        if (*DataPtr == '\n') { Uart_TransmitByteBlocking(Uart, '\r'); }
    }
}


Bool Uart_TransmitChar(Uart_HandleType Uart, Char Data)
{
    if (Fifo_Full(Uart->TxFifo)) { return False; }

    CRITICAL_SECTION_ENTER;
    Fifo_WriteByte(Uart->TxFifo, (U8)Data);
    CRITICAL_SECTION_EXIT;

    if (!Uart->TxBusy)
    {
        U8 TxData;
        CRITICAL_SECTION_ENTER;
        Fifo_ReadByte(Uart->TxFifo, &TxData);
        CRITICAL_SECTION_EXIT;
        Uart->Instance->CR1 |= USART_CR1_TXEIE;
        Uart->Instance->TDR = TxData;
        Uart->TxBusy = True;
    }
    return True;
}


Bool Uart_TransmitString(Uart_HandleType Uart, const Char* Data, U8 Length)
{
    if (Length > Fifo_GetNofAvailable(Uart->TxFifo)) { return False; }

    for (U8 i = 0; i < Length; i++)
    {
        CRITICAL_SECTION_ENTER;
        Fifo_WriteByte(Uart->TxFifo, (U8)Data[i]);
        CRITICAL_SECTION_EXIT;
    }

    if (!Uart->TxBusy)
    {
        U8 TxData;
        CRITICAL_SECTION_ENTER;
        Fifo_ReadByte(Uart->TxFifo, &TxData);
        CRITICAL_SECTION_EXIT;
        Uart->Instance->CR1 |= USART_CR1_TXEIE;
        Uart->Instance->TDR = TxData;
        Uart->TxBusy = True;
    }
    return True;
}


Bool Uart_RecieveChar(Uart_HandleType Uart, Char* RxData)
{
    if (Fifo_Empty(Uart->RxFifo)) { return False; }

    CRITICAL_SECTION_ENTER;
    Fifo_ReadByte(Uart->RxFifo, (U8*)RxData);
    CRITICAL_SECTION_EXIT;
    return True;
}


Bool Uart_Recieve(Uart_HandleType Uart, U8* RxData, U8 Length)
{
    if (Fifo_GetNofItems(Uart->RxFifo) < Length) { return False; }

    for (U8 i = 0; i < Length; i++)
    {
        CRITICAL_SECTION_ENTER;
        Fifo_ReadByte(Uart->RxFifo, &RxData[i]);
        CRITICAL_SECTION_EXIT;
    }
    return True;
}

Bool Uart_Transmit(Uart_HandleType Uart, const U8* Data, U8 Length)
{
    if (Length > Fifo_GetNofAvailable(Uart->TxFifo)) { return False; }

    for (U8 i = 0; i < Length; i++)
    {
        CRITICAL_SECTION_ENTER;
        Fifo_WriteByte(Uart->TxFifo, Data[i]);
        CRITICAL_SECTION_EXIT;
    }

    if (!Uart->TxBusy)
    {
        U8 TxData;
        CRITICAL_SECTION_ENTER;
        Fifo_ReadByte(Uart->TxFifo, &TxData);
        CRITICAL_SECTION_EXIT;
        Uart->Instance->CR1 |= USART_CR1_TXEIE;
        Uart->Instance->TDR = TxData;
        Uart->TxBusy = True;
    }
    return True;
}


void Uart_RxBufferClear(Uart_HandleType Uart)
{
    CRITICAL_SECTION_ENTER;
    Fifo_Clear(Uart->RxFifo, False);
    CRITICAL_SECTION_EXIT;
}

U8 Uart_GetNofInputBufferBytes(Uart_HandleType Uart)
{
    return Uart->RxFifo->NofItems;
}

U8 Uart_GetNofOutputBufferBytes(Uart_HandleType Uart)
{
    return Uart->TxFifo->NofItems;
}


/* ------------------------------- Interrupt handlers ------------------------------ */

/**
 * @brief Interrupt handler for USART1.
 */
#if defined(USART1_ENABLE)
void USART1_IRQHandler(void)
{
    const U32 TempIsr = USART1->ISR;

    /* Interrupt triggered by data reception */
    if (TempIsr & USART_ISR_RXNE)
    {
        if (!Fifo_Full(Usart1Handle.RxFifo))
        {
            const U8 RxData = (U8)(USART1->RDR & 0xFFUL);
            Fifo_WriteByte(Usart1Handle.RxFifo, RxData);
        }
    }

    /* Interrupt triggered by data transmission */
    if (TempIsr & USART_ISR_TXE)
    {
        if (!Fifo_Empty(Usart1Handle.TxFifo))
        {
            U8 TxData;
            Fifo_ReadByte(Usart1Handle.TxFifo, &TxData);
            USART1->TDR = TxData;
        }
        else
        {
            Usart1Handle.TxBusy = False;
            USART1->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
#endif /* USART1_ENABLE */


/**
 * @brief Interrupt handler for USART2.
 */
#if defined(USART2_ENABLE)
void USART2_IRQHandler(void)
{
    const U32 TempIsr = USART2->ISR;

    /* Interrupt triggered by data reception */
    if (TempIsr & USART_ISR_RXNE)
    {
        if (!Fifo_Full(Usart2Handle.RxFifo))
        {
            const U8 RxData = (U8)(USART2->RDR & 0xFFUL);
            Fifo_WriteByte(Usart2Handle.RxFifo, RxData);
        }
    }

    /* Interrupt triggered by data transmission */
    if (TempIsr & USART_ISR_TXE)
    {
        if (!Fifo_Empty(Usart2Handle.TxFifo))
        {
            U8 TxData;
            Fifo_ReadByte(Usart2Handle.TxFifo, &TxData);
            USART2->TDR = TxData;
        }
        else
        {
            Usart2Handle.TxBusy = False;
            USART2->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
#endif /* USART2_ENABLE */


/**
 * @brief Interrupt handler for USART3.
 */
#if defined(USART3_ENABLE)
void USART3_IRQHandler(void)
{
    const U32 TempIsr = USART3->ISR;

    /* Interrupt triggered by data reception */
    if (TempIsr & USART_ISR_RXNE)
    {
        if (!Fifo_Full(Usart3Handle.RxFifo))
        {
            const U8 RxData = (U8)(USART3->RDR & 0xFFUL);
            Fifo_WriteByte(Usart3Handle.RxFifo, RxData);
        }
    }

    /* Interrupt triggered by data transmission */
    if (TempIsr & USART_ISR_TXE)
    {
        if (!Fifo_Empty(Usart3Handle.TxFifo))
        {
            U8 TxData;
            Fifo_ReadByte(Usart3Handle.TxFifo, &TxData);
            USART3->TDR = TxData;
        }
        else
        {
            Usart3Handle.TxBusy = False;
            USART3->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
#endif /* USART3_ENABLE */


/**
 * @brief Interrupt handler for UART4.
 */
#if defined(UART4_ENABLE)
void UART4_IRQHandler(void)
{
    const U32 TempIsr = UART4->ISR;

    /* Interrupt triggered by data reception */
    if (TempIsr & USART_ISR_RXNE)
    {
        if (!Fifo_Full(Uart4Handle.RxFifo))
        {
            const U8 RxData = (U8)(UART4->RDR & 0xFFUL);
            Fifo_WriteByte(Uart4Handle.RxFifo, RxData);
        }
    }

    /* Interrupt triggered by data transmission */
    if (TempIsr & USART_ISR_TXE)
    {
        if (!Fifo_Empty(Uart4Handle.TxFifo))
        {
            U8 TxData;
            Fifo_ReadByte(Uart4Handle.TxFifo, &TxData);
            UART4->TDR = TxData;
        }
        else
        {
            Uart4Handle.TxBusy = False;
            UART4->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
#endif /* UART4_ENABLE */


/**
 * @brief Interrupt handler for UART5.
 */
#if defined(UART5_ENABLE)
void UART5_IRQHandler(void)
{
    const U32 TempIsr = UART5->ISR;

    /* Interrupt triggered by data reception */
    if (TempIsr & USART_ISR_RXNE)
    {
        if (!Fifo_Full(Uart5Handle.RxFifo))
        {
            const U8 RxData = (U8)(UART5->RDR & 0xFFUL);
            Fifo_WriteByte(Uart5Handle.RxFifo, RxData);
        }
    }

    /* Interrupt triggered by data transmission */
    if (TempIsr & USART_ISR_TXE)
    {
        if (!Fifo_Empty(Uart5Handle.TxFifo))
        {
            U8 TxData;
            Fifo_ReadByte(Uart5Handle.TxFifo, &TxData);
            UART5->TDR = TxData;
        }
        else
        {
            Uart5Handle.TxBusy = False;
            UART5->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
#endif /* UART5_ENABLE */


/**
 * @brief Interrupt handler for LPUART1.
 */
#if defined(LPUART1_ENABLE)
void LPUART1_IRQHandler(void)
{
    const U32 TempIsr = LPUART1->ISR;

    /* Interrupt triggered by data reception */
    if (TempIsr & USART_ISR_RXNE)
    {
        if (!Fifo_Full(Lpuart1Handle.RxFifo))
        {
            const U8 RxData = (U8)(LPUART1->RDR & 0xFFUL);
            Fifo_WriteByte(Lpuart1Handle.RxFifo, RxData);
        }
    }

    /* Interrupt triggered by data transmission */
    if (TempIsr & USART_ISR_TXE)
    {
        if (!Fifo_Empty(Lpuart1Handle.TxFifo))
        {
            U8 TxData;
            Fifo_ReadByte(Lpuart1Handle.TxFifo, &TxData);
            LPUART1->TDR = TxData;
        }
        else
        {
            Lpuart1Handle.TxBusy = False;
            LPUART1->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
#endif /* UART4_ENABLE */
