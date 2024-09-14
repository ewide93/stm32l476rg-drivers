/**
 * @file clock_control.h
 *
 * @brief Low level module for configuration & control of MCU clocks.
 *
 * @note Known limitations & not implemented features:
 *          * Missing ability to control multiplexers of
 *            various peripheral clocks.
 */

#ifndef CLOCK_CONTROL_H
#define CLOCK_CONTROL_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"

/* -------------------------------- Macro definitions ------------------------------ */
// #define LSE_NOT_INSTALLED
#define LSE_Freq_Hz             (32768UL)       /* 32.768 kHz */
#define HSE_NOT_INSTALLED
#define HSE_FREQ_Hz             (0UL)
#define LSI_FREQ_Hz             (32000UL)       /* 32 kHz     */
#define HSI_FREQ_Hz             (16000000UL)    /* 16 MHz     */
#define MSI_INIT_FREQ_Hz        (4000000UL)     /* 4 MHz      */

#define VCO_IN_FREQ_MIN_Hz      (4000000UL)     /* 4 MHz      */
#define VCO_IN_FREQ_MAX_Hz      (16000000UL)    /* 16 MHz     */
#define VCO_OUT_FREQ_MIN_Hz     (64000000UL)    /* 64 MHz     */
#define VCO_OUT_FREQ_MAX_Hz     (344000000UL)   /* 344 MHz    */

#define PLLQ_OUT_FREQ_MAX_Hz    (80000000UL)    /* 80 MHz     */
#define PLLR_OUT_FREQ_MAX_Hz    (80000000UL)    /* 80 MHz     */

#define PLL_N_MIN               (8U)
#define PLL_N_MAX               (86U)
#define PLL_P_MIN               (2U)
#define PLL_P_MAX               (31U)

/*  ---------------------------Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of return codes used in clock control module.
 */
typedef enum
{
    CC_OK,                  /* Ok */
    CC_INVALID_CFG,         /* Invalid argument passed to function */
    CC_FREQ_OUT_OF_RANGE,   /* The given configuration violates frequency specification */
    CC_INVALID_SEQUENCE     /* The function calling sequence is incorrect */
} ClkCtrl_ReturnCodeEnum;

/**
 * @brief Enumeration of available clock sources.
 */
typedef enum
{
    CLK_SRC_LSI,        /* Low speed internal RC oscillator (32 kHz). */
    CLK_SRC_MSI,        /* Multi-speed internal RC oscillator (100 kHz - 48 MHz). */
    CLK_SRC_HSI,        /* High speed internal RC oscillator (16 MHz). */
    CLK_SRC_LSE,        /* Low speed external crystal/ceramic resonator (32.768 kHz) . */
    CLK_SRC_HSE,        /* High speed external crystal/ceramic resonator. */
} ClkCtrl_ClockSourceEnum;


/**
 * @brief Enumeration of available PLLs.
 */
typedef enum
{
    PLL_MAIN = 7U,
    PLL_SAI1 = 5U,
    PLL_SAI2 = 3U,
} ClkCtrl_PllEnum;


/**
 * @brief Enumeration of available clock sources for the PLLs.
 */
typedef enum
{
    PLL_INPUT_NONE = 0x0U,
    PLL_INPUT_MSI = 0x1U,
    PLL_INPUT_HSI = 0x2U,
    PLL_INPUT_HSE = 0x3U,
} ClkCtrl_PllInputEnum;


/**
 * @brief Enumeration of the available PLL outputs.
 */
typedef enum
{
    PLL_OUTPUT_P = 16U,
    PLL_OUTPUT_Q = 20U,
    PLL_OUTPUT_R = 24U
} ClkCtrl_PllOutputEnum;


/**
 * @brief Enumeration of the available setting for the PLL VCO clock
 *        input divider M.
 * @note f,VCO in = f,x / M where x = {MSI, HSI, HSE}
 * @note 4 MHZ <= f,VCO in <= 16 MHz
 */
typedef enum
{
    PLL_M_1 = 0x0U,
    PLL_M_2 = 0x1U,
    PLL_M_3 = 0x2U,
    PLL_M_4 = 0x3U,
    PLL_M_5 = 0x4U,
    PLL_M_6 = 0x5U,
    PLL_M_7 = 0x6U,
    PLL_M_8 = 0x7U,
} ClkCtrl_PllMEnum;


/**
 * @brief Enumeration of the available settings for the output
 *        divider of PLL output Q.
 * @note Output Q only available on main PLL and PLLSAI1.
 */
typedef enum
{
    PLL_Q_2 = 0x0U,
    PLL_Q_4 = 0x1U,
    PLL_Q_6 = 0x2U,
    PLL_Q_8 = 0x3U,
} ClkCtrl_PllQEnum;


/**
 * @brief Enumeration of the available settings for the output
 *        divider of PLL output R.
 */
typedef enum
{
    PLL_R_2 = 0x0U,
    PLL_R_4 = 0x1U,
    PLL_R_6 = 0x2U,
    PLL_R_8 = 0x3U,
} ClkCtrl_PllREnum;


/**
 * @brief Enumeration of available clock inputs for SYSCLK.
 */
typedef enum
{
    SYSCLK_INPUT_MSI = 0x0U,
    SYSCLK_INPUT_HSI = 0x1U,
    SYSCLK_INPUT_HSE = 0x2U,
    SYSCLK_INPUT_PLL = 0x3U
} ClkCtrl_SysclkInputEnum;


/**
 * @brief Enumeration of available settings for the AHB prescaler.
 * @note f,HCLK = f,SYSCLK / AHB prescaler
 */
typedef enum
{
    AHB_PS_1 = 0x0U,
    AHB_PS_2 = 0x8U,
    AHB_PS_4 = 0x9U,
    AHB_PS_8 = 0xAU,
    AHB_PS_16 = 0xBU,
    AHB_PS_64 = 0xCU,
    AHB_PS_128 = 0xDU,
    AHB_PS_256 = 0xEU,
    AHB_PS_512 = 0xFU,
} ClkCtrl_AhbPrescalerEnum;


/**
 * @brief Enumeration of available APB buses.
 */
typedef enum
{
    APB1 = 8U,
    APB2 = 11U
} ClkCtrl_ApbEnum;

/**
 * @brief Enumeration of available settings for the APB prescaler.
 * @note f,PCLKx = f,SYSCLK / APBx prescaler where x = {1, 2}
 */
typedef enum
{
    APB_PS_1 = 0x0U,
    APB_PS_2 = 0x4U,
    APB_PS_4 = 0x5U,
    APB_PS_8 = 0x6U,
    APB_PS_16 = 0x7U,
} ClkCtrl_ApbPrescalerEnum;

/**
 * @brief Enumeration of available output frequency settings for
 *        the MSI oscillator [Hz].
 */
typedef enum
{
    MSI_FREQ_100_kHz = 0x0U,
    MSI_FREQ_200_kHz = 0x1U,
    MSI_FREQ_400_kHz = 0x2U,
    MSI_FREQ_800_kHz = 0x3U,
    MSI_FREQ_1_MHz = 0x4U,
    MSI_FREQ_2_MHz = 0x5U,
    MSI_FREQ_4_MHz = 0x6U,
    MSI_FREQ_8_MHz = 0x7U,
    MSI_FREQ_16_MHz = 0x8U,
    MSI_FREQ_24_MHz = 0x9U,
    MSI_FREQ_32_MHz = 0xAU,
    MSI_FREQ_48_MHz = 0xBU,
} ClkCtrl_MsiFreqRangeEnum;

/**
 * @brief Enumeration of the nodes of interest in the clock tree.
 */
typedef enum
{
    CLK_NODE_MSI,
    CLK_NODE_SYSCLK,
    CLK_NODE_HCLK,
    CLK_NODE_PCLK1,
    CLK_NODE_PCLK2,
    CLK_NODE_MAIN_PLL_P,
    CLK_NODE_MAIN_PLL_Q,
    CLK_NODE_MAIN_PLL_R,
    CLK_NODE_PLL_SAI1_P,
    CLK_NODE_PLL_SAI1_Q,
    CLK_NODE_PLL_SAI1_R,
    CLK_NODE_PLL_SAI2_P,
    CLK_NODE_PLL_SAI2_R,
} ClkCtrl_ClockTreeNodeEnum;


/**
 * @brief Enumeration of available peripheral clocks. The MSB of the
 *        chosen values for the enumerations correspond to the address
 *        offset from the RCC base register (0x4002 1000). The LSB corresponds to
 *        the bit position in the clock enable register.
 */
typedef enum
{
    PCLK_DMA1 = 0x4800U,
    PCLK_DMA2 = 0x4801U,
    PCLK_FLASH = 0x4808U,
    PCLK_CRC = 0x480CU,
    PCLK_TSC = 0x4810U,
    PCLK_GPIOA = 0x4C00U,
    PCLK_GPIOB = 0x4C01U,
    PCLK_GPIOC = 0x4C02U,
    PCLK_GPIOD = 0x4C03U,
    PCLK_GPIOE = 0x4C04U,
    PCLK_GPIOF = 0x4C05U,
    PCLK_GPIOG = 0x4C06U,
    PCLK_GPIOH = 0x4C07U,
    PCLK_OTGFS = 0x4C0CU,
    PCLK_ADC = 0x4C0DU,
    PCLK_RNG = 0x4C12U,
    PCLK_FMC = 0x5000U,
    PCLK_QPSI = 0x5008U,
    PCLK_TIM2 = 0x5800U,
    PCLK_TIM3 = 0x5801U,
    PCLK_TIM4 = 0x5802U,
    PCLK_TIM5 = 0x5803U,
    PCLK_TIM6 = 0x5804U,
    PCLK_TIM7 = 0x5805U,
    PCLK_LCD = 0x5809U,
    PCLK_WWDG = 0x580BU,
    PCLK_SPI2 = 0x580EU,
    PCLK_SPI3 = 0x580FU,
    PCLK_USART2 = 0x5811U,
    PCLK_USART3 = 0x5812U,
    PCLK_UART4 = 0x5813U,
    PCLK_UART5 = 0x5814U,
    PCLK_I2C1 = 0x5815U,
    PCLK_I2C2 = 0x5816U,
    PCLK_I2C3 = 0x5817U,
    PCLK_CAN1 = 0x5819U,
    PCLK_PWR = 0x581CU,
    PCLK_DAC1 = 0x581DU,
    PCLK_OPAMP = 0x581EU,
    PCLK_LPTIM1 = 0x581FU,
    PCLK_LPUART1 = 0x5C00U,
    PCLK_I2C4 = 0x5C01U,
    PCLK_LPTIM2 = 0x5C05U,
    PCLK_SYSCFG = 0x6000U,
    PCLK_FW = 0x6007U,
    PCLK_SDMMC1 = 0x600AU,
    PCLK_TIM1 = 0x600BU,
    PCLK_SPI1 = 0x600CU,
    PCLK_TIM8 = 0x600DU,
    PCLK_USART1 = 0x600EU,
    PCLK_TIM15 = 0x6010U,
    PCLK_TIM16 = 0x6011U,
    PCLK_TIM17 = 0x6012U,
    PCLK_SAI1 = 0x6015U,
    PCLK_SAI2 = 0x6016U,
    PCLK_DFSDM1 = 0x6018U,
} ClkCtrl_PeripheralClockEnum;

/* -------------------------- Public function definitions -------------------------- */

/**
 * @brief Await the ready flag for the given PLL.
 * @param PLL The PLL circuit of interest.
 */
static inline void ClkCtrl_AwaitPllReady(ClkCtrl_PllEnum Pll)
{
    while ( !(RCC->CR & (1 << (32U - Pll))) ) { __NOP(); }
}


/**
 * @brief Enable the given PLL.
 * @param Pll The PLL to be enabled.
 * @param AwaitReadyFlag Set to True to await the ready flag after
 *                       enabling the PLL.
 */
static inline void ClkCtrl_PllEnable(ClkCtrl_PllEnum Pll, Bool AwaitReadyFlag)
{
    RCC->CR |= (1 << (31U - Pll));
    if (AwaitReadyFlag) { ClkCtrl_AwaitPllReady(Pll); }
}


/**
 * @brief Disable the given PLL.
 * @param Pll The PLL to be disabled.
 */
static inline void ClkCtrl_PllDisable(ClkCtrl_PllEnum Pll)
{
    RCC->CR &= ~(1 << (31U - Pll));
}


/**
 * @brief Read status of the given PLL.
 * @param Pll The PLL of interest.
 * @return True if enabled, False if disabled.
 */
static inline Bool ClkCtrl_PllEnabled(ClkCtrl_PllEnum Pll)
{
    return RCC->CR & (1 << (31U - Pll));
}


/**
 * @brief Await the ready flag for the MSI oscillator.
 */
static inline void ClkCtrl_AwaitMsiReady(void)
{
    while ( !(RCC->CR & RCC_CR_MSIRDY) ) { __NOP(); }
}


/**
 * @brief Enable the MSI oscillator.
 * @param AwaitReadyFlag Set to True to await the ready flag after enabling.
 */
static inline void ClkCtrl_MsiEnable(Bool AwaitReadyFlag)
{
    RCC->CR |= RCC_CR_MSION;
    if (AwaitReadyFlag) { ClkCtrl_AwaitMsiReady(); }
}


/**
 * @brief Disable the MSI oscillator.
 */
static inline void ClkCtrl_MsiDisable(void)
{
    RCC->CR &= ~RCC_CR_MSION;
}


/**
 * @brief Read the status of the MSI oscillator.
 * @returns True if enabled, False if disabled.
 */
static inline Bool ClkCtrl_MsiEnabled(void)
{
    return RCC->CR & RCC_CR_MSION;
}


/**
 * @brief Await the ready flag for the HSI oscillator.
 */
static inline void ClkCtrl_AwaitHsiReady(void)
{
    while ( !(RCC->CR & RCC_CR_HSIRDY) ) { __NOP(); }
}


/**
 * @brief Enable the HSI oscillator.
 * @param AwaitReadyFlag Set to tru to await the ready flag after enabling.
 */
static inline void ClkCtrl_HsiEnable(Bool AwaitReadyFlag)
{
    RCC->CR |= RCC_CR_HSION;
    if (AwaitReadyFlag) { ClkCtrl_AwaitHsiReady(); }
}


/**
 * @brief Disable the HSI oscillator.
 */
static inline void ClkCtrl_HsiDisable(void)
{
    RCC->CR &= ~RCC_CR_HSION;
}


/**
 * @brief Read the status of the HSI oscillator.
 * @returns True if enabled, False if disabled.
 */
static inline Bool ClkCtrl_HsiEnabled(void)
{
    return RCC->CR & RCC_CR_HSION;
}

/* -------------------------- Public function prototypes --------------------------- */

/**
 * @brief Select clock source for SYSCLK.
 * @param Input Clock source for SYSCLK.
 */
void ClkCtrl_SetSysclkInput(ClkCtrl_SysclkInputEnum Input);


/**
 * @brief Configure the input section of the PLL block.
 * @param Input Clock source for the PLL block.
 * @param M Input clock divider.
 * @return Return code indicating result of the operation.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllInput(ClkCtrl_PllInputEnum Input, ClkCtrl_PllMEnum M);


/**
 * @brief Configure the frequency multiplication factor of the VCO
 *        for the given PLL circuit.
 * @param Pll The PLL circuit to be configured.
 * @param N Multiplication factor for VCO.
 * @return Return code indicating result of the operation.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllN(ClkCtrl_PllEnum Pll, U8 N);


/**
 * @brief Configure the output clock divider for clock output P
 *        of the given PLL circuit.
 * @param Pll The PLL circuit to be configured.
 * @param P VCO output divider P.
 * @return Return code indicating result of the operation.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllP(ClkCtrl_PllEnum Pll, U8 P);


/**
 * @brief Configure the output clock divider for clock output Q
 *        of the given PLL circuit.
 * @param Pll The PLL circuit to be configured.
 * @param Q VCO output divider Q.
 * @return Return code indicating result of the operation.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllQ(ClkCtrl_PllEnum Pll, ClkCtrl_PllQEnum Q);


/**
 * @brief Configure the output clock divider for clock output R
 *        of the given PLL circuit.
 * @param Pll The PLL circuit to be configured.
 * @param R VCO output divider R.
 * @return Return code indicating result of the operation.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_SetPllR(ClkCtrl_PllEnum Pll, ClkCtrl_PllREnum R);


/**
 * @brief Enable the given output of the given PLL circuit.
 * @param Pll The PLL circuit of interest.
 * @param Output The PLL output of interest.
 * @return Return code indicating result of the operation.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_PllOutputEnable(ClkCtrl_PllEnum Pll, ClkCtrl_PllOutputEnum Output);


/**
 * @brief Disable the given output of the given PLL circuit.
 * @param Pll The PLL circuit of interest.
 * @param Output The PLL output of interest.
 * @return Return code indicating result of the operation.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_PllOutputDisable(ClkCtrl_PllEnum Pll, ClkCtrl_PllOutputEnum Output);


/**
 * @brief Set the clock divider for the Advanced High-performance Bus.
 * @param Prescaler AHB prescaler
 * @return Return code indicating result of the operation.
 * @note After the first call of this function further calls
 *       will do nothing.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_SetAhbPrescaler(ClkCtrl_AhbPrescalerEnum Prescaler);


/**
 * Set the prescaler for the given Advanced Peripheral Bus.
 * @param Bus The APB of interest.
 * @param Prescaler Bus clock prescaler.
 * @return Return code indicating result of the operation.
 * @note The prescaler for the AHB needs to be configured before
 *       attempting to set the prescalers for the APBs.
 * @note Repeated calls to this function with the same APB as
 *       argument will do nothing.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_SetApbPrescaler(ClkCtrl_ApbEnum Bus, ClkCtrl_ApbPrescalerEnum Prescaler);


/**
 * @brief Set the operating frequency of the MSI oscillator.
 * @param FreqRange Desired operating frequency [Hz].
 * @return Return code indicating result of the operation.
 */
ClkCtrl_ReturnCodeEnum ClkCtrl_SetMsiFreqRange(ClkCtrl_MsiFreqRangeEnum FreqRange);


/**
 * @brief Get the current frequency of the given node in
 *        the clock tree.
 * @param Node Clock tree node.
 * @return Frequency in Hertz.
 */
U32 ClkCtrl_GetNodeFreq(ClkCtrl_ClockTreeNodeEnum Node);


/**
 * @brief Enable the clock for the given peripheral.
 * @param Peripheral Peripheral of interest.
 */
void ClkCtrl_PeripheralClockEnable(ClkCtrl_PeripheralClockEnum Peripheral);


/**
 * @brief Disable the clock for the given peripheral.
 * @param Peripheral Peripheral of interest.
 */
void ClkCtrl_PeripheralClockDisable(ClkCtrl_PeripheralClockEnum Peripheral);

#endif /* CLOCK_CONTROL_H */
