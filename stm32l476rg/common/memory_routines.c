/**
 * @file memory_routines.c
 *
 * @brief Provide implementations of memcpy & memset for FreeRTOS to avoid
 *        dependency on libc header string.h.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "memory_routines.h"

void* memcpy(void* restrict Destination, const void* restrict Source, size_t Count)
{
    for (size_t i = 0; i < Count; i++)
    {
        ((unsigned char*)Destination)[i] = ((unsigned char*)Source)[i];
    }
    return Destination;
}

void* memset(void* Destination, int Data, size_t Count)
{
    for (size_t i = 0; i < Count; i++)
    {
        ((unsigned char*)Destination)[i] = (unsigned char)Data;
    }
    return Destination;
}
