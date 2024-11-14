#ifndef MEMPOOL_H
#define MEMPOOL_H

#include "typedef.h"

#define CHUNK_SIZE      (16U)
#define NOF_CHUNKS      (8U)
#define POOL_SIZE       (CHUNK_SIZE * NOF_CHUNKS)

/**
 * @brief Initialization function for the memory pool handler module.
 */
void MemPool_Init(void);

/**
 * @brief Allocate enough chunks of the internally managed memory pool
 *        to cover the desired allocation size.
 * @param Size The desired number of bytes to allocate.
 * @return Pointer to the start of the allocated memory. Should be explicitly
 *         cast into proper pointer type by caller.
 *         Returns NULL if the desired size could not be allocated
 *         in a consecutive memory space.
 */
void* MemPool_Allocate(U16 Size);

/**
 * @brief Free the chunks allocated for the given address.
 * @param Address Address from a previous call to MemPool_Allocate.
 */
void MemPool_Free(void* Address);

/**
 * @brief Reset the internally managed memory pool.
 * @note Only used in unit tests.
 */
#if defined(UNIT_TEST)
void MemPool_Reset(void);
#endif  /* UNIT_TEST */

/**
 * @brief Get the number of chunks that are available for allocation.
 * @return Number of free chunks.
 */
U8 MemPool_GetNofAvailableChunks(void);

#endif /* MEMPOOL_H */