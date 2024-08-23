/**
 * @file systick.h
 *
 * @brief System timer
 */

#ifndef SYSTICK_H
#define SYSTICK_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"

/*  ---------------------------Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of the valid clock sources for the Cortex system timer.
 */
typedef enum
{
    SYSTICK_CLK_SRC_AHB_DIV_8 = 0,
    SYSTICK_CLK_SRC_AHB = 1,
} SysTick_ClockSourceEnum;

/**
 * @brief Configuration structure for the Cortex system timer.
 */
typedef struct
{
    U32 ReloadVal;
    SysTick_ClockSourceEnum ClockSource;
} SysTick_ConfigType;

/* ------------------------ External function declarations ------------------------- */

/**
 * @brief Initialize Cortex system timer
 * @param Config Pointer to timer configuration structure
 */
void SysTick_Init(const SysTick_ConfigType* Config);

/**
 * @brief Get the number of ticks that has passed since last reset.
 * @return Number of ticks since reset.
 */
U64 SysTick_GetTicks(void);

/* -------------------------- Inline function definitions -------------------------- */

/**
 * @brief Get default configuration structure for Cortex system timer.
 *        This configuration generates an interrupt with a frequency of 1 kHz.
 * @return Default configuration structure.
 */
static inline SysTick_ConfigType SysTick_GetDefaultConfig(void)
{
    return (SysTick_ConfigType) { .ReloadVal = 9999, .ClockSource = SYSTICK_CLK_SRC_AHB_DIV_8 };
}

#endif  /* SYSTICK_H */