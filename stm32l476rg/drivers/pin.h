/**
 * @file pin.h
 *
 * @brief GPIO configuration.
 */

#ifndef PIN_H
#define PIN_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"

/*  ---------------------------Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of MCU pins of the STM32L476RG.
 */
typedef enum
{
    PIN_A0 = 0xA0U,
    PIN_A1 = 0xA1U,
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

/**
 * @brief Enumeration of I/O modes.
 */
typedef enum
{
    PIN_MODE_INPUT,        /* Input */
    PIN_MODE_OUTPUT,       /* Output */
    PIN_MODE_AF,           /* Alternate function */
    PIN_MODE_ANALOG,       /* Analog */
    PIN_MODE_ENUM_LIM
} Pin_ModeEnum;

/**
 * @brief Enumeration of output types.
 */
typedef enum
{
    PIN_OUT_TYPE_PUSH_PULL,     /* Push-pull */
    PIN_OUT_TYPE_OPEN_DRAIN,    /* Open drain */
    PIN_OUT_TYPE_ENUM_LIM
} Pin_OutputTypeEnum;

/**
 * @brief Enumeration of output rise and fall times.
 * @note Bandwidth approximations calculated by: 0.35 / Rise time @ Vddio 3.3V
 */
typedef enum
{
    PIN_SPEED_LOW,          /* Rise/fall time = 17ns. BW = 20.6 MHz */
    PIN_SPEED_MEDIUM,       /* Rise/fall time = 4.5ns, BW = 77.8 MHz */
    PIN_SPEED_HIGH,         /* Rise/fall time = 2.5ns, BW = 140 MHz */
    PIN_SPEED_VERY_HIGH,    /* Rise/fall time 3.3ns, BW = 106.1 MHz */
    PIN_SPEED_ENUM_LIM
} Pin_SpeedEnum;

/**
 * @brief Enumeration of pin settings for pull-up/down resistors.
 */
typedef enum
{
    PIN_RES_NONE,          /* No pull-up/down resustor */
    PIN_RES_PULL_UP,       /* Pull-up resistor */
    PIN_RES_PULL_DOWN,     /* Pull-down resistor */
    PIN_RES_ENUM_LIM
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
    PIN_AF_ENUM_LIM
} Pin_AlternateFunctionEnum;

/**
 * @brief Enumeration indicating pin connection to ADC.
 */
typedef enum
{
    PIN_ANALOG_NOT_CONNECTED,       /* Pin is not connected to ADC circuitry */
    PIN_ANALOG_CONNECTED,           /* Pin is connected to ADC circuitry */
    PIN_ANALOG_ENUM_LIM
} Pin_AnalogEnum;

/**
 * @brief Pin configuration structure.
 */
typedef struct
{
    Pin_ModeEnum Mode;                  /* Pin mode */
    Pin_OutputTypeEnum OutputType;      /* Output circuitry type */
    Pin_SpeedEnum Speed;                /* Output dv/dt */
    Pin_ResistorEnum Resistor;          /* Pull-up/down resistor settings */
    Pin_AlternateFunctionEnum AltFunc;  /* Alternate function selection */
    Pin_AnalogEnum Analog;              /* Analog/digital type */
} Pin_ConfigType;

/**
 * @brief GPIO-pin structure.
 */
typedef struct
{
    Pin_ConfigType Config;              /* Configuration structure */
    GPIO_TypeDef* Port;                 /* I/O port */
    U32 Pin;                            /* Pin number */
} Pin_PinType;

/* ------------------------ External function declarations ------------------------- */

/**
 * @brief Initialize GPIO pins in accordance with configuration table.
 */
void Pin_Init(void);

/**
 * @brief Get the GPIO port based on the given port & pin enumeration.
 * @param PortPin Port & pin
 * @returns Pointer to GPIO port structure.
 */
GPIO_TypeDef* Pin_GetPort(Pin_PortPinEnum PortPin);

#endif /* PIN_H */