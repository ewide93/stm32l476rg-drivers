/**
 * @file range.h
 *
 * @brief Range checking functions.
 */

#ifndef RANGE_H
#define RANGE_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"

/* -------------------------- Public function definitions -------------------------- */
static inline Bool Range_U8(U8 Value, U8 Min, U8 Max)
{
    return (Value >= Min && Value <= Max);
}

static inline Bool Range_U16(U16 Value, U16 Min, U16 Max)
{
    return (Value >= Min && Value <= Max);
}

static inline Bool Range_U32(U32 Value, U32 Min, U32 Max)
{
    return (Value >= Min && Value <= Max);
}

static inline Bool Range_U64(U64 Value, U64 Min, U64 Max)
{
    return (Value >= Min && Value <= Max);
}

static inline Bool Range_S8(S8 Value, S8 Min, S8 Max)
{
    return (Value >= Min && Value <= Max);
}

static inline Bool Range_S16(S16 Value, S16 Min, S16 Max)
{
    return (Value >= Min && Value <= Max);
}

static inline Bool Range_S32(S32 Value, S32 Min, S32 Max)
{
    return (Value >= Min && Value <= Max);
}

static inline Bool Range_S64(S64 Value, S64 Min, S64 Max)
{
    return (Value >= Min && Value <= Max);
}

static inline Bool Range_F32(F32 Value, F32 Min, F32 Max)
{
    return (Value >= Min && Value <= Max);
}

#endif  /* RANGE_H*/
