/**
 * @file limit.h
 *
 * @brief Limiting functions.
 */

#ifndef LIMIT_H
#define LIMIT_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"

/* -------------------------- Public function definitions -------------------------- */
static inline U8 Limit_U8(U8 Value, U8 Min, U8 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

static inline U16 Limit_U16(U16 Value, U16 Min, U16 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

static inline U32 Limit_U32(U32 Value, U32 Min, U32 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

static inline U64 Limit_U64(U64 Value, U64 Min, U64 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

static inline S8 Limit_S8(S8 Value, S8 Min, S8 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

static inline S16 Limit_S16(S16 Value, S16 Min, S16 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

static inline S32 Limit_S32(S32 Value, S32 Min, S32 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

static inline S64 Limit_S64(S64 Value, S64 Min, S64 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

static inline F32 Limit_F32(F32 Value, F32 Min, F32 Max)
{
    if (Value >= Min && Value <= Max) { return Value; }
    else { return (Value < Min) ? Min : Max; }
}

#endif  /* LIMIT_H*/
