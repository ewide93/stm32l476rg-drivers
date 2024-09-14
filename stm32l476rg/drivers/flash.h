/**
 * @file flash.h
 *
 * @brief Configuration & control of embedded flash memory.
 *
 */

#ifndef FLASH_H
#define FLASH_H

/* ------------------------------- Include directives ------------------------------ */
#include "stm32l4xx.h"
#include "typedef.h"

/*  ---------------------------Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of available settings for flash access wait states.
 */
typedef enum
{
    FLASH_WS_0 = 0x0U,
    FLASH_WS_1 = 0x1U,
    FLASH_WS_2 = 0x2U,
    FLASH_WS_3 = 0x3U,
    FLASH_WS_4 = 0x4U
} Flash_WaitStateEnum;

/* -------------------------- Public function definitions -------------------------- */

/**
 * @brief Enable flash prefetch.
 */
static inline void Flash_PrefetchEnable(void)
{
    FLASH->ACR |= FLASH_ACR_PRFTEN;
}


/**
 * @brief Disable flash prefetch.
 */
static inline void Flash_PrefetchDisable(void)
{
    FLASH->ACR &= ~FLASH_ACR_PRFTEN;
}


/**
 * @brief Enable the instruction cache.
 */
static inline void Flash_InstructionCacheEnable(void)
{
    FLASH->ACR |= FLASH_ACR_ICEN;
}


/**
 * @brief Disable the instruction cache.
 */
static inline void Flash_InstructionCacheDisable(void)
{
    FLASH->ACR &= ~FLASH_ACR_ICEN;
}


/**
 * @brief Enable the data cache.
 */
static inline void Flash_DataCacheEnable(void)
{
    FLASH->ACR |= FLASH_ACR_DCEN;
}


/**
 * @brief Disable the data cache.
 */
static inline void Flash_DataCacheDisable(void)
{
    FLASH->ACR &= ~FLASH_ACR_DCEN;
}

/* -------------------------- Public function prototypes --------------------------- */

/**
 * @brief Set the number of wait states for flash access.
 * @param NofWaitStates Number of wait states.
 * @note While the system clock frequency is 80MHz and the digital core domain
 *       is not under-volted the number of wait states should be set to 4.
 */
void Flash_SetFlashLatency(Flash_WaitStateEnum NofWaitStates);

#endif /* FLASH_H */
