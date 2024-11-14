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

typedef struct
{
    U8* Data;
    U8 Index;
    MemPool_ChunkStatusEnum Status;
} MemPool_ChunkType;

static U8 MemoryPool[POOL_SIZE] = { 0 };
static MemPool_ChunkType Chunks[NOF_CHUNKS] = { 0 };
static U8 ChunksAvailable = NOF_CHUNKS;
static U8* FirstChunk;
static U8* LastChunk;
static Bool ModuleInitialized = False;


/**
 * @brief Calculate the required number of chunks for the given size.
 * @param Size Number of desired bytes to allocate.
 * @return Number of chunks required for the allocation.
 */
static U8 MemPool_CalcNofChunksRequired(U16 Size)
{
    U8 Rv = 0;

    if (Size > 0)
    {
        U8 CmpSize = CHUNK_SIZE;
        for (U8 i = 1; i <= NOF_CHUNKS; i++)
        {
            if (Size <= CmpSize)
            {
                Rv = i;
                break;
            }
            else
            {
                CmpSize += CHUNK_SIZE;
            }
        }
    }
    return Rv;
}

/**
 * @brief Determine if the requested allocation is possible.
 * @param Size Number of chunks needed for the requested for allocation.
 * @return Index of the first chunk for allocation. Index of -1
 *         indicates that the allocation is not possible.
 */
static S8 MemPool_GetAllocationIndex(U8 ChunksRequired)
{
    S8 FirstChunkIndex = -1;
    Bool FirstChunkFound = False;
    U8 ConsecutiveChunksCnt = 0;

    for (U8 i = 0; i < NOF_CHUNKS; i++)
    {
        if (Chunks[i].Status == CHUNK_STATUS_FREE)
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
    if (ChunksAvailable > 0)
    {
        for (U8 i = 0; i < NOF_CHUNKS; i++)
        {
            if (Chunks[i].Status == CHUNK_STATUS_FREE)
            {
                ChunkPtr = (void*)&MemoryPool[i * CHUNK_SIZE];
                Chunks[i].Status = CHUNK_STATUS_ALLOC_STANDALONE;
                ChunksAvailable--;
                break;
            }
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
    for (U8 i = 0; i < CHUNK_SIZE; i++)
    {
        Chunks[Index].Data[i] = 0;
    }
}

void MemPool_Init(void)
{
    for (U8 i = 0; i < NOF_CHUNKS; i++)
    {
        Chunks[i].Status = CHUNK_STATUS_FREE;
        Chunks[i].Index = i;
        Chunks[i].Data = &MemoryPool[i * CHUNK_SIZE];
    }
    FirstChunk = Chunks[0].Data;
    LastChunk = Chunks[(NOF_CHUNKS - 1)].Data;
    ModuleInitialized = True;
}


void* MemPool_Allocate(U16 Size)
{
    void* ChunkPtr = NULL;
    if (!ModuleInitialized) { return ChunkPtr; }
    const U8 ChunksRequired = MemPool_CalcNofChunksRequired(Size);

    if (ChunksRequired <= ChunksAvailable && ChunksRequired > 0)
    {
        if (ChunksRequired == 1)
        {
            ChunkPtr = MemPool_AllocateSingleChunk();
        }
        else
        {
            S8 FirstChunkIndex = MemPool_GetAllocationIndex(ChunksRequired);
            if (FirstChunkIndex >= 0)
            {
                ChunkPtr = (void*)Chunks[FirstChunkIndex].Data;
                const U8 AllocationBoundary = FirstChunkIndex + ChunksRequired;
                U8 ChunksAllocatedCnt = 0;

                for (U8 i = FirstChunkIndex; i < AllocationBoundary; i++)
                {
                    ChunksAllocatedCnt++;
                    if (ChunksAllocatedCnt == 1)
                    {
                        Chunks[i].Status = CHUNK_STATUS_ALLOC_FIRST;
                    }
                    else if (ChunksAllocatedCnt > 1 && ChunksAllocatedCnt < ChunksRequired)
                    {
                        Chunks[i].Status = CHUNK_STATUS_ALLOC_SECTION;
                    }
                    else
                    {
                        Chunks[i].Status = CHUNK_STATUS_ALLOC_LAST;
                    }
                    ChunksAvailable--;
                }
            }
        }
    }
    return ChunkPtr;
}


void MemPool_Free(void* Address)
{
    if (!ModuleInitialized) { return; }

    const U8* ChunkPtr = (U8*)Address;
    if (ChunkPtr < FirstChunk || ChunkPtr > LastChunk) { return; }
    Bool Done = False;
    Bool MultiChunk = False;

    for (U8 i = 0; i < NOF_CHUNKS; i++)
    {
        if (Chunks[i].Data == ChunkPtr)
        {
            if (Chunks[i].Status == CHUNK_STATUS_ALLOC_STANDALONE)
            {
                MemPool_ClearChunk(Chunks[i].Index);
                Chunks[i].Status = CHUNK_STATUS_FREE;
                ChunksAvailable++;
                Done = True;
            }
            else if (Chunks[i].Status == CHUNK_STATUS_ALLOC_FIRST)
            {
                MemPool_ClearChunk(Chunks[i].Index);
                Chunks[i].Status = CHUNK_STATUS_FREE;
                ChunksAvailable++;
                MultiChunk = True;
            }
            else { /* Should not happen... */ }

        }
        else if (MultiChunk)
        {
            if (Chunks[i].Status == CHUNK_STATUS_ALLOC_LAST) { Done = True; }
            MemPool_ClearChunk(Chunks[i].Index);
            Chunks[i].Status = CHUNK_STATUS_FREE;
            ChunksAvailable++;
        }
        else { /* Continue... */ }

        if (Done) { break; }
    }
}

U8 MemPool_GetNofAvailableChunks(void)
{
    return ChunksAvailable;
}

#if defined(UNIT_TEST)
void MemPool_Reset(void)
{
    for (U8 i = 0; i < NOF_CHUNKS; i++)
    {
        MemPool_ClearChunk(i);
        Chunks[i].Status = CHUNK_STATUS_FREE;
    }
    ChunksAvailable = NOF_CHUNKS;
}
#endif /* UNIT_TEST */

