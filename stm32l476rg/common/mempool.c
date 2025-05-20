/**
 * @file mempool.c
 *
 * @brief Static memory pool handler module.
 *
 */

/* ------------------------------- Include directives ------------------------------ */

#include "mempool.h"


/**
 * @brief Enumeration of different statuses for chunks of memory.
 */
typedef enum
{
    CHUNK_STATUS_FREE = 0,                  /* The chunk is free. */
    CHUNK_STATUS_ALLOC_STANDALONE = 1,      /* The chunk is allocated and not part of a multi-chunk allocation. */
    CHUNK_STATUS_ALLOC_FIRST = 2,           /* The chunk is allocated and is the first chunk of a multi-chunk allocation. */
    CHUNK_STATUS_ALLOC_SECTION = 3,         /* The chunk is allocated and is part of a multi-chunk allocation. */
    CHUNK_STATUS_ALLOC_LAST = 4             /* The chunk is allocated and is the last chunk of a multi-chunk allocation. */
} MemPool_ChunkStatusEnum;

/**
 * @brief Memory pool chunk metadata structure.
 */
typedef struct
{
    U8* Data;
    U8 Index;
    MemPool_ChunkStatusEnum Status;
} MemPool_ChunkType;

/**
 * @brief Memory pool module internal data structure.
 */
typedef struct
{
    MemPool_ChunkType Chunks[MEMPOOL_NOF_CHUNKS];
    U8* MemoryPool;
    U32 ChunksAvailable;
    U8* FirstChunk;
    U8* LastChunk;
    #if MEMPOOL_ENABLE_HIGH_WATER_MARK == 1
        U32 HighWaterMark;
    #endif /* MEMPOOL_ENABLE_HIGH_WATER_MARK == 1 */
    Bool Initialized;
} MemPool_InternalType;

/* -------------------------------- Local variables -------------------------------- */

#if (MEMPOOL_USE_FIXED_SIZE_HEAP_SECTION == 1)
    static U8 MemoryPoolBuffer[MEMPOOL_SIZE] ALIGN(MEMPOOL_CHUNK_SIZE) SECTION(MEMPOOL_FIXED_SIZE_HEAP_SECTION) = { 0 };
#else
    static U8 MemoryPoolBuffer[MEMPOOL_SIZE] ALIGN(MEMPOOL_CHUNK_SIZE) = { 0 };
#endif

static MemPool_InternalType Internal = { 0 };
static Bool MemPool_Enabled = True;

/* --------------------------- Local function definitions -------------------------- */

/**
 * @brief Check if the given address is within the buffer
 *        managed by this module.
 * @param Address Address to be checked.
 * @return True = Address is in the managed buffer,
 *         False = Address is not in the managed buffer.
 */
static Bool MemPool_AddressIsValid(void* Address)
{
    return ((U8*)Address >= Internal.FirstChunk) && ((U8*)Address <= Internal.LastChunk);
}


/**
 * @brief Calculate the required number of chunks for the given size.
 * @param Size Number of desired bytes to allocate.
 * @return Number of chunks required for the allocation.
 */
static U32 MemPool_CalcNofChunksRequired(U32 Size)
{
    U32 NofChunks = Size / MEMPOOL_CHUNK_SIZE;
    if (Size % MEMPOOL_CHUNK_SIZE != 0) { NofChunks++; }
    return NofChunks;
}

/**
 * @brief Determine if the requested allocation is possible.
 * @param ChunksRequired Number of chunks needed for the requested for allocation.
 * @return Index of the first chunk for allocation. Index of -1
 *         indicates that the allocation is not possible.
 */
static S32 MemPool_GetAllocationIndex(U32 ChunksRequired)
{
    S32 FirstChunkIndex = -1;
    Bool FirstChunkFound = False;
    U32 ConsecutiveChunksCnt = 0;

    for (U32 i = 0; i < MEMPOOL_NOF_CHUNKS; i++)
    {
        if (Internal.Chunks[i].Status == CHUNK_STATUS_FREE)
        {
            if (!FirstChunkFound)
            {
                FirstChunkFound = True;
            }

            ConsecutiveChunksCnt++;
            if (ConsecutiveChunksCnt == ChunksRequired)
            {
                FirstChunkIndex = i + 1 - ChunksRequired;
                break;
            }
        }
        else
        {
            ConsecutiveChunksCnt = 0;
            FirstChunkFound = False;
        }
    }
    return FirstChunkIndex;
}

/**
 * @brief Allocate a single chunk of memory from the memory pool.
 * @return Pointer to the start of the allocated memory.
 */
static void* MemPool_AllocateSingleChunk(void)
{
    void* ChunkPtr = NULL;
    for (U32 i = 0; i < MEMPOOL_NOF_CHUNKS; i++)
    {
        if (Internal.Chunks[i].Status == CHUNK_STATUS_FREE)
        {
            ChunkPtr = (void*)&Internal.MemoryPool[i * MEMPOOL_CHUNK_SIZE];
            Internal.Chunks[i].Status = CHUNK_STATUS_ALLOC_STANDALONE;
            Internal.ChunksAvailable--;
            break;
        }
    }
    return ChunkPtr;
}

/**
 * @brief Zero-fill the memory of the chunk at the given index.
 * @param Index Chunk index.
 */
void MemPool_ClearChunk(U8 Index)
{
    for (U8 i = 0; i < MEMPOOL_CHUNK_SIZE; i++)
    {
        Internal.Chunks[Index].Data[i] = 0;
    }
}

void MemPool_Init(void)
{
    if (!Internal.Initialized)
    {
        Internal.MemoryPool = MemoryPoolBuffer;
        Internal.ChunksAvailable = MEMPOOL_NOF_CHUNKS;
        for (U32 i = 0; i < Internal.ChunksAvailable; i++)
        {
            Internal.Chunks[i].Status = CHUNK_STATUS_FREE;
            Internal.Chunks[i].Index = i;
            Internal.Chunks[i].Data = &Internal.MemoryPool[i * MEMPOOL_CHUNK_SIZE];
        }
        Internal.FirstChunk = Internal.Chunks[0U].Data;
        Internal.LastChunk = Internal.Chunks[(MEMPOOL_NOF_CHUNKS - 1U)].Data;
        Internal.Initialized = True;
        #if MEMPOOL_ENABLE_HIGH_WATER_MARK == 1
            Internal.HighWaterMark = 0UL;
        #endif /* MEMPOOL_ENABLE_HIGH_WATER_MARK == 1 */
    }
}

void* MemPool_Allocate(U32 Size)
{
    void* ChunkPtr = NULL;
    if (!Internal.Initialized || !MemPool_Enabled) { return ChunkPtr; }
    const U32 ChunksRequired = MemPool_CalcNofChunksRequired(Size);

    if (ChunksRequired <= Internal.ChunksAvailable && ChunksRequired > 0)
    {
        if (ChunksRequired == 1)
        {
            ChunkPtr = MemPool_AllocateSingleChunk();
        }
        else
        {
            S32 FirstChunkIndex = MemPool_GetAllocationIndex(ChunksRequired);
            if (FirstChunkIndex >= 0)
            {
                ChunkPtr = (void*)Internal.Chunks[FirstChunkIndex].Data;
                const U32 AllocationBoundary = FirstChunkIndex + ChunksRequired;
                U32 ChunksAllocatedCnt = 0;

                for (U32 i = FirstChunkIndex; i < AllocationBoundary; i++)
                {
                    ChunksAllocatedCnt++;
                    if (ChunksAllocatedCnt == 1)
                    {
                        Internal.Chunks[i].Status = CHUNK_STATUS_ALLOC_FIRST;
                    }
                    else if (ChunksAllocatedCnt > 1 && ChunksAllocatedCnt < ChunksRequired)
                    {
                        Internal.Chunks[i].Status = CHUNK_STATUS_ALLOC_SECTION;
                    }
                    else
                    {
                        Internal.Chunks[i].Status = CHUNK_STATUS_ALLOC_LAST;
                    }
                    Internal.ChunksAvailable--;
                }
            }
        }
    }

    #if MEMPOOL_ENABLE_HIGH_WATER_MARK == 1
        const U32 CurrentWaterMark = MEMPOOL_NOF_CHUNKS - Internal.ChunksAvailable;
        if (CurrentWaterMark > Internal.HighWaterMark) { Internal.HighWaterMark = CurrentWaterMark; }
    #endif /* MEMPOOL_ENABLE_HIGH_WATER_MARK == 1 */
    return ChunkPtr;
}


void MemPool_Free(void* Address)
{
    if (!Internal.Initialized || !MemPool_Enabled || !MemPool_AddressIsValid(Address)) { return; }

    const U8* ChunkPtr = (U8*)Address;
    Bool Done = False;
    Bool MultiChunk = False;

    for (U32 i = 0; i < MEMPOOL_NOF_CHUNKS; i++)
    {
        if (Internal.Chunks[i].Data == ChunkPtr)
        {
            if (Internal.Chunks[i].Status == CHUNK_STATUS_ALLOC_STANDALONE)
            {
                MemPool_ClearChunk(Internal.Chunks[i].Index);
                Internal.Chunks[i].Status = CHUNK_STATUS_FREE;
                Internal.ChunksAvailable++;
                Done = True;
            }
            else if (Internal.Chunks[i].Status == CHUNK_STATUS_ALLOC_FIRST)
            {
                MemPool_ClearChunk(Internal.Chunks[i].Index);
                Internal.Chunks[i].Status = CHUNK_STATUS_FREE;
                Internal.ChunksAvailable++;
                MultiChunk = True;
            }
            else { /* Should not happen... */ }

        }
        else if (MultiChunk)
        {
            if (Internal.Chunks[i].Status == CHUNK_STATUS_ALLOC_LAST) { Done = True; }
            MemPool_ClearChunk(Internal.Chunks[i].Index);
            Internal.Chunks[i].Status = CHUNK_STATUS_FREE;
            Internal.ChunksAvailable++;
        }
        else { /* Continue... */ }

        if (Done) { break; }
    }
}

U32 MemPool_GetNofFreeBytes(void)
{
    return Internal.ChunksAvailable * MEMPOOL_CHUNK_SIZE;
}

Bool MemPool_IsInitialized(void)
{
    return Internal.Initialized;
}

void MemPool_Disable(void)
{
    MemPool_Enabled = False;
}

/* --------------- Conditionally compiled public function definitions. ------------- */

#if MEMPOOL_ENABLE_HIGH_WATER_MARK == 1
    U32 MemPool_GetHighWaterMark(void)
    {
        return Internal.HighWaterMark * MEMPOOL_CHUNK_SIZE;
    }
#endif /* MEMPOOL_ENABLE_HIGH_WATER_MARK == 1 */
