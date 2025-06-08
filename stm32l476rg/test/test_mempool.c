#include "mempool.h"
#include "unity.h"

#if (MEMPOOL_ENABLE_HIGH_WATER_MARK == 0)
    #undef MEMPOOL_ENABLE_HIGH_WATER_MARK
    #define MEMPOOL_ENABLE_HIGH_WATER_MARK 1
#endif

#if (MEMPOOL_USE_FIXED_SIZE_HEAP_SECTION == 1)
    #undef MEMPOOL_USE_FIXED_SIZE_HEAP_SECTION
    #define MEMPOOL_USE_FIXED_SIZE_HEAP_SECTION 1
#endif

/* --------------------------- Setup & teardown functions -------------------------- */

void setUp(void)
{

}

void tearDown(void)
{
    MemPool_Reset();
}

/* -------------------------------------- Tests ------------------------------------ */

void Test_UninitializedModuleFailsToAllocate(void)
{
    U8* ExpectedNullBuffer = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE);
    MemPool_Init();
    U8* ExpectedValidBuffer = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE);
    TEST_ASSERT_NULL(ExpectedNullBuffer);
    TEST_ASSERT_NOT_NULL(ExpectedValidBuffer);
}

void Test_AllocateOneChunk(void)
{
    MemPool_Init();
    U32 AvailableBytesAtStart = MemPool_GetNofFreeBytes();
    U8* Buffer = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE);
    U32 AvailableBytesAfterAllocation = MemPool_GetNofFreeBytes();
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE, AvailableBytesAtStart);
    TEST_ASSERT_NOT_NULL(Buffer);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE - MEMPOOL_CHUNK_SIZE, AvailableBytesAfterAllocation);
}

void Test_TooLargeAllocationFails(void)
{
    MemPool_Init();
    U8* TooLargeBuffer = (U8*)MemPool_Allocate(MEMPOOL_SIZE + 1);
    TEST_ASSERT_NULL(TooLargeBuffer);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE, MemPool_GetNofFreeBytes());
}

void Test_SizeZeroAllocationFails(void)
{
    MemPool_Init();
    U8* ExpectedNullBuffer = (U8*)MemPool_Allocate(0);
    TEST_ASSERT_NULL(ExpectedNullBuffer);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE, MemPool_GetNofFreeBytes());
}

void Test_AllocateTwoChunks(void)
{
    MemPool_Init();
    U32 AvailableBytesAtStart = MemPool_GetNofFreeBytes();
    U8* Buffer = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 2);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE, AvailableBytesAtStart);
    TEST_ASSERT_NOT_NULL(Buffer);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE - MEMPOOL_CHUNK_SIZE * 2, MemPool_GetNofFreeBytes());
}

void Test_AllocateTwoSeparateTwoChunkAllocations(void)
{
    MemPool_Init();
    U32 AvailableBytesAtStart = MemPool_GetNofFreeBytes();
    U8* Buffer1 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 2);
    U8* Buffer2 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 2);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE, AvailableBytesAtStart);
    TEST_ASSERT_NOT_NULL(Buffer1);
    TEST_ASSERT_NOT_NULL(Buffer2);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE - MEMPOOL_CHUNK_SIZE * 4, MemPool_GetNofFreeBytes());
    TEST_ASSERT_EQUAL(MEMPOOL_CHUNK_SIZE * 4, MemPool_GetHighWaterMark());
}

void Test_AllocateTwoSeparateTwoChunkAllocationsThenFree(void)
{
    MemPool_Init();
    U8* Buffer1 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 2);
    U8* Buffer2 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 2);
    TEST_ASSERT_NOT_NULL(Buffer1);
    TEST_ASSERT_NOT_NULL(Buffer2);
    MemPool_Free(Buffer1);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE - MEMPOOL_CHUNK_SIZE * 2, MemPool_GetNofFreeBytes());
    MemPool_Free(Buffer2);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE, MemPool_GetNofFreeBytes());
    TEST_ASSERT_EQUAL(MEMPOOL_CHUNK_SIZE * 4, MemPool_GetHighWaterMark());
}

void Test_AllocatiorHandlesSimpleFragmentation(void)
{
    MemPool_Init();
    const U32 QuarterPool = MEMPOOL_NOF_CHUNKS / 4U;
    U8* Buffer1 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * QuarterPool);
    U8* Buffer2 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * QuarterPool);
    U8* Buffer3 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * QuarterPool);
    U8* Buffer4 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * QuarterPool);
    TEST_ASSERT_EQUAL(0, MemPool_GetNofFreeBytes());

    MemPool_Free(Buffer2);
    TEST_ASSERT_EQUAL(QuarterPool * MEMPOOL_CHUNK_SIZE, MemPool_GetNofFreeBytes());

    U8* Buffer5 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * QuarterPool);
    TEST_ASSERT_NOT_NULL(Buffer5);
    TEST_ASSERT_EQUAL(0, MemPool_GetNofFreeBytes());
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE, MemPool_GetHighWaterMark());
}

void Test_AllocationFailsIfNotContiguousTwoChunks(void)
{
    MemPool_Init();
    U8* Buffer1 = (U8*)MemPool_Allocate(MEMPOOL_SIZE - 2 * MEMPOOL_CHUNK_SIZE);
    U8* Buffer2 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE);
    TEST_ASSERT_NOT_NULL(Buffer1);
    TEST_ASSERT_NOT_NULL(Buffer2);
    TEST_ASSERT_EQUAL(MEMPOOL_CHUNK_SIZE, MemPool_GetNofFreeBytes());

    /* Try to allocate two chunks where only one is available. */
    U8* Buffer3 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 2);
    TEST_ASSERT_NULL(Buffer3);
    MemPool_Free(Buffer2);
    Buffer3 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 2);
    TEST_ASSERT_NOT_NULL(Buffer3);
    TEST_ASSERT_EQUAL(0, MemPool_GetNofFreeBytes());
}

void Test_AllocationFailsIfNotContiguousThreeChunks(void)
{
    MemPool_Init();
    U8* Buffer1 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 2);
    U8* Buffer2 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE);
    U8* Buffer3 = (U8*)MemPool_Allocate(MemPool_GetNofFreeBytes());
    TEST_ASSERT_EQUAL(0, MemPool_GetNofFreeBytes());

    /* Attempt to allocate 3 chunks where only 2 are available. */
    MemPool_Free(Buffer2);
    U8* Buffer4 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 3);
    TEST_ASSERT_NULL(Buffer4);

    /* Free up the missing chunk, now it should be ok to allocate 3 chunks. */
    MemPool_Free(Buffer1);
    Buffer4 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE * 3);
    TEST_ASSERT_NOT_NULL(Buffer4);
    TEST_ASSERT_EQUAL(0, MemPool_GetNofFreeBytes());

    /* Free all */
    MemPool_Free(Buffer3);
    MemPool_Free(Buffer4);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE, MemPool_GetNofFreeBytes());
}

void Test_ChunkBoundaries(void)
{
    MemPool_Init();
    U8* Buffer1 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE - 1);
    TEST_ASSERT_NOT_NULL(Buffer1);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE - MEMPOOL_CHUNK_SIZE, MemPool_GetNofFreeBytes());

    U8* Buffer2 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE + 1);
    TEST_ASSERT_NOT_NULL(Buffer2);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE - MEMPOOL_CHUNK_SIZE * 3, MemPool_GetNofFreeBytes());
    TEST_ASSERT_EQUAL(MEMPOOL_CHUNK_SIZE * 3, MemPool_GetHighWaterMark());
}

void Test_LockMemPool(void)
{
    MemPool_Init();
    U8* Buffer1 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE);
    TEST_ASSERT_NOT_NULL(Buffer1);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE - MEMPOOL_CHUNK_SIZE, MemPool_GetNofFreeBytes());

    MemPool_Disable();
    U8* Buffer2 = (U8*)MemPool_Allocate(MEMPOOL_CHUNK_SIZE);
    TEST_ASSERT_NULL(Buffer2);
    TEST_ASSERT_EQUAL(MEMPOOL_SIZE - MEMPOOL_CHUNK_SIZE, MemPool_GetNofFreeBytes());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(Test_UninitializedModuleFailsToAllocate);
    RUN_TEST(Test_AllocateOneChunk);
    RUN_TEST(Test_TooLargeAllocationFails);
    RUN_TEST(Test_SizeZeroAllocationFails);
    RUN_TEST(Test_AllocateTwoChunks);
    RUN_TEST(Test_AllocateTwoSeparateTwoChunkAllocations);
    RUN_TEST(Test_AllocateTwoSeparateTwoChunkAllocationsThenFree);
    RUN_TEST(Test_AllocatiorHandlesSimpleFragmentation);
    RUN_TEST(Test_AllocationFailsIfNotContiguousTwoChunks);
    RUN_TEST(Test_AllocationFailsIfNotContiguousThreeChunks);
    RUN_TEST(Test_ChunkBoundaries);
    RUN_TEST(Test_LockMemPool);

    return UNITY_END();
}