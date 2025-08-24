#include "cobs_codec.h"
#include "unity.h"

#define UNIT_TEST_BUFFER_SIZE (1024U)
static U8 EncodeBuffer[UNIT_TEST_BUFFER_SIZE] = { 0 };
static U8 DecodeBuffer[UNIT_TEST_BUFFER_SIZE] = { 0 };

/* ----------------------- Unity setup & teardown functions ------------------------ */

void setUp(void)
{
    for (U16 i = 0; i < UNIT_TEST_BUFFER_SIZE; i++)
    {
        EncodeBuffer[i] = 0;
        DecodeBuffer[i] = 0;
    }
    return;
}

void tearDown(void)
{
    return;
}

/* -------- Unit tests, test encoding & decoding of all examples listed at:
            https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing -------- */

void Test_CodecExample1(void)
{
    U8 Payload[] = { 0x00 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    U8 Expected[] = { 0x01, 0x01, 0x00 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample2(void)
{
    U8 Payload[] = { 0x00, 0x00 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    U8 Expected[] = { 0x01, 0x01, 0x01, 0x00 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample3(void)
{
    U8 Payload[] = { 0x00, 0x11, 0x00 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    U8 Expected[] = { 0x01, 0x02, 0x11, 0x01, 0x00 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample4(void)
{
    U8 Payload[] = { 0x11, 0x22, 0x00, 0x33 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    U8 Expected[] = { 0x03, 0x11, 0x22, 0x02, 0x33, 0x00 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample5(void)
{
    U8 Payload[] = { 0x11, 0x22, 0x33, 0x44 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    U8 Expected[] = { 0x05, 0x11, 0x22, 0x33, 0x44, 0x00 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample6(void)
{
    U8 Payload[] = { 0x11, 0x00, 0x00, 0x00 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    U8 Expected[] = { 0x02, 0x11, 0x01, 0x01, 0x01, 0x00 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample7(void)
{
    U8 Payload[254] = { 0 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    for (U16 i = 0; i < PayloadSize; i++)
    {
        Payload[i] = i + 1U;
    }

    U8 Expected[256] = { 0 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));
    for (U16 i = 0; i < ExpectedSize; i++)
    {
        if (i == 0) { Expected[i] = 0xFF; }
        else if (i == 255) { Expected[i] = 0x00; }
        else { Expected[i] = i; }
    }

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample8(void)
{
    U8 Payload[255] = { 0 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    for (U16 i = 0; i < PayloadSize; i++)
    {
        Payload[i] = i;
    }

    U8 Expected[257] = { 0 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));
    for (U16 i = 0; i < ExpectedSize; i++)
    {
        if (i == 0) { Expected[i] = 0x01; }
        else if (i == 1) { Expected[i] = 0xFF; }
        else if (i == 256) { Expected[i] = 0x00; }
        else { Expected[i] = i - 1; }
    }

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample9(void)
{
    U8 Payload[255] = { 0 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    for (U16 i = 0; i < PayloadSize; i++)
    {
        Payload[i] = i + 1;
    }

    U8 Expected[258] = { 0 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));
    for (U16 i = 0; i < ExpectedSize; i++)
    {
        if (i == 0) { Expected[i] = 0xFF; }
        else if (i == 255) { Expected[i] = 0x02; }
        else if (i == 256) { Expected[i] = 0xFF; }
        else if (i == 257) { Expected[i] = 0x00; }
        else { Expected[i] = i; }
    }

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample10(void)
{
    U8 Payload[255] = { 0 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    for (U16 i = 0; i < PayloadSize - 1; i++)
    {
        Payload[i] = i + 2;
    }
    Payload[254] = 0x00;

    U8 Expected[258] = { 0 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));
    for (U16 i = 0; i < ExpectedSize; i++)
    {
        if (i == 0) { Expected[i] = 0xFF; }
        else if (i == 255) { Expected[i] = 0x01; }
        else if (i == 256) { Expected[i] = 0x01; }
        else if (i == 257) { Expected[i] = 0x00; }
        else { Expected[i] = i + 1; }
    }
    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecExample11(void)
{
    U8 Payload[255] = { 0 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    for (U16 i = 0; i < PayloadSize - 2; i++)
    {
        Payload[i] = i + 3;
    }
    Payload[253] = 0x00;
    Payload[254] = 0x01;

    U8 Expected[257] = { 0 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));
    for (U16 i = 0; i < ExpectedSize; i++)
    {
        if (i == 0) { Expected[i] = 0xFE; }
        else if (i == 254) { Expected[i] = 0x02; }
        else if (i == 255) { Expected[i] = 0x01; }
        else if (i == 256) { Expected[i] = 0x00; }
        else { Expected[i] = i + 2; }
    }

    CobsCodec_ResultType EncodeResult = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    CobsCodec_ResultType DecodeResult = CobsCodec_Decode(EncodeBuffer, EncodeResult.Length, DecodeBuffer, UNIT_TEST_BUFFER_SIZE);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, EncodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, EncodeResult.Length);
    TEST_ASSERT_TRUE(EncodeResult.Valid);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Payload, DecodeBuffer, DecodeResult.Length);
    TEST_ASSERT_EQUAL_UINT32(PayloadSize, DecodeResult.Length);
    TEST_ASSERT_TRUE(DecodeResult.Valid);
}

void Test_CodecHandlesNullPointers(void)
{
    U8 Payload[] = { 0x11, 0x22, 0x00, 0x33 };
    U16 PayloadSize = (U16)(sizeof(Payload) / sizeof(Payload[0]));
    U8* InvalidSrcPtr = NULL;
    U8* InvalidDstPtr = NULL;
    U8 Expected[] = { 0x03, 0x11, 0x22, 0x02, 0x33, 0x00 };
    U16 ExpectedSize = (U16)(sizeof(Expected) / sizeof(Expected[0]));

    CobsCodec_ResultType InvalidSrcPtrResult = CobsCodec_Encode(InvalidSrcPtr, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    TEST_ASSERT_FALSE(InvalidSrcPtrResult.Valid);
    TEST_ASSERT_EQUAL_UINT32(0UL, InvalidSrcPtrResult.Length);

    CobsCodec_ResultType InvalidDstPtrResult = CobsCodec_Encode(Payload, PayloadSize, InvalidDstPtr, UNIT_TEST_BUFFER_SIZE);
    TEST_ASSERT_FALSE(InvalidDstPtrResult.Valid);
    TEST_ASSERT_EQUAL_UINT32(0UL, InvalidDstPtrResult.Length);

    CobsCodec_ResultType Result = CobsCodec_Encode(Payload, PayloadSize, EncodeBuffer, UNIT_TEST_BUFFER_SIZE);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Expected, EncodeBuffer, Result.Length);
    TEST_ASSERT_EQUAL_UINT32(ExpectedSize, Result.Length);
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(Test_CodecExample1);
    RUN_TEST(Test_CodecExample2);
    RUN_TEST(Test_CodecExample3);
    RUN_TEST(Test_CodecExample4);
    RUN_TEST(Test_CodecExample5);
    RUN_TEST(Test_CodecExample6);
    RUN_TEST(Test_CodecExample7);
    RUN_TEST(Test_CodecExample8);
    RUN_TEST(Test_CodecExample9);
    RUN_TEST(Test_CodecExample10);
    RUN_TEST(Test_CodecExample11);
    RUN_TEST(Test_CodecHandlesNullPointers);

    return UNITY_END();
}
