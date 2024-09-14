/**
 * @file fifo.h
 *
 * @brief First In First Out buffer data structure.
 *
 */

#ifndef FIFO_H
#define FIFO_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"

/*  -------------------------- Structures & enumerations --------------------------- */

/**
 * @brief FIFO structure.
 */
typedef struct FifoType
{
    U8* Buffer;
    U8 Length;
    U8 Mask;
    U8 Head;
    U8 Tail;
    U8 NofItems;
} FifoType;

/* --------------------------- Public function prototypes -------------------------- */

/**
 *  @brief Initialization function for fifo structures.
 *  @param Fifo Pointer to fifo structure.
 *  @param Buffer Pointer to buffer where data for the fifo to manage is stored.
 *  @param Length Length of buffer.
 */
void Fifo_Init(FifoType* Fifo, U8* Buffer, const U8 Length);


/**
 *  @brief Write one byte of data to the fifo.
 *  @param Fifo Pointer to fifo structure.
 *  @param Data Byte to be written.
 */
void Fifo_WriteByte(FifoType* Fifo, U8 Data);


/**
 *  @brief Read one byte of data from the fifo.
 *  @param Fifo Pointer to fifo structure.
 *  @param Data Pointer to where the read data should be stored.
 */
void Fifo_ReadByte(FifoType* Fifo, U8* Data);


/**
 *  @brief Checks if a fifo structure is empty.
 *  @param Fifo Pointer to fifo structure.
 *  @returns True = empty, False = not empty
 */
Bool Fifo_Empty(const FifoType* Fifo);


/**
 *  @brief Checks if a fifo structure is full.
 *  @param Fifo Pointer to fifo structure.
 *  @returns True = full, False = not full
 */
Bool Fifo_Full(const FifoType* Fifo);


/**
 *  @brief Checks a fifo structure for the number of bytes that
 *         can be written before overwrite protection kicks in.
 *  @param Fifo Pointer to fifo structure.
 *  @returns Number of available bytes that can be written.
 */
U8 Fifo_GetNofAvailable(const FifoType* Fifo);


/**
 *  @brief Checks a fifo structure for the number of bytes
 *         that are in the underlying buffer that has not been read.
 *  @param Fifo Pointer to fifo strcture.
 *  @returns Number of unread bytes.
 */
U8 Fifo_GetNofItems(const FifoType* Fifo);


/**
 * @brief Resets a fifo structure and optionally zero-fills it's underlying buffer.
 * @param Fifo Pointer to fifo structure.
 * @param ZeroFill Set True to zero-fill.
 * @returns
 */
void Fifo_Clear(FifoType* Fifo, Bool ZeroFill);

#endif // FIFO_H
