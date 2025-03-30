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

/* -------------------------- Public function declarations ------------------------- */

/**
 * @brief Test the bit indicating if the processor cycle counter
 *        is available to this MCU.
 * @return True = available, False = not available.
 */
Bool CycleCounterIsAvailable(void);

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

#endif /* CORE_DEBUG */
