#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <stdint.h>
#include <stdbool.h>

#define NULL ((void*)0)

typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t U8;

typedef int64_t S64;
typedef int32_t S32;
typedef int16_t S16;
typedef int8_t S8;

typedef char Char;

typedef float F32;
typedef bool Bool;
#define True     (true)
#define False    (false)

/**
 * @brief Generic return code enumeration.
 */
typedef enum
{
    RC_OK = 0x0U,
    RC_ERROR = 0x1U
} ReturnCodeEnum;

#endif /* TYPEDEF_H */