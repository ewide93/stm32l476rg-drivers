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
#define OSAL_MAX_NOF_MUTEXES    (16U)

/* ---------------------- FreeRTOS specific OSAL functionality --------------------- */

#if defined(OSAL_CONFIG_USE_FREERTOS) && (OSAL_CONFIG_USE_FREERTOS == 1)

/* ---------------------- FreeRTOS specific include directives --------------------- */

#include "memory_routines.h"
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ----------------------- FreeRTOS specific type definitions ---------------------- */

typedef TaskFunction_t Osal_ThreadFunc;
typedef SemaphoreHandle_t Osal_MutexHandleType;


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
 * @brief Delay thread execution until the given time increment has elapsed.
 * @param LastWakeTime Tick count when execution of this thread was last resumed.
 * @param TimeIncrement Time increment in OS ticks.
 */
void Osal_DelayUntil(U32* const LastWakeTime, U32 TimeIncrement);

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

/**
 * @brief Suspend execution of the thread with the given ID.
 * @param Id Thread ID returned from Osal_ThreadCreate.
 */
void Osal_ThreadSuspend(S32 Id);

/**
 * @brief Resume execution of a thread that has been suspended using
 *        Osal_ThreadSuspend.
 * @param Id Thread ID returned from Osal_ThreadCreate.
 */
void Osal_ThreadResume(S32 Id);

/**
 * @brief Get the OS tick counter value.
 * @return Current OS tick count.
 * @note Only use outside of interrupt context.
 */
U32 Osal_GetTickCount(void);

/**
 * @brief Get the OS tick counter value from interrupt context.
 * @return Current OS tick count.
 * @note Only use inside of interrupt context.
 */
U32 Osal_GetTickCount_ISR(void);

/**
 * @brief Attempt to claim a handle to a statically allocated mutex.
 * @return Mutex handle used for OS mutex API, returns NULL if the operation failed.
 * @note Maximum number of mutexes dictated by preprocessor definition OSAL_MAX_NOF_MUTEXES.
 */
Osal_MutexHandleType Osal_MutexCreate(void);

/**
 * @brief Release a mutex previously claimed using Osal_MutexCreate back to the OS.
 * @param MutexHandle Handle to mutex.
 */
void Osal_MutexDestroy(Osal_MutexHandleType MutexHandle);

/**
 * @brief Get the number of statically allocated mutexes claimed from the OS.
 * @return Number of claimed mutexes.
 */
U8 Osal_GetNofClaimedMutexes(void);

/**
 * @brief Get the number of statically allocated mutexes that are left to claim from the OS.
 * @return Number of remaining mutexes.
 */
U8 Osal_GetNofFreeMutexes(void);

/**
 * @brief Aquire the mutex of the given mutex handle.
 * @param MutexHandle Handle to mutex.
 * @return True = success, False = failure
 */
Bool Osal_MutexAquire(Osal_MutexHandleType MutexHandle);

/**
 * @brief Release the mutex of the given mutex handle.
 * @param MutexHandle Handle to mutex.
 * @return True = success, False = failure
 */
Bool Osal_MutexRelease(Osal_MutexHandleType MutexHandle);

#endif /* OSAL_H */
