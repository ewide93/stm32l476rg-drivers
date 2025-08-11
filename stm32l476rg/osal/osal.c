/**
 * @brief Operating System Abstraction Layer
 *
 *        Purpose: Provide application facing interface to RTOS.
 */

/* ------------------------------- Include directives ------------------------------ */

#include "osal.h"
#include "mempool.h"
#include "range.h"

/* ------------------------ OSAL implementation for FreeRTOS ----------------------- */

#if defined(OSAL_CONFIG_USE_FREERTOS) && (OSAL_CONFIG_USE_FREERTOS == 1)

/* ------------------ FreeRTOS specific structures & enumerations ------------------ */

typedef struct
{
    StaticTask_t Tcb;           // Task control block
    TaskHandle_t TaskHandle;    // Task handle
    U8* StackPtr;               // Pointer to thread stack.
    U32 StackSize;              // Thread stack size in bytes.
    S32 Id;                     // Incremental unique ID assigned to threads.
} Osal_ThreadMetadata;

/* -------------------------------- Local variables -------------------------------- */

static Osal_ThreadMetadata Private_Threads[OSAL_MAX_NOF_THREADS] = { 0 };
static U8 Private_NofThreads = 0U;
static Osal_MutexHandleType Private_MutexHandles[OSAL_MAX_NOF_MUTEXES] = { 0 };
static StaticSemaphore_t Private_MutexBuffer[OSAL_MAX_NOF_MUTEXES] = { 0 };
static U8 Private_NofMutexes = 0U;

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
    const Bool ThreadLimitNotReached = (Private_NofThreads < OSAL_MAX_NOF_THREADS);
    return MemoryOk && FuncNonNull && PrioOk && ThreadLimitNotReached;
}

/* ----------------- FreeRTOS specific public function definitions ----------------- */

void Osal_Delay_ms(U32 Delay_ms)
{
    const U32 NofTicks = Osal_msToTicks(Delay_ms);
    vTaskDelay(NofTicks);
}

void Osal_DelayUntil(U32* const LastWakeTime, U32 TimeIncrement)
{
    vTaskDelayUntil(LastWakeTime, TimeIncrement);
}

void Osal_StartScheduler(void)
{
    if (Private_NofThreads > 0U) { vTaskStartScheduler(); }
}

S32 Osal_ThreadCreate(Osal_ThreadFunc Func, void* Arg, U32 StackSize, Osal_PriorityEnum Priority)
{
    S32 RetVal = -1;
    if (Osal_ThreadCreateArgsValid(Func, StackSize, Priority))
    {
        Private_Threads[Private_NofThreads].StackPtr = (U8*)MemPool_Allocate(StackSize);
        if (Private_Threads[Private_NofThreads].StackPtr != NULL)
        {
            // Use an ASCII representation of the thread ID as the thread name.
            /**
             * @todo Handle ASCII conversion better (multiple digits?)
             */
            Char Name[2] = { '\0', '\0' };
            Name[0] = (Char)(Private_NofThreads + 0x30);

            // Calculate stack size into number of words.
            const size_t NofWords = StackSize / sizeof(StackType_t);

            Private_Threads[Private_NofThreads].TaskHandle = xTaskCreateStatic(Func, Name, NofWords, Arg, Priority, (StackType_t*)Private_Threads[Private_NofThreads].StackPtr, &Private_Threads[Private_NofThreads].Tcb);

            // Check if thread was created successfully, if not, free allocated stack.
            if (Private_Threads[Private_NofThreads].TaskHandle != NULL)
            {
                RetVal = Private_NofThreads;
                Private_Threads[Private_NofThreads].Id = Private_NofThreads;
                Private_Threads[Private_NofThreads].StackSize = StackSize;
                Private_NofThreads++;
            }
            else
            {
                MemPool_Free(Private_Threads[Private_NofThreads].StackPtr);
            }
        }

    }
    return RetVal;
}

void Osal_ThreadSuspend(S32 Id)
{
    if (Range_S32(Id, 0, OSAL_MAX_NOF_THREADS))
    {
        vTaskSuspend(Private_Threads[Id].TaskHandle);
    }
}

void Osal_ThreadResume(S32 Id)
{
    if (Range_S32(Id, 0, OSAL_MAX_NOF_THREADS))
    {
        vTaskResume(Private_Threads[Id].TaskHandle);
    }
}

U32 Osal_GetTickCount(void)
{
    return (U32)xTaskGetTickCount();
}

U32 Osal_GetTickCount_ISR(void)
{
    return (U32)xTaskGetTickCountFromISR();
}

Osal_MutexHandleType Osal_MutexCreate(void)
{
    Osal_MutexHandleType Retval = (Osal_MutexHandleType)NULL;
    for (U8 i = 0U; i < OSAL_MAX_NOF_MUTEXES; i++)
    {
        if (Private_MutexHandles[i] == NULL)
        {
            Retval = xSemaphoreCreateMutexStatic(&Private_MutexBuffer[i]);
            if (Retval != NULL)
            {
                Private_NofMutexes++;
                break;
            }
        }
    }
    return Retval;
}

void Osal_MutexDestroy(Osal_MutexHandleType MutexHandle)
{
    for (U8 i = 0U; i < OSAL_MAX_NOF_MUTEXES; i++)
    {
        if (Private_MutexHandles[i] == MutexHandle)
        {
            Private_MutexHandles[i] = NULL;
            Private_NofMutexes--;
            break;
        }
    }
}

U8 Osal_GetNofClaimedMutexes(void)
{
    return Private_NofMutexes;
}

U8 Osal_GetNofFreeMutexes(void)
{
    return (U8)(OSAL_MAX_NOF_MUTEXES) - Private_NofMutexes;
}

Bool Osal_MutexAquire(Osal_MutexHandleType MutexHandle)
{
    /**
     * @todo Handle blocking for x amount of ticks or only use non-blocking?
     */
    return (Bool)xSemaphoreTake(MutexHandle, 0UL);
}

Bool Osal_MutexRelease(Osal_MutexHandleType MutexHandle)
{
    return (Bool)xSemaphoreGive(MutexHandle);
}

#endif /* FreeRTOS specific */
