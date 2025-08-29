/**
 * @file cobs_codec.h
 *
 * @brief Interface for COBS codec module.
 */

#ifndef COBS_CODEC_H
#define COBS_CODEC_H

/* -------------------------------- DLL shenanigans -------------------------------- */

#ifdef BUILD_DLL
    #define DLLEXPORT __declspec(dllexport)
#else
    #define DLLEXPORT
#endif

/* ------------------------------- Include directives ------------------------------ */

#include "typedef.h"

/* ---------------------------- Preprocessor definitions --------------------------- */

#define COBS_MINIMUM_ENCODE_OVERHEAD    (2U)    /* One control byte plus zero delimiter byte. */

/* --------------------------- Structures & enumerations --------------------------- */

typedef struct
{
    U16 Length;
    Bool Valid;
} CobsCodec_ResultType;

/* -------------------------- Public function declarations ------------------------- */

/**
 * @brief Encode the given data.
 * @param Src Pointer to data to be encoded.
 * @param SrcLen Length of data to be encoded.
 * @param Dst Encoded data output buffer.
 * @param DstLen Capacity of output buffer.
 * @return Result of the encoding operation.
 */
DLLEXPORT CobsCodec_ResultType CobsCodec_Encode(const U8* Src, U16 SrcLen, U8* Dst, U16 DstLen);

/**
 * @brief Decode the given data.
 * @param Src Pointer to data to be decoded.
 * @param SrcLen Length of data to be decoded.
 * @param Dst Decoded data output buffer.
 * @param DstLen Capacity of output buffer.
 * @return Result of the decoding operation.
 */
DLLEXPORT CobsCodec_ResultType CobsCodec_Decode(const U8* Src, U16 SrcLen, U8* Dst, U16 DstLen);

#endif /* COBS_CODEC_H */
