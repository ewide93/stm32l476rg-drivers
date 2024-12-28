/**
 * @file systick.c
 *
 * @brief System timer
 */

/* -------------------------------Include directives ------------------------------- */
#include "systick.h"

/* --------------------------- Module private variables ---------------------------- */
static volatile U32 TickCnt = 0;
static volatile U32 PreviousTickCnt = 0;
static volatile U32 WrapAroundCnt = 0;

/* -------------------------- Public function definitions -------------------------- */

void SysTick_Init(const SysTick_ConfigType* Config)
{
    SysTick->LOAD = ( (Config->ReloadVal & SysTick_LOAD_RELOAD_Msk) - 1U );
    SysTick->VAL = 0UL;
    SysTick->CTRL |= ( SysTick_CTRL_TICKINT_Msk |
                       SysTick_CTRL_ENABLE_Msk |
                       ( Config->ClockSource == SYSTICK_CLK_SRC_AHB ? (1 << SysTick_CTRL_CLKSOURCE_Pos) : (0U) ) );
}

U32 SysTick_GetTicks(void)
{
    return TickCnt;
}

U32 SysTick_GetNofWrapArounds(void)
{
    return WrapAroundCnt;
}


/* -------------------- Redefinition of weak interrupt handlers -------------------- */

/**
 * @brief Interrupt handler for Cortex system timer. Used to generate a time base
 *        with a frequency given by the timer configuration.
 */
void SysTickHandler(void)
{
    PreviousTickCnt = TickCnt;
    TickCnt++;
    if (TickCnt < PreviousTickCnt) { WrapAroundCnt++; }
}

