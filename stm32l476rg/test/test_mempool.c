#include "mempool.h"
#include "unity.h"

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
    U8* ExpectedNullBuffer = (U8*)MemPool_Allocate(CHUNK_SIZE);
    MemPool_Init();
    U8* ExpectedValidBuffer = (U8*)MemPool_Allocate(CHUNK_SIZE);
    TEST_ASSERT_NULL(ExpectedNullBuffer);
    TEST_ASSERT_NOT_NULL(ExpectedValidBuffer);
}

void Test_AllocateOneChunk(void)
{
    U8 AvailableChunksAtStart = MemPool_GetNofAvailableChunks();
    U8* Buffer = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8 AvailableChunksAfterAllocation = MemPool_GetNofAvailableChunks();
    TEST_ASSERT_EQUAL(NOF_CHUNKS, AvailableChunksAtStart);
    TEST_ASSERT_NOT_NULL(Buffer);
    TEST_ASSERT_EQUAL(NOF_CHUNKS - 1, AvailableChunksAfterAllocation);
}

void Test_TooLargeAllocationFails(void)
{
    U8* TooLargeBuffer = (U8*)MemPool_Allocate(POOL_SIZE + 1);
    TEST_ASSERT_NULL(TooLargeBuffer);
    TEST_ASSERT_EQUAL(NOF_CHUNKS, MemPool_GetNofAvailableChunks());
}

void Test_SizeZeroAllocationFails(void)
{
    U8* ExpectedNullBuffer = (U8*)MemPool_Allocate(0);
    TEST_ASSERT_NULL(ExpectedNullBuffer);
    TEST_ASSERT_EQUAL(NOF_CHUNKS, MemPool_GetNofAvailableChunks());
}

void Test_AllocateTwoChunks(void)
{
    U8 AvailableChunksAtStart = MemPool_GetNofAvailableChunks();
    U8* Buffer = (U8*)MemPool_Allocate(CHUNK_SIZE * 2);
    TEST_ASSERT_EQUAL(AvailableChunksAtStart, NOF_CHUNKS);
    TEST_ASSERT_NOT_NULL(Buffer);
    TEST_ASSERT_EQUAL(NOF_CHUNKS - 2, MemPool_GetNofAvailableChunks());
}

void Test_AllocateTwoSeparateTwoChunkAllocations(void)
{
    U8 AvailableChunksAtStart = MemPool_GetNofAvailableChunks();
    U8* Buffer1 = (U8*)MemPool_Allocate(CHUNK_SIZE * 2);
    U8* Buffer2 = (U8*)MemPool_Allocate(CHUNK_SIZE * 2);
    TEST_ASSERT_EQUAL(AvailableChunksAtStart, NOF_CHUNKS);
    TEST_ASSERT_NOT_NULL(Buffer1);
    TEST_ASSERT_NOT_NULL(Buffer2);
    TEST_ASSERT_EQUAL(NOF_CHUNKS - 4, MemPool_GetNofAvailableChunks());
}

void Test_AllocateTwoSeparateTwoChunkAllocationsThenFree(void)
{
    U8* Buffer1 = (U8*)MemPool_Allocate(CHUNK_SIZE * 2);
    U8* Buffer2 = (U8*)MemPool_Allocate(CHUNK_SIZE * 2);
    TEST_ASSERT_NOT_NULL(Buffer1);
    TEST_ASSERT_NOT_NULL(Buffer2);
    MemPool_Free(Buffer1);
    TEST_ASSERT_EQUAL(NOF_CHUNKS - 2, MemPool_GetNofAvailableChunks());
    MemPool_Free(Buffer2);
    TEST_ASSERT_EQUAL(NOF_CHUNKS, MemPool_GetNofAvailableChunks());
}

void Test_AllocatiorHandlesSimpleFragmentation(void)
{
    U8* Buffer1 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer2 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer3 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer4 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer5 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer6 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer7 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer8 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    TEST_ASSERT_EQUAL(0, MemPool_GetNofAvailableChunks());

    MemPool_Free(Buffer4);
    MemPool_Free(Buffer5);
    MemPool_Free(Buffer6);
    U8* Buffer9 = (U8*)MemPool_Allocate(CHUNK_SIZE * 3);
    TEST_ASSERT_NOT_NULL(Buffer9);
}

void Test_AllocationFailsIfNotContiguousTwoChunks(void)
{
    U8* Buffer1 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer2 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer3 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer4 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer5 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer6 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer7 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer8 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    TEST_ASSERT_EQUAL(0, MemPool_GetNofAvailableChunks());

    MemPool_Free(Buffer2);
    MemPool_Free(Buffer4);
    U8* Buffer9 = (U8*)MemPool_Allocate(CHUNK_SIZE * 2);
    TEST_ASSERT_NULL(Buffer9);
    TEST_ASSERT_EQUAL(2, MemPool_GetNofAvailableChunks());
}

void Test_AllocationFailsIfNotContiguousThreeChunks(void)
{
    U8* Buffer1 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer2 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer3 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer4 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer5 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer6 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer7 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    U8* Buffer8 = (U8*)MemPool_Allocate(CHUNK_SIZE);
    TEST_ASSERT_EQUAL(0, MemPool_GetNofAvailableChunks());

    MemPool_Free(Buffer2);
    MemPool_Free(Buffer3);
    MemPool_Free(Buffer5);
    U8* Buffer9 = (U8*)MemPool_Allocate(CHUNK_SIZE * 3);
    TEST_ASSERT_NULL(Buffer9);
    TEST_ASSERT_EQUAL(3, MemPool_GetNofAvailableChunks());
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

    return UNITY_END();
}