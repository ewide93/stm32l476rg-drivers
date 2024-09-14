/**
 * @file flash.c
 *
 * @brief Configuration & control of embedded flash memory.
 *
 */

/* ------------------------------- Include directives ------------------------------ */
#include "flash.h"
#include "critical_section.h"

/* -------------------------------- Macro definitions ------------------------------ */

/*  ---------------------------Structures & enumerations --------------------------- */

/* -------------------------- Public function definitions -------------------------- */

void Flash_SetFlashLatency(Flash_WaitStateEnum NofWaitStates)
{
    CRITICAL_SECTION_ENTER;
    U32 TempReg = FLASH->ACR;
    TempReg &= ~(FLASH_ACR_LATENCY_Msk);
    TempReg |= NofWaitStates;
    FLASH->ACR = TempReg;
    CRITICAL_SECTION_EXIT;
}
