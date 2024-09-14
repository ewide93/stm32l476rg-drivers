/**
 * @file clock_control.c
 *
 * @brief Low level module for configuration & control of MCU clocks.
 */

/* ------------------------------- Include directives ------------------------------ */
#include "clock_control.h"
#include "limit.h"
#include "range.h"

/*  ------------------------ Local preprocessor definitions ------------------------ */
#define PLLP_Pos    (27U)
#define PLLP_Msk    (0x1FUL << PLLP_Pos)

/*  -------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Clock control module status structure.
 */
typedef struct
{
    U32 MsiFreq_Hz;
    U32 SysclkFreq_Hz;
    U32 HclkFreq_Hz;
    U32 Pclk1Freq_Hz;
    U32 Pclk2Freq_Hz;
    U32 PllInFreq_Hz;
    U32 MainPllVcoOutFreq_Hz;
    U32 MainPllPFreq_Hz;
    U32 MainPllQFreq_Hz;
    U32 MainPllRFreq_Hz;
    U32 Pllsai1VcoOutFreq_Hz;
    U32 Pllsai1PFreq_Hz;
    U32 Pllsai1QFreq_Hz;
    U32 Pllsai1RFreq_Hz;
    U32 Pllsai2VcoOutFreq_Hz;
    U32 Pllsai2PFreq_Hz;
    U32 Pllsai2RFreq_Hz;
    ClkCtrl_SysclkInputEnum SysclkInput;
    ClkCtrl_PllInputEnum PllInput;
    bool AhbPrescalerLocked;
    bool Apb1PrescalerLocked;
    bool Apb2PrescalerLocked;
} ClkCtrl_ClockTreeType;

/*  ------------------------- Private function prototypes -------------------------- */

/*  ------------------------------ Private variables ------------------------------- */

static ClkCtrl_ClockTreeType ClockTree =
{
    .MsiFreq_Hz = MSI_INIT_FREQ_Hz,
    .SysclkFreq_Hz = MSI_INIT_FREQ_Hz,
    .HclkFreq_Hz = MSI_INIT_FREQ_Hz,
    .Pclk1Freq_Hz = MSI_INIT_FREQ_Hz,
    .Pclk2Freq_Hz = MSI_INIT_FREQ_Hz,
    .PllInFreq_Hz = 0UL,
    .MainPllVcoOutFreq_Hz = 0UL,
    .MainPllPFreq_Hz = 0UL,
    .MainPllQFreq_Hz = 0UL,
    .MainPllRFreq_Hz = 0UL,
    .Pllsai1VcoOutFreq_Hz = 0UL,
    .Pllsai1PFreq_Hz = 0UL,
    .Pllsai1QFreq_Hz = 0UL,
    .Pllsai1RFreq_Hz = 0UL,
    .Pllsai2VcoOutFreq_Hz = 0UL,
    .Pllsai2PFreq_Hz = 0UL,
    .Pllsai2RFreq_Hz = 0UL,
    .SysclkInput = SYSCLK_INPUT_MSI,
    .PllInput = PLL_INPUT_NONE,
    .AhbPrescalerLocked = false,
    .Apb1PrescalerLocked = false,
    .Apb2PrescalerLocked = false
};

/*  ------------------------- Private function definitions ------------------------- */

/**
 * @brief Extract an integer divisor from enumerated clock divider Q.
 * @param Q Q enumeration.
 * @return Integer divisor matching the given Q enumeration.
 */
static inline U8 ClkCtrl_QToDivisor(ClkCtrl_PllQEnum Q)
{
    U8 Rv;
    switch (Q)
    {
        case PLL_Q_2: { Rv = 2U; break; }
        case PLL_Q_4: { Rv = 4U; break; }
        case PLL_Q_6: { Rv = 6U; break; }
        case PLL_Q_8: { Rv = 8U; break; }
        default: { Rv = 1; break; }
    }
    return Rv;
}


/**
 * @brief Extract an integer divisor from enumerated clock divider R.
 * @param R R enumeration.
 * @return Integer divisor matching the given R enumeration.
 */
static inline U8 ClkCtrl_RToDivisor(ClkCtrl_PllREnum R)
{
    U8 Rv;
    switch (R)
    {
        case PLL_R_2: { Rv = 2U; break; }
        case PLL_R_4: { Rv = 4U; break; }
        case PLL_R_6: { Rv = 6U; break; }
        case PLL_R_8: { Rv = 8U; break; }
        default: { Rv = 1; break; }
    }
    return Rv;
}


/**
 * @brief Extract an integer divisor from enumerated AHB prescaler.
 * @param Prescaler AHB prescaler enumeration.
 * @return Integer divisor matching the given AHB prescaler.
 */
static inline S16 ClkCtrl_AhbPrescalerToDivisor(ClkCtrl_AhbPrescalerEnum Prescaler)
{
    S16 Rv;
    switch (Prescaler)
    {
        case AHB_PS_1:   { Rv = 1;   break; }
        case AHB_PS_2:   { Rv = 2;   break; }
        case AHB_PS_4:   { Rv = 4;   break; }
        case AHB_PS_8:   { Rv = 8;   break; }
        case AHB_PS_16:  { Rv = 16;  break; }
        case AHB_PS_64:  { Rv = 64;  break; }
        case AHB_PS_128: { Rv = 128; break; }
        case AHB_PS_256: { Rv = 256; break; }
        case AHB_PS_512: { Rv = 512; break; }
        default:         { Rv = -1;  break; }
    }
    return Rv;
}


/**
 * @brief Extract an integer divisor from enumerated APB prescaler.
 * @param Prescaler APB prescaler enumeration.
 * @return Integer divisor matching the given AHB prescaler.
 */
static inline S16 ClkCtrl_ApbPrescalerToDivisor(ClkCtrl_ApbPrescalerEnum Prescaler)
{
    S16 Rv;
    switch (Prescaler)
    {
        case APB_PS_1:  { Rv = 1;  break; }
        case APB_PS_2:  { Rv = 2;  break; }
        case APB_PS_4:  { Rv = 4;  break; }
        case APB_PS_8:  { Rv = 8;  break; }
        case APB_PS_16: { Rv = 16; break; }
        default:        { Rv = -1; break; }
    }
    return Rv;
}


/**
 * @brief Transform a given enumeration of available MSI frequency ranges
 *        to a value in Hz.
 * @param FreqRange MSI oscillator frequency range enumeration.
 * @return Frequency in Hertz.
 */
static inline U32 ClkCtrl_MsiFreqRangeToHz(ClkCtrl_MsiFreqRangeEnum FreqRange)
{
    U32 Rv;
    switch (FreqRange)
    {
        case MSI_FREQ_100_kHz: { Rv = 100000UL;   break; }
        case MSI_FREQ_200_kHz: { Rv = 200000UL;   break; }
        case MSI_FREQ_400_kHz: { Rv = 400000UL;   break; }
        case MSI_FREQ_800_kHz: { Rv = 800000UL;   break; }
        case MSI_FREQ_1_MHz:   { Rv = 1000000UL;  break; }
        case MSI_FREQ_2_MHz:   { Rv = 2000000UL;  break; }
        case MSI_FREQ_4_MHz:   { Rv = 4000000UL;  break; }
        case MSI_FREQ_8_MHz:   { Rv = 8000000UL;  break; }
        case MSI_FREQ_16_MHz:  { Rv = 16000000UL; break; }
        case MSI_FREQ_24_MHz:  { Rv = 24000000UL; break; }
        case MSI_FREQ_32_MHz:  { Rv = 32000000UL; break; }
        case MSI_FREQ_48_MHz:  { Rv = 48000000UL; break; }
        default:               { Rv = 0UL;        break; }
    }
    return Rv;
}

/*  ------------------------- Public function definitions -------------------------- */

void ClkCtrl_SetSysclkInput(ClkCtrl_SysclkInputEnum Input)
{
    RCC->CFGR &= ~RCC_CFGR_SW_Msk;
    RCC->CFGR |= Input;
    ClockTree.SysclkInput = Input;

    switch (Input)
    {
        case SYSCLK_INPUT_MSI: { ClockTree.SysclkFreq_Hz = ClockTree.MsiFreq_Hz; break; }
        case SYSCLK_INPUT_HSI: { ClockTree.SysclkFreq_Hz = HSI_FREQ_Hz; break; }
        #if defined(HSE_NOT_INSTALLED)
        #else
        case SYSCLK_INPUT_HSE: { ClockTree.SysclkFreq_Hz = HSE_FREQ; break; }
        #endif
        case SYSCLK_INPUT_PLL: { ClockTree.SysclkFreq_Hz = ClockTree.MainPllRFreq_Hz; break; }
        default: { break; }
    }
}


ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllInput(ClkCtrl_PllInputEnum Input, ClkCtrl_PllMEnum M)
{
    U32 TempPllInFreq_Hz;

    switch (Input)
    {
        case PLL_INPUT_MSI: { TempPllInFreq_Hz = ClockTree.MsiFreq_Hz; break; }
        case PLL_INPUT_HSI: { TempPllInFreq_Hz = HSI_FREQ_Hz; break; }
        #if defined(HSE_NOT_INSTALLED)
        #else
        case PLL_INPUT_HSE: { break; }
        #endif
        default: { return CC_INVALID_CFG; }
    }

    /* Determine if the frequency is within the allowed range */
    TempPllInFreq_Hz /= (M + 1U);
    if ( Range_U32(TempPllInFreq_Hz, VCO_IN_FREQ_MIN_Hz, VCO_IN_FREQ_MAX_Hz) )
    {
        RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC_HSE_Msk;
        RCC->PLLCFGR |= Input;
        RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
        RCC->PLLCFGR |= (M << RCC_PLLCFGR_PLLM_Pos);
        ClockTree.PllInFreq_Hz = TempPllInFreq_Hz;
        ClockTree.PllInput = Input;
    }
    else
    {
        return CC_FREQ_OUT_OF_RANGE;
    }

    return CC_OK;
}


ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllN(ClkCtrl_PllEnum Pll, U8 N)
{
    if ( !Range_U8(N, PLL_N_MIN, PLL_N_MAX) ) { return CC_INVALID_CFG; }

    const U32 TempPllOutFreq_Hz = ClockTree.PllInFreq_Hz * N;
    if ( Range_U32(TempPllOutFreq_Hz, VCO_OUT_FREQ_MIN_Hz, VCO_OUT_FREQ_MAX_Hz) )
    {
        switch (Pll)
        {
            case PLL_MAIN:
            {
                RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
                RCC->PLLCFGR |= (N << RCC_PLLCFGR_PLLN_Pos);
                ClockTree.MainPllVcoOutFreq_Hz = TempPllOutFreq_Hz;
                break;
            }
            case PLL_SAI1:
            {
                RCC->PLLSAI1CFGR &= ~RCC_PLLSAI1CFGR_PLLSAI1N_Msk;
                RCC->PLLSAI1CFGR |= (N << RCC_PLLSAI1CFGR_PLLSAI1N_Pos);
                ClockTree.Pllsai1VcoOutFreq_Hz = TempPllOutFreq_Hz;
                break;
            }
            case PLL_SAI2:
            {
                RCC->PLLSAI2CFGR &= ~RCC_PLLSAI2CFGR_PLLSAI2N_Msk;
                RCC->PLLSAI2CFGR |= (N << RCC_PLLSAI2CFGR_PLLSAI2N_Pos);
                ClockTree.Pllsai2VcoOutFreq_Hz = TempPllOutFreq_Hz;
                break;
            }
            default:
            {
                return CC_INVALID_CFG;
            }
        }
    }
    else
    {
        return CC_FREQ_OUT_OF_RANGE;
    }

    return CC_OK;
}


ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllP(ClkCtrl_PllEnum Pll, U8 P)
{
    if (!Range_U8(P, PLL_P_MIN, PLL_P_MAX)) { return CC_INVALID_CFG; }

    switch (Pll)
    {
        case PLL_MAIN:
        {
            RCC->PLLCFGR &= ~PLLP_Msk;
            RCC->PLLCFGR |= (P << PLLP_Pos);
            ClockTree.MainPllPFreq_Hz = ClockTree.MainPllVcoOutFreq_Hz / P;
            break;
        }
        case PLL_SAI1:
        {
            RCC->PLLSAI1CFGR &= ~PLLP_Msk;
            RCC->PLLSAI1CFGR |= (P << PLLP_Pos);
            ClockTree.Pllsai1PFreq_Hz = ClockTree.Pllsai1VcoOutFreq_Hz / P;
            break;
        }
        case PLL_SAI2:
        {
            RCC->PLLSAI2CFGR &= ~PLLP_Msk;
            RCC->PLLSAI2CFGR |= (P << PLLP_Pos);
            ClockTree.Pllsai1PFreq_Hz = ClockTree.Pllsai1VcoOutFreq_Hz / P;
            break;
        }
        default:
        {
            return CC_INVALID_CFG;
        }
    }

    return CC_OK;
}


ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllQ(ClkCtrl_PllEnum Pll, ClkCtrl_PllQEnum Q)
{
    const U8 Divisor = ClkCtrl_QToDivisor(Q);
    if (Divisor < 2) { return CC_INVALID_CFG; }

    ClkCtrl_ReturnCodeEnum Rc = CC_OK;
    U32 TempQFreq_Hz;

    switch (Pll)
    {
        case PLL_MAIN:
        {
            TempQFreq_Hz = ClockTree.MainPllVcoOutFreq_Hz / Divisor;
            if (TempQFreq_Hz <= PLLQ_OUT_FREQ_MAX_Hz)
            {
                RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ_Msk;
                RCC->PLLCFGR |= (Q << RCC_PLLCFGR_PLLQ_Pos);
                ClockTree.MainPllQFreq_Hz = TempQFreq_Hz;
            }
            else
            {
                Rc = CC_FREQ_OUT_OF_RANGE;
            }
            break;
        }
        case PLL_SAI1:
        {
            TempQFreq_Hz = ClockTree.Pllsai1VcoOutFreq_Hz / Divisor;
            if (TempQFreq_Hz <= PLLQ_OUT_FREQ_MAX_Hz)
            {
                RCC->PLLSAI1CFGR &= ~RCC_PLLSAI1CFGR_PLLSAI1Q_Msk;
                RCC->PLLSAI1CFGR |= (Q << RCC_PLLSAI1CFGR_PLLSAI1Q_Pos);
                ClockTree.Pllsai1QFreq_Hz = TempQFreq_Hz;
            }
            else
            {
                Rc = CC_FREQ_OUT_OF_RANGE;
            }
            break;
        }
        case PLL_SAI2:  /* Intentional fallthrough */
        default:
        {
            Rc = CC_INVALID_CFG;
            break;
        }
    }

    return Rc;
}


ClkCtrl_ReturnCodeEnum ClkCtrl_PllOutputEnable(ClkCtrl_PllEnum Pll, ClkCtrl_PllOutputEnum Output)
{
    if ((Pll == PLL_SAI2) && (Output == PLL_OUTPUT_Q)) { return CC_INVALID_CFG; }
    ClkCtrl_ReturnCodeEnum Rc = CC_OK;

    switch (Pll)
    {
        case PLL_MAIN: { RCC->PLLCFGR |= (1 << Output);     break; }
        case PLL_SAI1: { RCC->PLLSAI1CFGR |= (1 << Output); break; }
        case PLL_SAI2: { RCC->PLLSAI2CFGR |= (1 << Output); break; }
        default:       { Rc = CC_INVALID_CFG;               break; }
    }
    return Rc;
}


ClkCtrl_ReturnCodeEnum ClkCtrl_PllOutputDisable(ClkCtrl_PllEnum Pll, ClkCtrl_PllOutputEnum Output)
{
    if ((Pll == PLL_SAI2) && (Output == PLL_OUTPUT_Q)) { return CC_INVALID_CFG; }
    ClkCtrl_ReturnCodeEnum Rc = CC_OK;

    switch (Pll)
    {
        case PLL_MAIN: { RCC->PLLCFGR &= ~(1 << Output);     break; }
        case PLL_SAI1: { RCC->PLLSAI1CFGR &= ~(1 << Output); break; }
        case PLL_SAI2: { RCC->PLLSAI2CFGR &= ~(1 << Output); break; }
        default:       { Rc = CC_INVALID_CFG;                break; }
    }
    return Rc;
}



ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllR(ClkCtrl_PllEnum Pll, ClkCtrl_PllREnum R)
{
    const U8 Divisor = ClkCtrl_RToDivisor(R);
    if (Divisor < 2) { return CC_INVALID_CFG; }

    ClkCtrl_ReturnCodeEnum Rc = CC_OK;
    U32 TempRFreq_Hz;

    switch (Pll)
    {
        case PLL_MAIN:
        {
            TempRFreq_Hz = ClockTree.MainPllVcoOutFreq_Hz / R;
            if (TempRFreq_Hz <= PLLR_OUT_FREQ_MAX_Hz)
            {
                RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR_Msk;
                RCC->PLLCFGR |= (R << RCC_PLLCFGR_PLLR_Pos);
                ClockTree.MainPllRFreq_Hz = TempRFreq_Hz;
            }
            else
            {
                Rc = CC_FREQ_OUT_OF_RANGE;
            }
            break;
        }
        case PLL_SAI1:
        {
            TempRFreq_Hz = ClockTree.Pllsai1VcoOutFreq_Hz / R;
            if (TempRFreq_Hz <= PLLR_OUT_FREQ_MAX_Hz)
            {
                RCC->PLLSAI1CFGR &= ~RCC_PLLSAI1CFGR_PLLSAI1R_Msk;
                RCC->PLLSAI1CFGR |= (R << RCC_PLLSAI1CFGR_PLLSAI1R_Pos);
                ClockTree.Pllsai1RFreq_Hz = TempRFreq_Hz;
            }
            else
            {
                Rc = CC_FREQ_OUT_OF_RANGE;
            }
            break;
        }
        case PLL_SAI2:
        {
            TempRFreq_Hz = ClockTree.Pllsai2VcoOutFreq_Hz / R;
            if (TempRFreq_Hz <= PLLR_OUT_FREQ_MAX_Hz)
            {
                RCC->PLLSAI2CFGR &= ~RCC_PLLSAI2CFGR_PLLSAI2R_Msk;
                RCC->PLLSAI2CFGR |= (R << RCC_PLLSAI2CFGR_PLLSAI2R_Pos);
                ClockTree.Pllsai2PFreq_Hz = TempRFreq_Hz;
            }
            else
            {
                Rc = CC_FREQ_OUT_OF_RANGE;
            }
            break;
        }
        default:
        {
            Rc = CC_INVALID_CFG;
            break;
        }
    }

    return Rc;
}


ClkCtrl_ReturnCodeEnum ClkCtrl_SetAbhPrescaler(ClkCtrl_AhbPrescalerEnum Prescaler)
{
    const S16 Divisor = ClkCtrl_AhbPrescalerToDivisor(Prescaler);
    if (Divisor < 1) { return CC_INVALID_CFG; }
    if (ClockTree.AhbPrescalerLocked) { return CC_INVALID_SEQUENCE; }

    RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
    if (Prescaler != AHB_PS_1)
    {
        RCC->CFGR |= (Prescaler << RCC_CFGR_HPRE_Pos);
    }

    ClockTree.HclkFreq_Hz = ClockTree.SysclkFreq_Hz / Divisor;
    ClockTree.AhbPrescalerLocked = true;
    return CC_OK;
}


ClkCtrl_ReturnCodeEnum ClkCtrl_SetApbPrescaler(ClkCtrl_ApbEnum Bus, ClkCtrl_ApbPrescalerEnum Prescaler)
{
    if (!ClockTree.AhbPrescalerLocked) { return CC_INVALID_SEQUENCE; }
    const S16 Divisor = ClkCtrl_ApbPrescalerToDivisor(Prescaler);
    if (Divisor < 1) { return CC_INVALID_CFG; }

    ClkCtrl_ReturnCodeEnum Rc = CC_OK;

    switch (Bus)
    {
        case APB1:
        {
            if (!ClockTree.Apb1PrescalerLocked)
            {
                RCC->CFGR &= ~RCC_CFGR_PPRE1_Msk;
                RCC->CFGR |= (Prescaler << RCC_CFGR_PPRE1_Pos);
                ClockTree.Pclk1Freq_Hz = ClockTree.HclkFreq_Hz / Divisor;
                ClockTree.Apb1PrescalerLocked = true;
            }
            else
            {
                Rc = CC_INVALID_SEQUENCE;
            }
            break;
        }
        case APB2:
        {
            if (!ClockTree.Apb2PrescalerLocked)
            {
                RCC->CFGR &= ~RCC_CFGR_PPRE2_Msk;
                RCC->CFGR |= (Prescaler << RCC_CFGR_PPRE2_Pos);
                ClockTree.Pclk2Freq_Hz = ClockTree.HclkFreq_Hz / Divisor;
                ClockTree.Apb2PrescalerLocked = true;
            }
            else
            {
                Rc = CC_INVALID_SEQUENCE;
            }
            break;
        }
        default:
        {
            Rc = CC_INVALID_CFG;
            break;
        }
    }

    return Rc;
}


ClkCtrl_ReturnCodeEnum ClkCtrl_SetMsiFreqRange(ClkCtrl_MsiFreqRangeEnum FreqRange)
{
    const U32 TargetFreq_Hz = ClkCtrl_MsiFreqRangeToHz(FreqRange);
    if (TargetFreq_Hz == 0UL) { return CC_INVALID_CFG; }

    if (ClkCtrl_MsiEnabled()) { ClkCtrl_MsiDisable(); }
    RCC->CR |= (1 << RCC_CR_MSIRGSEL_Pos);
    RCC->CR &= ~RCC_CR_MSIRANGE_Msk;
    RCC->CR |= (FreqRange << RCC_CR_MSIRANGE_Pos);
    ClockTree.MsiFreq_Hz = TargetFreq_Hz;

    return CC_OK;
}


U32 ClkCtrl_GetNodeFreq(ClkCtrl_ClockTreeNodeEnum Node)
{
    U32 Rv;
    switch (Node)
    {
        case CLK_NODE_MSI:        { Rv = ClockTree.MsiFreq_Hz;      break; }
        case CLK_NODE_SYSCLK:     { Rv = ClockTree.SysclkFreq_Hz;   break; }
        case CLK_NODE_HCLK:       { Rv = ClockTree.HclkFreq_Hz;     break; }
        case CLK_NODE_PCLK1:      { Rv = ClockTree.Pclk1Freq_Hz;    break; }
        case CLK_NODE_PCLK2:      { Rv = ClockTree.Pclk2Freq_Hz;    break; }
        case CLK_NODE_MAIN_PLL_P: { Rv = ClockTree.MainPllPFreq_Hz; break; }
        case CLK_NODE_MAIN_PLL_Q: { Rv = ClockTree.MainPllQFreq_Hz; break; }
        case CLK_NODE_MAIN_PLL_R: { Rv = ClockTree.MainPllRFreq_Hz; break; }
        case CLK_NODE_PLL_SAI1_P: { Rv = ClockTree.Pllsai1PFreq_Hz; break; }
        case CLK_NODE_PLL_SAI1_Q: { Rv = ClockTree.Pllsai1QFreq_Hz; break; }
        case CLK_NODE_PLL_SAI1_R: { Rv = ClockTree.Pllsai1RFreq_Hz; break; }
        case CLK_NODE_PLL_SAI2_P: { Rv = ClockTree.Pllsai2PFreq_Hz; break; }
        case CLK_NODE_PLL_SAI2_R: { Rv = ClockTree.Pllsai2RFreq_Hz; break; }
        default:                  { Rv = 0;                         break; }
    }
    return Rv;
}


void ClkCtrl_PeripheralClockEnable(ClkCtrl_PeripheralClockEnum Peripheral)
{
    volatile U32* const Register = (volatile U32*)(RCC_BASE + ((Peripheral & 0xFF00) >> 8U));
    const U8 Bit = (U8)(Peripheral & 0xFFU);
    *Register |= (1 << Bit);
}


void ClkCtrl_PeripheralClockDisable(ClkCtrl_PeripheralClockEnum Peripheral)
{
    volatile U32* const Register = (volatile U32*)(RCC_BASE + ((Peripheral & 0xFF00) >> 8U));
    const U8 Bit = (U8)(Peripheral & 0xFFU);
    *Register &= ~(1 << Bit);
}
