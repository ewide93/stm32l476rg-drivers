/**
 * @file crc.h
 *
 * @brief Hardware accelerated CRC computation module.
 *
 */

#ifndef CRC_H
#define CRC_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"
#include "clock_control.h"

/* ---------------------------- Preprocessor directives ---------------------------- */

/*  -------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of the avaiable settings for CRC polynomial size.
 */
typedef enum
{
    POLY_SIZE_32 = 0x0U,        /* 32-bit */
    POLY_SIZE_16 = 0x1U,        /* 16-bit */
    POLY_SIZE_8 = 0x2U,         /* 8-bit  */
    POLY_SIZE_7 = 0x3U          /* 7-bit  */
} Crc_PolynomialSizeEnum;

/**
 * @brief Enumeration of the available setting for reversal of CRC input.
 */
typedef enum
{
    CRC_INPUT_BIT_ORDER_NORMAL = 0x0U,                 /* Bit order not affected     */
    CRC_INPUT_BIT_ORDER_REVERSED_BYTE = 0x1U,          /* Bytewise bit reversal      */
    CRC_INPUT_BIT_ORDER_REVERSED_HALFWORD = 0x2U,      /* Half-wordwise bit reversal */
    CRC_INPUT_BIT_ORDER_REVERSED_WORD = 0x3U           /* Wordwise bit reversal      */
} Crc_InputBitOrderEnum;

/**
 * @brief Enumeration of the available setting for reversal of CRC output.
 */
typedef enum
{
    CRC_OUTPUT_BIT_ORDER_NORMAL = 0x0U,        /* Bit order not affected */
    CRC_OUTPUT_BIT_ORDER_REVERSED = 0x1U       /* Bit order reversed     */
} Crc_OutputBitOrderEnum;

/**
 * @brief CRC configuration structure.
 */
typedef struct
{
    U8 Polynomial;
    U8 InitValue;
    U8 XorValue;
    Crc_PolynomialSizeEnum PolySize;
    Crc_InputBitOrderEnum InputBitOrder;
    Crc_OutputBitOrderEnum OutputBitOrder;
} Crc_Crc8ConfigType;


/* -------------------------- Public function prototypes --------------------------- */

/**
 * @brief Initialize the CRC peripheral with the given configuration.
 * @param Config CRC configuration structure.
 */
void Crc_Crc8Init(const Crc_Crc8ConfigType* Config);

/**
 * @brief Calculate the CRC-8 for the given buffer.
 * @param Buffer Pointer to data for which CRC is to be calculated.
 * @param Length Number of bytes in buffer.
 * @return Calculated CRC-8.
 */
U8 Crc_CalcCrc8(const U8* Buffer, U8 Length);

/**
 * @brief Set the size of the CRC polynomial.
 * @param PolySize Polynomial size.
 */
void Crc_SetPolynomialSize(Crc_PolynomialSizeEnum PolySize);

/**
 * @brief Set CRC data input bit ordering scheme.
 * @param BitOrder Desired bit ordering scheme.
 */
void Crc_SetInputBitOrder(Crc_InputBitOrderEnum BitOrder);

/**
 * @brief Set CRC data output bit ordering scheme.
 * @param BitOrder Desired bit ordering scheme.
 */
void Crc_SetOutputBitOrder(Crc_OutputBitOrderEnum BitOrder);

/**
 * @brief Get a SAE-J1850 CRC-8 configuration.
 * @return CRC-8 configuration.
 */
Crc_Crc8ConfigType Crc_GetSAEJ1850Config(void);

/* ------------------------- Public function definitions --------------------------- */

/**
 * @brief Enable the CRC peripheral.
 */
static inline void Crc_Enable(void)
{
    ClkCtrl_PeripheralClockEnable(PCLK_CRC);
}

/**
 * @brief Disable the CRC peripheral.
 */
static inline void Crc_Disable(void)
{
    ClkCtrl_PeripheralClockDisable(PCLK_CRC);
}

/**
 * @brief Reset the CRC calculation unit.
 */
static inline void Crc_Reset(void)
{
    CRC->CR |= CRC_CR_RESET;
}

/**
 * @brief Set the initial value for the CRC data register.
 * @param Value Initial value.
 */
static inline void Crc_SetInitVal(U32 Value)
{
    CRC->INIT = Value;
}

/**
 * @brief Set the CRC polynomial.
 * @param Poly Polynomial value.
 */
static inline void Crc_SetPolynomial(U32 Poly)
{
    CRC->POL = Poly;
}

#endif /* CRC_H */
