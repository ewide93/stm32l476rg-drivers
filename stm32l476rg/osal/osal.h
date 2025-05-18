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
#endif /* OSAL conflicting configurations. */

#if (OSAL_CONFIG_USE_FREERTOS == 0) && (OSAL_CONFIG_USE_BARE_METAL == 0)
    #error "Either OSAL_CONFIG_USE_FREERTOS or OSAL_CONFIG_USE_BARE_METAL must be set."
#endif /* OSAL not configured. */

/* --------------------------- General include directives -------------------------- */

#include "typedef.h"

/* --------------------------- RTOS-agnostic OSAL settings ------------------------- */

/**
 * @brief Set the number of bytes to reserve for RTOS threads.
 */
#define OSAL_STATIC_MEMORY_SIZE_BYTES (4096U)

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

#include "port.h"
#include "FreeRTOS.h"
#include "task.h"

typedef TickType_t Osal_TickType;
#define OSAL_WORD_SIZE  (sizeof(StackType_t))

/* ----------------------- FreeRTOS specific inline functions ---------------------- */

/**
 * @brief Convert the given time in milliseconds to the corresponding
 *        number of OS ticks.
 * @param Milliseconds Time in milliseconds.
 * @return Time in OS ticks.
 */
static inline Osal_TickType Osal_msToTicks(U32 Milliseconds)
{
    #if defined(configTICK_RATE_HZ) && (configTICK_RATE_HZ == 1000U)
        return (Osal_TickType)Milliseconds;
    #else
        return (Osal_TickType)pdMS_TO_TICKS(Milliseconds);
    #endif
}

/**
 * @brief Convert the given number of OS ticks into the corresponding time
 *        in milliseconds.
 * @param Ticks Number of ticks.
 * @return Time in milliseconds.
 */
static inline U32 Osal_msFromTicks(Osal_TickType Ticks)
{
    #if defined(configTICK_RATE_HZ) && (configTICK_RATE_HZ == 1000U)
        return Ticks;
    #else
        return (U32)pdTICKS_TO_MS(Ticks);
    #endif
}

/* --------------------- FreeRTOS specific function prototypes --------------------- */

/**
 * @brief
 */
void Osal_Delay_ms(U32 Delay);


#endif /* FreeRTOS specific */

#endif /* OSAL_H */