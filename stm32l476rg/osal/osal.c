/**
 * @brief Operating System Abstraction Layer
 *
 *        Purpose: Provide application facing interface to RTOS.
 */

/* ------------------------------- Include directives ------------------------------ */

#include "osal.h"
#include "memory_routines.h"

/* ---------------------------- Preprocessor directives ---------------------------- */

#if defined(OSAL_CONFIG_USE_BARE_METAL) && (OSAL_CONFIG_USE_BARE_METAL == 0)
    static U8 RtosMemory[OSAL_STATIC_MEMORY_SIZE_BYTES] ALIGN(OSAL_WORD_SIZE) SECTION(".ram2") = { 0 };
#endif /* Static memory allocation. */

static inline void DummyFunction(void)
{
    for (U32 i = 0; i < OSAL_STATIC_MEMORY_SIZE_BYTES; i++)
    {
        RtosMemory[i] = 0xFFU;
    }
}

void Osal_Delay_ms(U32 Delay_ms)
{
    const Osal_TickType NofTicks = Osal_msToTicks(Delay_ms);
    vTaskDelay(NofTicks);
}