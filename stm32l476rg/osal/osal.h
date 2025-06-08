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

/* --------------------- RTOS agnostic preprocessor definitions -------------------- */

#define OSAL_MAX_NOF_THREADS    (8U)

/* ---------------------- FreeRTOS specific OSAL functionality --------------------- */

#if defined(OSAL_CONFIG_USE_FREERTOS) && (OSAL_CONFIG_USE_FREERTOS == 1)

/* ---------------------- FreeRTOS specific include directives --------------------- */

#include "memory_routines.h"
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"

/* ----------------------- FreeRTOS specific type definitions ---------------------- */

typedef TaskFunction_t Osal_ThreadFunc;

/**
 * @brief Enumeration of thread priority levels.
 */
typedef enum
{
    THREAD_PRIORITY_LOWEST = 0,
    THREAD_PRIORITY_LOW = (configMAX_PRIORITIES - 11),
    THREAD_PRIORITY_MEDIUM = (configMAX_PRIORITIES - 7),
    THREAD_PRIORITY_HIGH = (configMAX_PRIORITIES - 4),
    THREAD_PRIORITY_HIGHEST = (configMAX_PRIORITIES - 1),
} Osal_PriorityEnum;

/* ----------------------- FreeRTOS specific inline functions ---------------------- */

/**
 * @brief Convert the given time in milliseconds to the corresponding
 *        number of OS ticks.
 * @param Milliseconds Time in milliseconds.
 * @return Time in OS ticks.
 */
static inline U32 Osal_msToTicks(U32 Milliseconds)
{
    #if defined(configTICK_RATE_HZ) && (configTICK_RATE_HZ == 1000U)
        return Milliseconds;
    #else
        return (U32)pdMS_TO_TICKS(Milliseconds);
    #endif
}

/**
 * @brief Convert the given number of OS ticks into the corresponding time
 *        in milliseconds.
 * @param Ticks Number of ticks.
 * @return Time in milliseconds.
 */
static inline U32 Osal_msFromTicks(U32 Ticks)
{
    #if defined(configTICK_RATE_HZ) && (configTICK_RATE_HZ == 1000U)
        return Ticks;
    #else
        return (U32)pdTICKS_TO_MS((TickType_t)Ticks);
    #endif
}

#endif /* FreeRTOS specific */

/* ----------------------- RTOS agnostic function prototypes ----------------------- */

/**
 * @brief Delay the current thread of execution.
 * @param Delay Delay time in milliseconds.
 */
void Osal_Delay_ms(U32 Delay);

/**
 * @brief Start the scheduler.
 */
void Osal_StartScheduler(void);

/**
 * @brief Create thread.
 * @param Func Thread function.
 * @param Arg Argument passed to thread function. Can be NULL.
 * @param StackSize Size of thread stack in bytes.
 * @param Priority Thread priority.
 * @return ID assigned to thread. A value of -1 indicates thread creation failed.
 */
S32 Osal_ThreadCreate(Osal_ThreadFunc Func, void* Arg, U32 StackSize, Osal_PriorityEnum Priority);

#endif /* OSAL_H */