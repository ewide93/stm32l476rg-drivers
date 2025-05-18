/**
 * @file memory_routines.h
 *
 * @brief Provide implementations of memcpy & memset for FreeRTOS to avoid
 *        dependency on libc header string.h.
 *        Additionally provide helper functions for memory alignment.
 */

#ifndef MEMORY_ROUTINES_H
#define MEMORY_ROUTINES_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"

/* --------------------------- Public function prototypes -------------------------- */

/**
 * @brief Copy the given number of bytes from source to destination.
 * @param Destination Destination start address.
 * @param Source Source start address.
 * @param Count Number of bytes to copy.
 * @return Copy of Destination.
 */
void* memcpy(void* restrict Destination, const void* restrict Source, size_t Count);

/**
 * @brief Starting from the given destination address, fill the
 *        next Count bytes with Data.
 * @param Destination Destination start address.
 * @param Data Fill value.
 * @param Count Number of bytes to fill.
 * @return Copy of Destination.
 */
void* memset(void* Destination, int Data, size_t Count);

/**
 * @brief Check if a value is a power of two (valid memory aligment).
 * @param Value The value to test.
 * @return True = is a power of two, False = is not a power of two.
 */
Bool IsPowerOfTwo(U32 Value);


/**
 * @brief Check if the given address has the given alignment.
 * @param Address Address to check.
 * @param Alignment Alignment to check against.
 * @return True = is aligned, False = is not aligned.
 */
Bool IsAligned(void* Address, U32 Alignment);

#endif /* MEMORY_ROUTINES_H */
