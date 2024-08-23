/**
 * @file clock_control.h
 *
 * @brief Low level module for configuration & control of MCU clocks.
 */

#ifndef CLOCK_CONTROL_H
#define CLOCK_CONTROL_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"

/*  ---------------------------Structures & enumerations --------------------------- */

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
} ClockSourceEnum;

/* ------------------------ External function declarations ------------------------- */


#endif /* CLOCK_CONTROL_H */