/**
 * @file core_debug.h
 *
 * @brief Arm Cortex M4 debug peripheral module.
 */

#ifndef CORE_DEBUG_H
#define CORE_DEBUG_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"
#include "stm32l4xx.h"

/* ---------------------------- Preprocessor definitions --------------------------- */

#define NOF_WATCHPOINT_COMPARATORS_MAX  (4U)
#define DEBUG_MONITOR_EXCEPTION_PRIO    (0U)

/* --------------------------- Structures & enumerations --------------------------- */

typedef void (*WatchpointCallback)(void);

/**
 * @brief Enumeration of status codes returned from functions interacting
 *        with the data watchpoint comparators.
 */
typedef enum
{
    WATCHPOINT_STATUS_OK = 0U,                  /* Ok */
    WATCHPOINT_STATUS_UNINITIALIZED = 1U,       /* Watchpoint comparator functions called before initializiation */
    WATCHPOINT_STATUS_WRONG_ALIGNMENT = 2U,     /* Address not aligned on 4-byte boundary */
    WATCHPOINT_STATUS_HW_LIMIT = 3U,            /* All comparators already in use */
    WATCHPOINT_STATUS_UNKNOWN_ADDRESS = 4U,     /* Address not assigned to any watchpoint */
} WatchpointComparatorStatusEnum;

/* -------------------------- Public function declarations ------------------------- */

/**
 * @brief Enable the processor cycle counter.
 */
void EnableCycleCounter(void);

/**
 * @brief Disable the processor cycle counter.
 */
void DisableCycleCounter(void);

/**
 * @brief Read the processor cycle counter.
 * @return Counter value.
 */
U32 ReadCycleCounter(void);

/**
 * @brief Compute the difference in number of clock cycles between
 *        two samples of the processor cycle counter, taking counter
 *        overflow into account.
 * @param Start First counter value.
 * @param Stop Last counter value.
 * @return Difference between Start & Stop [number of cycles].
 */
U32 ComputeCycleCounterDiff(U32 Start, U32 Stop);

/**
 * @brief Test the bit indicating if the processor cycle counter
 *        is available to this MCU.
 * @return True = available, False = not available.
 */
static inline Bool CycleCounterIsAvailable(void);

/**
 * @brief Check the number of watchpoint comparators implemented.
 * @return Number of comparators.
 */
static inline U8 GetNofWatchpointComparators(void);

/**
 * @brief Test the bit indicating if this MCU supports trace sampling
 *        & execution tracing.
 */
static inline Bool TraceSamplingIsAvailable(void);

/**
 * @brief Enable the Debug Monitor system exception.
 */
static inline void DebugMonitorExceptionEnable(void);

/**
 * @brief Disable the Debug Monitor system exception.
 */
static inline void DebugMonitorExceptionDisable(void);

/**
 * @brief Check if the Debug Monitor exception is enabled.
 * @return True = enabled, False = not enabled.
 */
static inline Bool DebugMonitorIsEnabled(void);

/**
 * @brief Initialize watchpoint comparators.
 */
void WatchpointComparatorsInit(void);

/**
 * @brief Set a write access data watchpoint.
 * @param Address Address of data watchpoint.
 * @param Callback Comparator match callback function.
 * @note The given address should be 4-byte aligned. Callback parameter of NULL
 *       results in the default handler being used.
 * @return Status code.
 */
WatchpointComparatorStatusEnum DataWatchpointSet(U32* Address, WatchpointCallback Callback);

/**
 * @brief Clear a previously set write access data watchpoint,
 *        freeing the underlying comparator for use again.
 * @param Address Address of previously set watchpoint.
 * @return Status code.
 */
WatchpointComparatorStatusEnum DataWatchpointClear(const U32* Address);

/* --------------------------- Public function definitions ------------------------- */

static inline Bool CycleCounterIsAvailable(void)
{
    return ( (DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk) == 0U );
}

static inline U8 GetNofWatchpointComparators(void)
{
    return (U8)((DWT->CTRL & DWT_CTRL_NUMCOMP_Msk) >> DWT_CTRL_NUMCOMP_Pos);
}

static inline Bool TraceSamplingIsAvailable(void)
{
    return ( (DWT->CTRL & DWT_CTRL_NOTRCPKT_Msk) == 0U );
}

static inline void DebugMonitorExceptionEnable(void)
{
    __COMPILER_BARRIER();
    CoreDebug->DEMCR |= (CoreDebug_DEMCR_MON_EN_Msk | CoreDebug_DEMCR_TRCENA_Msk);
    __COMPILER_BARRIER();
}

static inline void DebugMonitorExceptionDisable(void)
{
    CoreDebug->DEMCR &= ~(CoreDebug_DEMCR_MON_EN_Msk | CoreDebug_DEMCR_TRCENA_Msk);
    __DSB();
    __ISB();
}

static inline Bool DebugMonitorIsEnabled(void)
{
    U32 TmpDEMCR = CoreDebug->DEMCR;
    return (TmpDEMCR & CoreDebug_DEMCR_MON_EN_Msk) && (TmpDEMCR & CoreDebug_DEMCR_TRCENA_Msk);
}

#endif /* CORE_DEBUG */
