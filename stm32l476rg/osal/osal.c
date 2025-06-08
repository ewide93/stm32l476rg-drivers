/**
 * @brief Operating System Abstraction Layer
 *
 *        Purpose: Provide application facing interface to RTOS.
 */

/* ------------------------------- Include directives ------------------------------ */

#include "osal.h"
#include "mempool.h"

/* ------------------------ OSAL implementation for FreeRTOS ----------------------- */

#if defined(OSAL_CONFIG_USE_FREERTOS) && (OSAL_CONFIG_USE_FREERTOS == 1)

/* ------------------ FreeRTOS specific structures & enumerations ------------------ */

typedef struct
{
    StaticTask_t Tcb;           // Task control block
    TaskHandle_t TaskHandle;    // Task handle
    U8* StackPtr;               // Pointer to thread stack.
    S32 Id;                     // Incremental unique ID assigned to threads.
} Osal_ThreadMetadata;

/* -------------------------------- Local variables -------------------------------- */

static Osal_ThreadMetadata OsalMetadata[OSAL_MAX_NOF_THREADS] = { 0 };
static U8 NofThreads = 0U;

/* ----------------- FreeRTOS specific local function declarations ----------------- */

/**
 * @brief Validate arguments to thread create function.
 * @return True = valid, False = invalid.
 */
static inline Bool Osal_ThreadCreateArgsValid(Osal_ThreadFunc Func, U32 StackSize, Osal_PriorityEnum Priority);

/* ------------------ FreeRTOS specific local function definitions ----------------- */

static inline Bool Osal_ThreadCreateArgsValid(Osal_ThreadFunc Func, U32 StackSize, Osal_PriorityEnum Priority)
{
    if (!MemPool_IsInitialized()) { return False; }
    const Bool MemoryOk = (MemPool_GetNofFreeBytes() >= StackSize);
    const Bool FuncNonNull = (Func != NULL);
    const Bool PrioOk = (Priority <= THREAD_PRIORITY_HIGHEST);
    const Bool ThreadLimitNotReached = (NofThreads < OSAL_MAX_NOF_THREADS);
    return MemoryOk && FuncNonNull && PrioOk && ThreadLimitNotReached;
}

/* ----------------- FreeRTOS specific public function definitions ----------------- */

void Osal_Delay_ms(U32 Delay_ms)
{
    const U32 NofTicks = Osal_msToTicks(Delay_ms);
    vTaskDelay(NofTicks);
}

void Osal_StartScheduler(void)
{
    if (NofThreads > 0U) { vTaskStartScheduler(); }
}

S32 Osal_ThreadCreate(Osal_ThreadFunc Func, void* Arg, U32 StackSize, Osal_PriorityEnum Priority)
{
    S32 RetVal = -1;
    if (Osal_ThreadCreateArgsValid(Func, StackSize, Priority))
    {
        OsalMetadata[NofThreads].StackPtr = (U8*)MemPool_Allocate(StackSize);
        if (OsalMetadata[NofThreads].StackPtr != NULL)
        {
            // Use an ASCII representation of the thread ID as the thread name.
            Char Name[2] = { '\0', '\0' };
            Name[0] = (Char)(NofThreads + 0x30);

            // Calculate stack size into number of words.
            const size_t NofWords = StackSize / sizeof(StackType_t);

            OsalMetadata[NofThreads].TaskHandle = xTaskCreateStatic(Func, Name, NofWords, Arg, Priority, (StackType_t*)OsalMetadata[NofThreads].StackPtr, &OsalMetadata[NofThreads].Tcb);

            // Check if thread was created successfully, if not, free allocated stack.
            if (OsalMetadata[NofThreads].TaskHandle != NULL)
            {
                RetVal = NofThreads;
                NofThreads++;
            }
            else
            {
                MemPool_Free(OsalMetadata[NofThreads].StackPtr);
            }
        }

    }
    return RetVal;
}

#endif /* FreeRTOS specific */
