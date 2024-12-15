/**
 * @file crc.h
 *
 * @brief Hardware accelerated CRC computation module.
 *
 */

/* ------------------------------- Include directives ------------------------------ */
#include "crc.h"

/* --------------------------------- Local variables ------------------------------- */
static U8 Crc8XorVal = 0x00U;

/**
 * @todo Implement support for CRC-16, CRC-32
 */
// static U16 Crc16XorVal = 0x0000U;
// static U32 Crc32XorVal = 0x00000000U;

/* -------------------------- Private function definitions ------------------------- */

/* ------------------------- Public function definitions --------------------------- */

void Crc_Crc8Init(const Crc_Crc8ConfigType* Config)
{
    Crc_SetInitVal((U32)Config->InitValue);
    Crc_SetPolynomialSize(Config->PolySize);
    Crc_SetPolynomial(Config->Polynomial);
    Crc_SetInputBitOrder(Config->InputBitOrder);
    Crc_SetOutputBitOrder(Config->OutputBitOrder);
    Crc8XorVal = Config->XorValue;
    Crc_Reset();
}

U8 Crc_CalcCrc8(const U8* Buffer, U8 Length)
{
    /* Force right-aligned byte access. */
    U8* DataRegPtr = (U8*)&CRC->DR;
    Crc_Reset();
    for (U8 i = 0; i < Length; i++)
    {
        *DataRegPtr = Buffer[i];
    }
    return ((U8)CRC->DR ^ Crc8XorVal);
}

void Crc_SetPolynomialSize(Crc_PolynomialSizeEnum PolySize)
{
    CRC->CR &= ~CRC_CR_POLYSIZE;
    CRC->CR |= PolySize << CRC_CR_POLYSIZE_Pos;
}

void Crc_SetInputBitOrder(Crc_InputBitOrderEnum BitOrder)
{
    CRC->CR &= ~CRC_CR_REV_IN;
    CRC->CR |= BitOrder << CRC_CR_REV_IN_Pos;
}

void Crc_SetOutputBitOrder(Crc_OutputBitOrderEnum BitOrder)
{
    if ( BitOrder == CRC_OUTPUT_BIT_ORDER_NORMAL )
    {
        CRC->CR &= ~CRC_CR_REV_OUT;
    }
    else
    {
        CRC->CR |= CRC_CR_REV_OUT;
    }
}

Crc_Crc8ConfigType Crc_GetSAEJ1850Config(void)
{
    return (Crc_Crc8ConfigType)
    {
        .InitValue = 0xFF,
        .PolySize = POLY_SIZE_8,
        .Polynomial = 0x1D,
        .XorValue = 0xFF,
        .InputBitOrder = CRC_INPUT_BIT_ORDER_NORMAL,
        .OutputBitOrder = CRC_OUTPUT_BIT_ORDER_NORMAL
    };
}
