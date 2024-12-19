/**
 * @file pin.h
 *
 * @brief Pin configuration module.
 */

#ifndef PIN_H
#define PIN_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"

/*  --------------------------- Preprocessor definitions --------------------------- */
#define LQFP64

/*  -------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of MCU pins of the STM32L476RG in a LQFP64 package.
 */
#if defined(LQFP64)
typedef enum
{
    PIN_A0 = 0xA0U,
    PIN_A1 = 0xA1U,
    PIN_A2 = 0xA2U,
    PIN_A3 = 0xA3U,
    PIN_A4 = 0xA4U,
    PIN_A5 = 0xA5U,
    PIN_A6 = 0xA6U,
    PIN_A7 = 0xA7U,
    PIN_A8 = 0xA8U,
    PIN_A9 = 0xA9U,
    PIN_A10 = 0xAAU,
    PIN_A11 = 0xABU,
    PIN_A12 = 0xACU,
    PIN_A15 = 0xAFU,
    PIN_B0 = 0xB0U,
    PIN_B1 = 0xB1U,
    PIN_B2 = 0xB2U,
    PIN_B4 = 0xB4U,
    PIN_B5 = 0xB5U,
    PIN_B6 = 0xB6U,
    PIN_B7 = 0xB7U,
    PIN_B8 = 0xB8U,
    PIN_B9 = 0xB9U,
    PIN_B10 = 0xBAU,
    PIN_B11 = 0xBBU,
    PIN_B12 = 0xBCU,
    PIN_B13 = 0xBDU,
    PIN_B14 = 0xBEU,
    PIN_B15 = 0xBFU,
    PIN_C0 = 0xC0U,
    PIN_C1 = 0xC1U,
    PIN_C2 = 0xC2U,
    PIN_C3 = 0xC3U,
    PIN_C4 = 0xC4U,
    PIN_C5 = 0xC5U,
    PIN_C6 = 0xC6U,
    PIN_C7 = 0xC7U,
    PIN_C8 = 0xC8U,
    PIN_C9 = 0xC9U,
    PIN_C10 = 0xCAU,
    PIN_C11 = 0xCBU,
    PIN_C12 = 0xCCU,
    PIN_C13 = 0xCDU,
    PIN_D2 = 0xD2U,
    PIN_ENUM_LIM = 0xFFU
} Pin_PortPinEnum;
#endif /* LQFP64 */

/**
 * @brief Enumeration of I/O modes.
 */
typedef enum
{
    PIN_MODE_INPUT = 0x0U,        /* Input */
    PIN_MODE_OUTPUT = 0x1U,       /* Output */
    PIN_MODE_AF = 0x2U,           /* Alternate function */
    PIN_MODE_ANALOG = 0x3U        /* Analog */
} Pin_ModeEnum;

/**
 * @brief Enumeration of output types.
 */
typedef enum
{
    PIN_OUT_TYPE_PUSH_PULL = 0x0U,     /* Push-pull */
    PIN_OUT_TYPE_OPEN_DRAIN = 0x1U,    /* Open drain */
} Pin_OutputTypeEnum;

/**
 * @brief Enumeration of output rise and fall times.
 * @note Bandwidth approximations calculated by: 0.35 / Rise time @ Vddio 3.3V
 */
typedef enum
{
    PIN_SPEED_LOW = 0x0U,          /* Rise/fall time = 17ns. BW = 20.6 MHz */
    PIN_SPEED_MEDIUM = 0x1U,       /* Rise/fall time = 4.5ns, BW = 77.8 MHz */
    PIN_SPEED_HIGH = 0x2U,         /* Rise/fall time = 2.5ns, BW = 140 MHz */
    PIN_SPEED_VERY_HIGH = 0x3U,    /* Rise/fall time 3.3ns, BW = 106.1 MHz */
} Pin_SpeedEnum;

/**
 * @brief Enumeration of pin settings for pull-up/down resistors.
 */
typedef enum
{
    PIN_RES_NONE = 0x0U,          /* No pull-up/down resustor */
    PIN_RES_PULL_UP = 0x1U,       /* Pull-up resistor */
    PIN_RES_PULL_DOWN = 0x2U,     /* Pull-down resistor */
} Pin_ResistorEnum;

/**
 * @brief Enumeration of alternate pin functions.
 */
typedef enum
{
    PIN_AF0,        /* Default at reset, RTC & debug/trace functionality for some pins */
    PIN_AF1,        /* Timer 1, 2, 5 & 8, Low power timer 1 */
    PIN_AF2,        /* Timer 1, 2, 3, 4 & 5 */
    PIN_AF3,        /* Timer 8 */
    PIN_AF4,        /* I2C 1, 2 & 3 */
    PIN_AF5,        /* SPI 1 & 2 */
    PIN_AF6,        /* SPI 3 & DFDSM (Digital Filter for Delta Sigma Modulators) */
    PIN_AF7,        /* USART 1, 2 & 3 */
    PIN_AF8,        /* UART 4 & 5, Low power UART 1 */
    PIN_AF9,        /* CAN1 & TSC (Touch sensing controller) */
    PIN_AF10,       /* USB on-the-go full-speed & Quad SPI */
    PIN_AF11,       /* LCD (Liquid Crystal Display) */
    PIN_AF12,       /* SDMMC1 (SD and MMC interface), COMP 1 & 2, FMC (Flexible memory controller) & SWPMI1 (One-wire interface) */
    PIN_AF13,       /* SAI 1 & 2 (Serial audio interface) */
    PIN_AF14,       /* Timer 2, 15, 16, 17 & Low power timer 2 */
    PIN_AF15,       /* EVENTOUT, weird pulse thingy... */
} Pin_AlternateFunctionEnum;

/**
 * @brief Enumeration indicating pin connection to ADC.
 */
typedef enum
{
    PIN_ANALOG_NOT_CONNECTED,       /* Pin is not connected to ADC circuitry */
    PIN_ANALOG_CONNECTED,           /* Pin is connected to ADC circuitry */
} Pin_AnalogEnum;

/* ------------------------ External function declarations ------------------------- */

/**
 * @brief Configure the pin mode of the given pin.
 * @param PortPin Port & pin enumeration.
 * @param Mode Pin mode.
 */
void Pin_SetMode(Pin_PortPinEnum PortPin, Pin_ModeEnum Mode);

/**
 * @brief Configure the output type of the given pin.
 * @param PortPin Port & pin enumeration.
 * @param OutType Output type.
 */
void Pin_SetOutputType(Pin_PortPinEnum PortPin, Pin_OutputTypeEnum OutputType);

/**
 * @brief Configure the output speed of the given pin.
 * @param PortPin Port & pin enumeration.
 * @param Speed Rise/fall time.
 */
void Pin_SetSpeed(Pin_PortPinEnum PortPin, Pin_SpeedEnum Speed);

/**
 * @brief Configure the pull-up/down circuitry of the given pin.
 * @param PortPin Port & pin enumeration.
 * @param Resistor Configuration for pull-up/pull-down resistor.
 */
void Pin_SetResistor(Pin_PortPinEnum PortPin, Pin_ResistorEnum Resistor);

/**
 * @brief Configure the alternate function of the given pin.
 * @param PortPin Port & pin enumeration.
 * @param AltFunc Alternate pin function.
 */
void Pin_SetAltFunc(Pin_PortPinEnum PortPin, Pin_AlternateFunctionEnum AltFunc);

/**
 * @brief Configure the analog settings of the given pin.
 * @param PortPin Port & pin enumeration.
 * @param Analog Analog configuration.
 */
void Pin_SetAnalog(Pin_PortPinEnum PortPin, Pin_AnalogEnum Analog);

/**
 * @brief Set the output data register bit of the given pin.
 * @param PortPin Port & pin enumeration.
 */
void Pin_SetOutputData(Pin_PortPinEnum PortPin);

/**
 * @brief Clear the output data register bit of the given pin.
 * @param PortPin Port & pin enumeration.
 */
void Pin_ClearOutputData(Pin_PortPinEnum PortPin);

/**
 * @brief Toggle the output data register bit of the given pin.
 * @param PortPin Port & pin enumeration.
 */
void Pin_ToggleOutputData(Pin_PortPinEnum PortPin);

/**
 * @brief Read the input data register bit of the given pin.
 * @param PortPin Port & pin enumeration.
 * @return True = high, False = low
 */
Bool Pin_ReadInputData(Pin_PortPinEnum PortPin);

#endif /* PIN_H */
