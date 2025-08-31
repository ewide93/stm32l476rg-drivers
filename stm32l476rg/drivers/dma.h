/**
 * @file dma.h
 *
 * @brief Interface for DMA peripheral driver.
 */

#ifndef DMA_H
#define DMA_H

/* ------------------------------- Include directives ------------------------------ */
#include "typedef.h"
#include "stm32l4xx.h"

/* --------------------------- Structures & enumerations --------------------------- */

/**
 * @brief Enumeration of available DMA peripherals.
 */
typedef enum
{
    DMA_INSTANCE_1 = 0x0U,
    DMA_INSTANCE_2 = 0x1U,
    DMA_INSTANCE_ENUM_LIMIT
} Dma_InstanceEnum;

/**
 * @brief Enumeration of available channels in a DMA instance.
 */
typedef enum
{
    DMA_CHANNEL_1 = 0x0U,
    DMA_CHANNEL_2 = 0x1U,
    DMA_CHANNEL_3 = 0x2U,
    DMA_CHANNEL_4 = 0x3U,
    DMA_CHANNEL_5 = 0x4U,
    DMA_CHANNEL_6 = 0x5U,
    DMA_CHANNEL_7 = 0x6U,
    DMA_CHANNEL_ENUM_LIMIT
} Dma_ChannelEnum;

/**
 * @brief Enumeration of DMA channel priority levels.
 */
typedef enum
{
    DMA_CHANNEL_PRIO_LOW = 0x0U,
    DMA_CHANNEL_PRIO_MEDIUM = 0x1U,
    DMA_CHANNEL_PRIO_HIGH = 0x2U,
    DMA_CHANNEL_PRIO_VERY_HIGH = 0x3U,
    DMA_CHANNEL_PRIO_ENUM_LIMIT
} Dma_ChannelPriorityEnum;

/**
 * @brief Enumeration of DMA transfer sizes.
 */
typedef enum
{
    DMA_TRANSFER_SIZE_8BIT = 0x0U,
    DMA_TRANSFER_SIZE_16BIT = 0x1U,
    DMA_TRANSFER_SIZE_32BIT = 0x2U,
    DMA_TRANSFER_SIZE_ENUM_LIMIT
} Dma_TransferSizeEnum;

/**
 * @brief Enumeration of DMA transfer directions.
 */
typedef enum
{
    DMA_TRANSFER_DIR_READ_FROM_PERIPHERAL = 0x0U,
    DMA_TRANSFER_DIR_READ_FROM_MEMORY = 0x1U,
    DMA_TRANSFER_DIR_ENUM_LIMIT
} Dma_TransferDirectionEnum;

/**
 * @brief Opaque DMA handle type.
 */
typedef struct Dma_OpaqueHandleType* Dma_HandleType;

/* -------------------------- Public function declarations ------------------------- */

/**
 * @brief DMA driver initialization, must be called before before
 *        further interaction with the DMA driver module.
 */
void Dma_Init(void);

/**
 * @brief Get a DMA handle matching the given instance & channel pair.
 * @param Instance DMA instance number.
 * @param Channel DMA channel number.
 * @return Handle used to interact with the DMA peripheral.
 * @warning Returns NULL if the DMA driver is not initialized before
 *          calling this function.
 */
Dma_HandleType Dma_GetHandle(Dma_InstanceEnum Instance, Dma_ChannelEnum Channel);

/**
 * @brief Check availablility of the given DMA channel.
 * @param Handle DMA peripheral handle.
 * @return True = available, False = not available.
 */
Bool Dma_ChannelIsAvailable(Dma_HandleType Handle);

/**
 * @brief Read the "number of data to transfer" register of the given DMA channel.
 * @param Handle DMA peripheral handle.
 * @return Remaining transfers until the configured number of transfers is reached.
 */
U16 Dma_GetTransferCnt(Dma_HandleType Handle);

/**
 * @brief Enable the given DMA channel.
 * @param Handle DMA peripheral handle.
 */
void Dma_ChannelEnable(Dma_HandleType Handle);

/**
 * @brief Disable the given DMA channel.
 * @param Handle DMA peripheral handle.
 */
void Dma_ChannelDisable(Dma_HandleType Handle);

#endif /* DMA_H */
