/**
 * @file memory_routines.c
 *
 * @brief Provide implementations of memcpy & memset for FreeRTOS to avoid
 *        dependency on libc header string.h.
 *        Additionally provide helper functions for memory alignment.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "memory_routines.h"

/* -------------------------- Public function definitions -------------------------- */

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

Bool IsPowerOfTwo(U32 Value)
{
    return ( Value > 0 ) && ( ( Value & ( Value - 1U ) ) == 0 );
}

Bool IsAligned(void* Address, U32 Alignment)
{
    StaticAssert(sizeof(U32) == sizeof(void*), "Mismatched length for type casting from void* to U32");
    const U32 AlignmentMask = Alignment - 1U;
    return ( ( (U32)Address & AlignmentMask ) == 0 );
}
