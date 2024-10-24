/**
 * @file fifo.c
 *
 * @brief First In First Out buffer data structure.
 * @note Wrap-around logic requires the size of the underlying buffer
 *       to be a power of two.
 */

/* ------------------------------- Include directives ------------------------------ */

#include "fifo.h"

/* -------------------------- Public function definitions -------------------------- */

void Fifo_Init(FifoType* Fifo, U8* Buffer, const U8 Length)
{
    Fifo->Buffer = Buffer;
    Fifo->Length = Length;
    Fifo->Mask = Length - 1;
    Fifo->Head = 0;
    Fifo->Tail = 0;
    Fifo->NofItems = 0;
}


void Fifo_WriteByte(FifoType* Fifo, U8 Data)
{
    if (Fifo_Full(Fifo)) return;
    Fifo->Buffer[Fifo->Head] = Data;
    Fifo->Head = (Fifo->Head + 1) & Fifo->Mask;
    Fifo->NofItems++;
}


void Fifo_ReadByte(FifoType* Fifo, U8* Data)
{
    if (Fifo_Empty(Fifo)) return;
    *Data = Fifo->Buffer[Fifo->Tail];
    Fifo->Tail = (Fifo->Tail + 1) & Fifo->Mask;
    Fifo->NofItems--;
}


Bool Fifo_Empty(const FifoType* Fifo)
{
    return Fifo->NofItems == 0;
}


Bool Fifo_Full(const FifoType* Fifo)
{
    return Fifo->NofItems == Fifo->Length;
}


U8 Fifo_GetNofAvailable(const FifoType* Fifo)
{
    return (Fifo->Length - Fifo->NofItems);
}


U8 Fifo_GetNofItems(const FifoType* Fifo)
{
    return Fifo->NofItems;
}


void Fifo_Clear(FifoType* Fifo, Bool ZeroFill)
{
    if (ZeroFill)
    {
        for (U8 i = 0; i < Fifo->Length; i++)
        {
            Fifo->Buffer[i] = 0;
        }
    }

    Fifo->Head = 0;
    Fifo->Tail = 0;
    Fifo->NofItems = 0;
}
