/**
 * @file core_debug.h
 *
 * @brief Arm Cortex M4 debug peripheral module.
 */

 /* ------------------------------- Include directives ------------------------------ */
 #include "core_debug.h"
 #include "memory_routines.h"

 /* --------------------------- Structures & enumerations --------------------------- */

 typedef struct
 {
    WatchpointCallback Callback;
    Bool Enabled;
 } WatchpointComparatorType;

 /* ------------------------------- Private variables ------------------------------- */

WatchpointComparatorType WatchpointComparators[NOF_WATCHPOINT_COMPARATORS_MAX] = { 0 };
U8 NofEnabledComparators = 0U;

/* -------------------------- Private function declarations ------------------------- */

/**
 * @brief Default watchpoint comparator handler.
 */
void WatchpointComparatorDefaultHandler(void);

/**
 * @brief Find the index for the first free watchpoint comparator.
 * @return Comparator index, negative value indicates all comparators are busy.
 */
S8 FindComparatorIndex(void);

/**
 * @brief Read the address assigned to the comparator at the given
 *        comparator index.
 * @param Index Comparator index.
 * @return Address configuration of the comparator.
 */
void* GetComparatorAddress(U8 Index);

/**
 * @brief Reset the watchpoint comparator at the given index.
 * @param Index Comparator index.
 */
void ComparatorClear(U8 Index);


 /* -------------------------- Public function definitions -------------------------- */

void EnableCycleCounter(void)
{
    if (CycleCounterIsAvailable())
    {
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    }
}

void DisableCycleCounter(void)
{
    if (CycleCounterIsAvailable())
    {
        DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
        CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
    }
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

void WatchpointComparatorsInit(void)
{
    for (U8 i = 0; i < NOF_WATCHPOINT_COMPARATORS_MAX; i++)
    {
        WatchpointComparators[i].Callback = WatchpointComparatorDefaultHandler;
        WatchpointComparators[i].Enabled = False;
    }
}

 S8 FindComparatorIndex(void)
 {
    S8 Index = -1;
    U8 MaxNofComparators = GetNofWatchpointComparators();
    for (U8 i = 0; i < MaxNofComparators; i++)
    {
        if (!WatchpointComparators[i].Enabled)
        {
            Index = i;
            break;
        }
    }
    return Index;
}

void* GetComparatorAddress(U8 Index)
{
    void* Address = NULL;
    switch (Index)
    {
        case 0  : { Address = (void*)DWT->COMPARATOR0; break; }
        case 1  : { Address = (void*)DWT->COMPARATOR1; break; }
        case 2  : { Address = (void*)DWT->COMPARATOR2; break; }
        case 3  : { Address = (void*)DWT->COMPARATOR3; break; }
        default : { break; }
    }
    return Address;
}

WatchpointComparatorStatusEnum DataWatchpointSet(U32* Address, WatchpointCallback Callback)
{
    WatchpointComparatorStatusEnum Status;
    S8 ComparatorIndex = FindComparatorIndex();

    if (ComparatorIndex < 0)
    {
        Status = WATCHPOINT_STATUS_HW_LIMIT;
    }
    else if (!IsAligned((void*)Address, 4U))
    {
        Status = WATCHPOINT_STATUS_WRONG_ALIGNMENT;
    }
    else
    {
        switch (ComparatorIndex)
        {
            case 0:
            {
                DWT->COMPARATOR0 = (U32)Address;
                DWT->MASK0 = 0x00UL;
                DWT->FUNCTION0 |= (0x3U << DWT_FUNCTION_FUNCTION_Pos) | (0x2U << DWT_FUNCTION_DATAVSIZE_Pos);
                break;
            }
            case 1:
            {
                DWT->COMPARATOR1 = (U32)Address;
                DWT->MASK1 = 0x00UL;
                DWT->FUNCTION1 |= (0x3U << DWT_FUNCTION_FUNCTION_Pos) | (0x2U << DWT_FUNCTION_DATAVSIZE_Pos);
                break;
            }
            case 2:
            {
                DWT->COMPARATOR2 = (U32)Address;
                DWT->MASK2 = 0x00UL;
                DWT->FUNCTION2 |= (0x3U << DWT_FUNCTION_FUNCTION_Pos) | (0x2U << DWT_FUNCTION_DATAVSIZE_Pos);
                break;
            }
            case 3:
            {
                DWT->COMPARATOR3 = (U32)Address;
                DWT->MASK3 = 0x00UL;
                DWT->FUNCTION3 |= (0x3U << DWT_FUNCTION_FUNCTION_Pos) | (0x2U << DWT_FUNCTION_DATAVSIZE_Pos);
                break;
            }
            default:
            {
                break;
            }
        }

        if (Callback != NULL) { WatchpointComparators[ComparatorIndex].Callback = Callback; }
        WatchpointComparators[ComparatorIndex].Enabled = True;
        NofEnabledComparators++;
        Status = WATCHPOINT_STATUS_OK;
    }
    return Status;
}

void ComparatorClear(U8 Index)
{
    switch (Index)
    {
        case 0:
        {
            DWT->COMPARATOR0 = 0x00UL;
            DWT->MASK0 = 0x00UL;
            DWT->FUNCTION0 = 0x00UL;
            break;
        }
        case 1:
        {
            DWT->COMPARATOR1 = 0x00UL;
            DWT->MASK1 = 0x00UL;
            DWT->FUNCTION1 = 0x00UL;
            break;
        }
        case 2:
        {
            DWT->COMPARATOR2 = 0x00UL;
            DWT->MASK2 = 0x00UL;
            DWT->FUNCTION2 = 0x00UL;
            break;
        }
        case 3:
        {
            DWT->COMPARATOR3 = 0x00UL;
            DWT->MASK3 = 0x00UL;
            DWT->FUNCTION3 = 0x00UL;
            break;
        }
        default:
        {
            break;
        }
    }
    WatchpointComparators[Index].Enabled = False;
    WatchpointComparators[Index].Callback = WatchpointComparatorDefaultHandler;
    NofEnabledComparators--;
}

WatchpointComparatorStatusEnum DataWatchpointClear(const U32* Address)
{
    WatchpointComparatorStatusEnum Status = WATCHPOINT_STATUS_UNKNOWN_ADDRESS;
    U8 MaxNofComparators = GetNofWatchpointComparators();

    for (U8 i = 0; i < MaxNofComparators; i++)
    {
        if (Address == (U32*)GetComparatorAddress(i))
        {
            ComparatorClear(i);
            Status = WATCHPOINT_STATUS_OK;
            break;
        }
    }
    return Status;
}

 void WatchpointComparatorDefaultHandler(void)
 {
    while (True)
    {
        __NOP();
    }
 }

/**
 * @brief Handler for Debug Monitor system exception. Check for watchpoint comparator
 *        matches and execute attached callback function.
 */
void DebugMonHandler(void)
{
    if (DWT->FUNCTION0 & DWT_FUNCTION_MATCHED_Msk)
    {
        WatchpointComparators[0].Callback();
    }

    if (DWT->FUNCTION1 & DWT_FUNCTION_MATCHED_Msk)
    {
        WatchpointComparators[1].Callback();
    }

    if (DWT->FUNCTION2 & DWT_FUNCTION_MATCHED_Msk)
    {
        WatchpointComparators[2].Callback();
    }

    if (DWT->FUNCTION3 & DWT_FUNCTION_MATCHED_Msk)
    {
        WatchpointComparators[3].Callback();
    }
}
