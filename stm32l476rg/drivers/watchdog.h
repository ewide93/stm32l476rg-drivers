/**
 * @file watchdog.h
 */


#ifndef WATCHDOG_H
#define WATCHDOG_H

/* ------------------------------- Include directives ------------------------------ */

#include "typedef.h"
#include "stm32l4xx.h"

/* ------------------------------ Preprocessor macros ------------------------------ */

#define WDG_DEBUG (1U)

/* --------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of available reset reasons for the MCU.
 */
typedef enum
{
    RESET_REASON_FIREWALL    = (1U << 0U),     /* 0x01: Firewall reset                    */
    RESET_REASON_OPTION_BYTE = (1U << 1U),     /* 0x02: Option byte loader reset          */
    RESET_REASON_RESET_PIN   = (1U << 2U),     /* 0x04: nRST pin reset                    */
    RESET_REASON_BROWN_OUT   = (1U << 3U),     /* 0x08: Brown out reset                   */
    RESET_REASON_SW_RESET    = (1U << 4U),     /* 0x10: Software reset                    */
    RESET_REASON_IWDG        = (1U << 5U),     /* 0x20: Independent window watchdog reset */
    RESET_REASON_WWDG        = (1U << 6U),     /* 0x40: Window watchdog reset             */
    RESET_REASON_LP_RESET    = (1U << 7U),     /* 0x80: Low-power reset                   */
    RESET_REASON_ENUM_LIMIT  = 0xFFU
} Wdg_ResetReasonEnum;

/* -------------------------- Public function declarations ------------------------- */

/**
 * @brief Read the reason for the latest reboot.
 */
U8 Wdg_ReadResetReason(void);

/**
 * @brief Check if the given reset reason is active for this MCU reset cycle.
 * @param Reason Reset reason to check against.
 * @return True = Reason is active, False = Reason is not active.
 */
Bool Wdg_ResetReasonActive(Wdg_ResetReasonEnum Reason);

#endif /* WATCHDOG_H */
