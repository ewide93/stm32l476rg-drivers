/**
 * @brief Operating System Abstraction Layer
 *
 *        Purpose: Provide application facing interface to RTOS.
 */

#ifndef OSAL_H
#define OSAL_H

/* ------------------------ Preprocessor configuration flags ----------------------- */

/**
 * @brief Set this define to "1" to use FreeRTOS
 */
#define OSAL_CONFIG_USE_FREERTOS    (1)

/**
 * @brief Set this define to "1" to run bare metal.
 */
#define OSAL_CONFIG_USE_BARE_METAL  (0)

#if (OSAL_CONFIG_USE_FREERTOS == 1) && (OSAL_CONFIG_USE_BARE_METAL == 1)
    #error "Conflicing configurations for OSAL_CONFIG_USE_FREERTOS and OSAL_CONFIG_USE_BARE_METAL"
#endif /* OSAL_CONFIG */

/* --------------------------- General include directives -------------------------- */

#include "typedef.h"
#include "port.h"

/* ---------------------- FreeRTOS specific OSAL functionality --------------------- */

/**
 * @todo List of stuff that needs to be done to get FreeRTOS running:
 *
 *       1. Check if configCHECK_FOR_STACK_OVERFLOW is greater than 0.
 *          If this is the case we need to provide a callback function
 *          that will be called upon detection of stack overflow.
 *
 *       2.
 */

#if defined(OSAL_CONFIG_USE_FREERTOS) && (OSAL_CONFIG_USE_FREERTOS == 1)

/* ---------------------- FreeRTOS specific include directives --------------------- */

#include "FreeRTOS.h"
#include "task.h"


#endif /* FreeRTOS specific */

#endif /* OSAL_H */