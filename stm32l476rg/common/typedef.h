/**
 * @file typedef.h
 *
 * @brief Common type definitions & preprocessor marcros.
 *
 */

#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint64_t U64;
typedef uint32_t U32;
#define U32_MAX UINT32_MAX
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

#if defined(__GNUC__)
    #define ALIGN(Alignment)          __attribute__((aligned((Alignment))))
    #define SECTION(LinkerSection)    __attribute__((section(LinkerSection)))
#endif /* GCC attribute wrapper macros. */

/**
 * @brief Mark an argument as unused to prevent compiler warnings.
 */
#define UNUSED(Arg) ((void)(Arg))

/**
 * @brief Generic return code enumeration.
 */
typedef enum
{
    RC_OK = 0x0U,
    RC_ERROR = 0x1U
} ReturnCodeEnum;

#endif /* TYPEDEF_H */