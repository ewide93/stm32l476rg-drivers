/**
 * @file cobs_codec.h
 *
 * @brief Implementation of COBS codec module.
 */

/* ------------------------------- Include directives ------------------------------ */

#include "cobs_codec.h"

/* -------------------------- Public function definitions -------------------------- */

CobsCodec_ResultType CobsCodec_Encode(const U8* Src, U16 SrcLen, U8* Dst, U16 DstLen)
{
    U16 WriteIdx = 1U;
    CobsCodec_ResultType Result = { .Length = 0UL, .Valid = False };

    /* Verify that both pointers are valid & that the destination buffer is
    at least big enough to handle the minimum encoded data length. */
    if ( (Src != NULL) && (Dst != NULL) && (DstLen >= SrcLen + COBS_MINIMUM_ENCODE_OVERHEAD) )
    {
        U16 ControlByteIdx = 0U;
        U16 ReadIdx = 0U;
        U8 ConsecutiveNonZeroBytes = 0U;

        for (;;)
        {
            /* Check for buffer overflows */
            if ( (WriteIdx >= DstLen) || (ReadIdx >= SrcLen) ) { break; }

            /* Should we insert pointer byte? */
            if ( ConsecutiveNonZeroBytes >= 254U )
            {
                Dst[ControlByteIdx] = 0xFFU;
                ControlByteIdx = WriteIdx;
                WriteIdx++;
                ConsecutiveNonZeroBytes = 0U;
                continue;
            }

            if ( Src[ReadIdx] != 0x00U )
            {
                Dst[WriteIdx] = Src[ReadIdx];
                ConsecutiveNonZeroBytes++;
            }
            else
            {
                Dst[ControlByteIdx] = WriteIdx - ControlByteIdx;
                ControlByteIdx = WriteIdx;
                ConsecutiveNonZeroBytes = 0U;
            }

            ReadIdx++;
            WriteIdx++;

            if ( ReadIdx == SrcLen )
            {
                /* Is there enough room in destination buffer to complete? */
                if ( (WriteIdx + 2U) >= DstLen ) { break; }

                Dst[ControlByteIdx] = WriteIdx - ControlByteIdx;
                WriteIdx++;
                Dst[WriteIdx++] = 0x00U;
                Result.Valid = True;
                break;
            }
        }
    }

    Result.Length = WriteIdx - 1U;
    return Result;
}

CobsCodec_ResultType CobsCodec_Decode(const U8* Src, U16 SrcLen, U8* Dst, U16 DstLen)
{
    U16 WriteIdx = 0U;
    CobsCodec_ResultType Result = { .Length = 0U, .Valid = False };

    if ( (Src != NULL) && (Dst != NULL) && (DstLen >= SrcLen - COBS_MINIMUM_ENCODE_OVERHEAD) )
    {
        U16 ControlByteIdx = Src[0];
        bool NextControlByteIsPointer = (ControlByteIdx == 0xFFU);

        for (U16 ReadIdx = 1U; ReadIdx < SrcLen; ReadIdx++)
        {
            /* Check for zero delimiter, only set the valid flag if it's the last byte. */
            if ( Src[ReadIdx] == 0x00U ) { Result.Valid = (ReadIdx == SrcLen - 1U); break; }

            if ( ReadIdx == ControlByteIdx )
            {
                if ( NextControlByteIsPointer )
                {
                    NextControlByteIsPointer = (Src[ReadIdx] == 0xFFU);
                }
                else
                {
                    if ( WriteIdx >= SrcLen ) { break; }
                    Dst[WriteIdx++] = 0x00U;
                }
                ControlByteIdx += Src[ReadIdx];
            }
            else
            {
                if ( WriteIdx >= SrcLen ) { break; }
                Dst[WriteIdx++] = Src[ReadIdx];
            }
        }
    }

    Result.Length = WriteIdx;
    return Result;
}
