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

#define KiB_TO_BYTES(KiB)   (1024UL * (KiB))

/* ---- Preprocessor definitions controlling size & granularity of memory pool. ---- */

/**
 * @note Smaller chunks cause greater memory overhead for metadata
 *       but potentially wastes less space if many small allocations are needed.
 */
#define MEMPOOL_SIZE        (KiB_TO_BYTES(8))    /* Memory pool size in bytes.  */
#define MEMPOOL_CHUNK_SIZE  (64U)                /* Memory chunk size in bytes. */
#define MEMPOOL_NOF_CHUNKS  (MEMPOOL_SIZE / MEMPOOL_CHUNK_SIZE)

StaticAssert(MEMPOOL_SIZE % MEMPOOL_CHUNK_SIZE == 0, "POOL_SIZE not evenly divisible by CHUNK_SIZE!");
StaticAssert(MEMPOOL_CHUNK_SIZE <= 128UL, "MEMPOOL_CHUNK_SIZE max size exceeded!");

/* ----------------- Module configuration preprocessor definitions. ---------------- */

#define MEMPOOL_FIXED_SIZE_HEAP_SECTION        ".fixed_size_heap"
#define MEMPOOL_USE_FIXED_SIZE_HEAP_SECTION    (1)
StaticAssert(MEMPOOL_USE_FIXED_SIZE_HEAP_SECTION == 1 || MEMPOOL_USE_FIXED_SIZE_HEAP_SECTION == 0,
            "Missing or invalid value for MEMPOOL_USE_FIXED_SIZE_HEAP_SECTION!");

#define MEMPOOL_ENABLE_HIGH_WATER_MARK  (1)
StaticAssert(MEMPOOL_ENABLE_HIGH_WATER_MARK == 1 || MEMPOOL_ENABLE_HIGH_WATER_MARK == 0,
            "Missing or invalid value for MEMPOOL_ENABLE_HIGH_WATER_MARK!");

/* ------------------------- Public function declarations. ------------------------- */

/**
 * @brief Initialization function for the memory pool handler module.
 */
void MemPool_Init(void);

/**
 * @brief Get memory pool module initialization status.
 * @return True = initialized, False = not initialized.
 */
Bool MemPool_IsInitialized(void);

/**
 * @brief Disable interactions with memory pool. Effectively renders
 *        further calls to Allocate/Free useless.
 */
void MemPool_Disable(void);

/**
 * @brief Allocate enough chunks of the internally managed memory pool
 *        to cover the desired allocation size.
 * @param Size The desired number of bytes to allocate.
 * @return Pointer to the start of the allocated memory. Should be explicitly
 *         cast into proper pointer type by caller.
 *         Returns NULL if the desired size could not be allocated
 *         in a consecutive memory space. Allocated memory is zero initialized.
 */
void* MemPool_Allocate(U32 Size);

/**
 * @brief Free the chunks allocated for the given address.
 * @param Address Address from a previous call to MemPool_Allocate.
 */
void MemPool_Free(void* Address);

/**
 * @brief Get the number of bytes that are available for allocation.
 * @note Does not necessarily mean that this amount
 *       of bytes can be allocated due to fragmentation.
 * @return Number of free bytes.
 */
U32 MemPool_GetNofFreeBytes(void);

/* -------------- Conditionally compiled public function declarations. ------------- */

#if MEMPOOL_ENABLE_HIGH_WATER_MARK == 1
    /**
     * @brief Get the maximum number of bytes that has been allocated at one point.
     * @return High water mark in number of bytes.
     */
    U32 MemPool_GetHighWaterMark(void);
#endif /* MEMPOOL_ENABLE_HIGH_WATER_MARK == 1 */

#endif /* MEMPOOL_H */
