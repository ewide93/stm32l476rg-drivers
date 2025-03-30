/**
 * @file core_debug.h
 *
 * @brief Arm Cortex M4 debug peripheral module.
 */

 /* ------------------------------- Include directives ------------------------------ */
 #include "core_debug.h"

 /* -------------------------- Public function definitions -------------------------- */

Bool CycleCounterIsAvailable(void)
{
    return ( (DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk) == 0U );
}
void EnableCycleCounter(void)
{
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
}
void DisableCycleCounter(void)
{
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
}

U32 ReadCycleCounter(void)
{
    return DWT->CYCCNT;
}

U32 ComputeCycleCounterDiff(U32 Start, U32 Stop)
{
    U32 Diff = 0U;
    if ( Stop > Start )
    {
        /* No overflow. */
        Diff = Stop - Start;
    }
    else
    {
        /* Counter overflow. */
        Diff = (U32_MAX - Start) + (Stop + 1U);
    }
    return Diff;
}
