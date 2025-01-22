/**
 * @file memory_routines.h
 *
 * @brief Provide implementations of memcpy & memset for FreeRTOS to avoid
 *        dependency on libc header string.h.
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

#endif /* MEMORY_ROUTINES_H */
