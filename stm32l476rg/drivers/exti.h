/**
 * @file exti.h
 *
 * @brief External interrupt module.
 *
 */

#ifndef EXTI_H
#define EXTI_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"
#include "pin.h"

/*  --------------------------- Preprocessor definitions --------------------------- */
#define NOF_GPIO_EXTI   (16U)

/*  -------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of edge trigger options.
 */
typedef enum
{
    EXTI_TRIGGER_RISING_EDGE = 0x0U,
    EXTI_TRIGGER_FALLING_EDGE = 0x1U,
    EXTI_TRIGGER_ANY_EDGE = 0x2U
} Exti_EdgeTriggerEnum;

/**
 * @brief Enumeration of available GPIO connected external interrupts.
 */
typedef enum
{
    EXTI0 = 0U,
    EXTI1 = 1U,
    EXTI2 = 2U,
    EXTI3 = 3U,
    EXTI4 = 4U,
    EXTI5 = 5U,
    EXTI6 = 6U,
    EXTI7 = 7U,
    EXTI8 = 8U,
    EXTI9 = 9U,
    EXTI10 = 10U,
    EXTI11 = 11U,
    EXTI12 = 12U,
    EXTI13 = 13U,
    EXTI14 = 14U,
    EXTI15 = 15U
} Exti_GpioExtiEnum;

/* -------------------------- Public function declarations ------------------------- */

/**
 * @brief Configure external interrupt for the given GPIO pin.
 * @param PortPin GPIO port & pin.
 * @param Callback Callback function to be called when triggered.
 * @param EdgeTrigger Edge trigger configuration.
 */
void Exti_GpioInit(Pin_PortPinEnum PortPin, CallbackFunction Callback, Exti_EdgeTriggerEnum EdgeTrigger);


#endif /* EXTI_H */
