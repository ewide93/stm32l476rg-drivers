/**
 * @file mempool.c
 *
 * @brief Static memory pool handler module.
 *
 */

#ifndef MEMPOOL_H
#define MEMPOOL_H

/* ------------------------------- Include directives ------------------------------ */

#include "typedef.h"

#define KiB_TO_BYTES(KiB)   (1024UL * KiB)

/* ---- Preprocessor definitions controlling size & granularity of memory pool. ---- */
#define MEMPOOL_SIZE        (KiB_TO_BYTES(8))    /* Memory pool size in bytes.  */
#define MEMPOOL_CHUNK_SIZE  (32UL)      /* Memory chunk size in bytes. */
#define MEMPOOL_NOF_CHUNKS  (MEMPOOL_SIZE / MEMPOOL_CHUNK_SIZE)
StaticAssert(MEMPOOL_SIZE % MEMPOOL_CHUNK_SIZE == 0, "POOL_SIZE not evenly divisible by CHUNK_SIZE!");
StaticAssert(MEMPOOL_CHUNK_SIZE < 128UL, "MEMPOOL_CHUNK_SIZE max size exceeded!");

#define MEMPOOL_USE_RAM2_SECTION    (1)
StaticAssert(MEMPOOL_USE_RAM2_SECTION == 1 || MEMPOOL_USE_RAM2_SECTION == 0, "Missing or invalid value for MEMPOOL_USE_RAM2_SECTION!");

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
void* MemPool_Allocate(U32 Size);

/**
 * @brief Free the chunks allocated for the given address.
 * @param Address Address from a previous call to MemPool_Allocate.
 */
void MemPool_Free(void* Address);

/**
 * @brief Get the number of chunks that are available for allocation.
 * @return Number of free chunks.
 */
U8 MemPool_GetNofAvailableChunks(void);

#endif /* MEMPOOL_H */