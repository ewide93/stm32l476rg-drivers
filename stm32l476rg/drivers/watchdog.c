/**
 * @file watchdog.c
 */

/* ------------------------------- Include directives ------------------------------ */

#include "watchdog.h"

/* -------------------------------- Local variables -------------------------------- */

static U8 ResetReason = (U8)RESET_REASON_ENUM_LIMIT;

/* -------------------------- Public function definitions -------------------------- */

U8 Wdg_ReadResetReason(void)
{
    if (ResetReason == RESET_REASON_ENUM_LIMIT)
    {
        ResetReason = (U8)(RCC->CSR >> RCC_CSR_FWRSTF_Pos);

        /* Clear reset reason bits */
        RCC->CSR |= RCC_CSR_RMVF;
    }

    return ResetReason;
}

Bool Wdg_ResetReasonActive(Wdg_ResetReasonEnum Reason)
{
    if (ResetReason == RESET_REASON_ENUM_LIMIT) { (void)Wdg_ReadResetReason(); }
    return (ResetReason & (U8)Reason);
}
