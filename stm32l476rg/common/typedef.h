#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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
typedef void (*CallbackFunction)(void);

#define True     (true)
#define False    (false)

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define StaticAssert(Condition, Message) _Static_assert(Condition, Message)
#else
#error "Use of static assertions requires C11 or greater."
#endif

/**
 * @brief Generic return code enumeration.
 */
typedef enum
{
    RC_OK = 0x0U,
    RC_ERROR = 0x1U
} ReturnCodeEnum;

#endif /* TYPEDEF_H */