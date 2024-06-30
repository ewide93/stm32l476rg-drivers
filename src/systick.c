/**
 * @file systick.c
 *
 * @brief System timer
 */

/* -------------------------------Include directives ------------------------------- */
#include "systick.h"

/* --------------------------- Module private variables ---------------------------- */
static volatile U64 TickCnt = 0;

/* -------------------------- Public function definitions -------------------------- */

void SysTick_Init(const SysTick_ConfigType* Config)
{
    SysTick->VAL = 0UL;
    SysTick->LOAD = ( (Config->ReloadVal & SysTick_LOAD_RELOAD_Msk) - 1U );
    SysTick->CTRL |= ( SysTick_CTRL_TICKINT_Msk |
                       SysTick_CTRL_ENABLE_Msk |
                       ( Config->ClockSource == SYSTICK_CLK_SRC_AHB ? (1 << SysTick_CTRL_CLKSOURCE_Pos) : (0U) ) );
}

U64 SysTick_GetTicks(void)
{
    return TickCnt;
}

/* -------------------- Redefinition of weak interrupt handlers -------------------- */

/**
 * @brief Interrupt handler for Cortex system timer. Used to generate a time base
 *        with a frequency given by the timer configuration.
 */
void SysTickHandler(void)
{
    TickCnt++;
}

