/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : Testsuite.h                                                *
 *  Description : Sample testsuite                                           *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */

#include "System.h"
#include "uCUnit-v1.0.h"
#include "Testsuite.h"
#include "circular_buffer.h"

//static void Test_BasicChecksDemo(void)
//{
//    char * s = NULL;
//    int a = 0;
//    int b = 0;
//    int c = 0;
//
//    UCUNIT_TestcaseBegin("DEMO:Simple Checks");
//    UCUNIT_CheckIsEqual( 0, a ); /* Pass*/
//    UCUNIT_CheckIsEqual( 0, b ); /* Pass */
//    UCUNIT_CheckIsEqual( 0, c ); /* Pass */
//
//    UCUNIT_CheckIsEqual( 0, (1-1) ); /* Pass */
//    UCUNIT_CheckIsEqual( 0, (-1+1) ); /* Pass */
//    UCUNIT_CheckIsEqual( -1, (-2+1) ); /* Pass */
//
//    UCUNIT_CheckIsNull(s); /* Pass */
//    UCUNIT_TestcaseEnd();
//}
//
//static void Test_PointersDemo(void)
//{
//    char * s = NULL;
//
//    UCUNIT_TestcaseBegin("DEMO:Checking pointers");
//
//    UCUNIT_CheckIsNull(s); /* Pass */
//
//    /* Empty string has a 0 byte/word as end of string */
//    s = "";
//
//    UCUNIT_CheckIsNotNull(s); /* Pass */
//    UCUNIT_Check( (*s)=='\00' , "No end-of-string found","s" ); /* Pass */
//
//    /* Check if first character of string "Hello World!\n" is a 'H' */
//    s = "Hello World!\n";
//    UCUNIT_CheckIsNotNull(s); /* Pass */
//    UCUNIT_Check( (*s)=='H' , "String starts not with 'H'","s" ); /* Pass */
//
//    UCUNIT_TestcaseEnd(); /* Pass */
//}
//
//static void Test_ChecklistDemo(void)
//{
//    int a = 0;
//    int b = 0;
//    int c = 0;
//
//    UCUNIT_TestcaseBegin("DEMO:Checklists");
//
//    /* Check if calculation works */
//    a = 10;
//    b = -20;
//    c = a + b;
//
//    UCUNIT_ChecklistBegin(UCUNIT_ACTION_WARNING);
//    UCUNIT_CheckIsEqual( -10, c ); /* Pass */
//    UCUNIT_CheckIsInRange(a,0,10); /* Pass */
//    UCUNIT_CheckIsInRange(a,11,20); /* Fail */
//    UCUNIT_CheckIsInRange(a,0,9); /* Fail */
//    UCUNIT_ChecklistEnd();
//
//    UCUNIT_TestcaseEnd(); /* Fail */
//}
//
//static void Test_BitChecksDemo(void)
//{
//    UCUNIT_TestcaseBegin("DEMO:Checking Bits");
//
//    UCUNIT_CheckIsBitSet(0x0001, 0); /* Pass */
//    UCUNIT_CheckIsBitSet(0x0000, 0); /* Fail */
//    UCUNIT_CheckIsBitSet(0x0002, 1); /* Pass */
//    UCUNIT_CheckIsBitSet(0x0000, 6); /* Fail */
//    UCUNIT_CheckIsBitSet(0xFFFF, 6); /* Pass */
//
//    UCUNIT_CheckIsBitClear(0x0001, 0); /* Fail */
//    UCUNIT_CheckIsBitClear(0x0001, 1); /* Pass */
//
//    UCUNIT_TestcaseEnd(); /* Fail */
//}
//
//static void Test_CheckTracepointsDemo(void)
//{
//
//    int found = FALSE;
//    int i = 0;
//    unsigned short a = 0;
//
//    UCUNIT_TestcaseBegin("DEMO:Tracepoints");
//
//    a = 0x123BUFFER_TEST_SIZE;
//    UCUNIT_ResetTracepointCoverage();
//    UCUNIT_Tracepoint(0); /* Pass */
//    UCUNIT_CheckIs8Bit(a); /* Fail */
//    UCUNIT_CheckIs16Bit(a); /* Pass */
//    UCUNIT_CheckIs32Bit(a); /* Pass */
//    UCUNIT_Tracepoint(1); /* Pass */
//
//    UCUNIT_CheckTracepointCoverage(0); /* Pass */
//    UCUNIT_CheckTracepointCoverage(1); /* Pass */
//
//    UCUNIT_ResetTracepointCoverage();
//    for (i = 0; (i < 50) && (!found); i++)
//    {
//        if (i == 25)
//        {
//            UCUNIT_Tracepoint(0); /* Never executed */
//            break;
//        }
//
//        if (i == 10)
//        {
//            UCUNIT_Tracepoint(1); /* Executed */
//            found = TRUE;
//        }
//
//        if (i == 15)
//        {
//            UCUNIT_Tracepoint(2); /* Never executed */
//            found = TRUE;
//        }
//    }
//
//    UCUNIT_CheckTracepointCoverage(0); /* Fail  */
//    UCUNIT_CheckTracepointCoverage(1); /* Pass */
//    UCUNIT_CheckTracepointCoverage(2); /* Fail */
//
//    UCUNIT_TestcaseEnd(); /* Fail */
//}


/* 	T E S T 	C A S E S	*/

void Test_CircularBufferCreate(void)
{
	char* Old_buf;
	CircularBuffer_t * buffer = CircBufCreate();
	UCUNIT_TestcaseBegin("Create Circular Buffer");
	UCUNIT_CheckIsNotNull(buffer);
	UCUNIT_CheckIsNull(buffer->buffer_start);

	/* Init Circular Buffer with size of BUFFER_TEST_SIZE */
	CircBufInit(buffer, BUFFER_TEST_SIZE);
	UCUNIT_CheckIsNotNull(buffer->buffer_start);

	/* Save the value of the buffer */

	Old_buf = buffer->buffer_start;

	/* Free Buffer */
	CircBufDestroy(buffer);

	UCUNIT_CheckIsNull(Old_buf);
	UCUNIT_CheckIsNull(buffer);

	UCUNIT_TestcaseEnd();


}


void Test_BasicBuffer(void)
{
	int i = 0;
	char bufOut;
	char string[BUFFER_TEST_SIZE] = {'a', 'b', 'c', 'd'};
	CircBufferReturn_t bufStatus;
	CircularBuffer_t * buffer = CircBufCreate();
	UCUNIT_TestcaseBegin("Basic Buffer Test");

	/* Init buffer and ensure success */
	bufStatus = CircBufInit(buffer, BUFFER_TEST_SIZE);
	UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);

	/* Add characters to capacity */
	for(i=0 ; i<BUFFER_TEST_SIZE ; i++)
	{
		bufStatus = CircBufAdd(buffer, string[i]);
		UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);
	}

	/* Remove elements and ensure equality */
	for(i=0; i<BUFFER_TEST_SIZE; i++)
	{
		bufStatus = CircBufRemove(buffer, &bufOut);
		UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);
		UCUNIT_CheckIsEqual(string[i], bufOut);
	}

	CircBufDestroy(buffer);
	UCUNIT_TestcaseEnd();
}

void Test_CircularBufferFull()
{
	char string[BUFFER_TEST_SIZE] = {'a', 'b', 'c', 'd'};
	int i;
	CircBufferReturn_t bufStatus;
	CircularBuffer_t * buffer = CircBufCreate();
	UCUNIT_TestcaseBegin("Buffer Full Test");

	/* Init buffer and ensure success */
	bufStatus = CircBufInit(buffer, BUFFER_TEST_SIZE);
	UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);

	/* Add characters to capacity */
	for(i=0 ; i<BUFFER_TEST_SIZE ; i++)
	{
		bufStatus = CircBufAdd(buffer, string[i]);
		UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);
	}

	bufStatus = CircBufAdd(buffer, string[0]);
	UCUNIT_CheckIsEqual(bufStatus, BUF_FULL);

	CircBufDestroy(buffer);
	UCUNIT_TestcaseEnd();
}

void Test_CircularBufferEmpty(void)
{
	CircBufferReturn_t bufStatus;
	char bufOut;
	CircularBuffer_t * buffer = CircBufCreate();
	UCUNIT_TestcaseBegin("Buffer Empty Test");

	/* Init buffer and ensure success */
	bufStatus = CircBufInit(buffer, BUFFER_TEST_SIZE);
	UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);

	/* Try to remove element from empty buffer */
	bufStatus = CircBufRemove(buffer, &bufOut);
	UCUNIT_CheckIsEqual(bufStatus, BUF_EMPTY);

	CircBufDestroy(buffer);
	UCUNIT_TestcaseEnd();
}

void Test_CircularBufferWrapAdd(void)
{
	char string[BUFFER_TEST_SIZE + 1] = {'a', 'b', 'c', 'd', 'e'};
	char bufOut;
	int i;
	CircBufferReturn_t bufStatus;
	CircularBuffer_t * buffer = CircBufCreate();
	UCUNIT_TestcaseBegin("Buffer Wrap Add Test");

	/* Init buffer and ensure success */
	bufStatus = CircBufInit(buffer, BUFFER_TEST_SIZE);
	UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);

	/* Add characters to capacity */
	for(i=0 ; i<BUFFER_TEST_SIZE ; i++)
	{
		bufStatus = CircBufAdd(buffer, string[i]);
		UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);
	}

	/* Take one element out of buffer */
	bufStatus = CircBufRemove(buffer, &bufOut);
	UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);
	UCUNIT_CheckIsEqual(string[0], bufOut);

	/* Add another element to buffer - should wrap around to beginning */
	bufStatus = CircBufAdd(buffer, string[i]);
	/* Check that the last element added is at the beginning of the buffer */
	UCUNIT_CheckIsEqual(*(buffer->buffer_start), string[i]);

	CircBufDestroy(buffer);
	UCUNIT_TestcaseEnd();
}

void Test_CircularBufferWrapRemove(void)
{
	char string[BUFFER_TEST_SIZE + 1] = {'a', 'b', 'c', 'd', 'e'};
	char bufOut;
	int i;
	CircBufferReturn_t bufStatus;
	CircularBuffer_t * buffer = CircBufCreate();
	UCUNIT_TestcaseBegin("Buffer Wrap Remove Test");

	/* Init buffer and ensure success */
	bufStatus = CircBufInit(buffer, BUFFER_TEST_SIZE);
	UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);

	/* Add characters to capacity */
	for(i=0 ; i<BUFFER_TEST_SIZE ; i++)
	{
		bufStatus = CircBufAdd(buffer, string[i]);
		UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);
	}

	/* Take one element out of buffer */
	bufStatus = CircBufRemove(buffer, &bufOut);
	UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);
	UCUNIT_CheckIsEqual(string[0], bufOut);

	/* Add another element to buffer - should wrap around to beginning */
	bufStatus = CircBufAdd(buffer, string[i]);

	bufStatus = BUF_SUCCESS;
	i = 0;
	/* Remove elements until buffer is empty */
	while(bufStatus == BUF_SUCCESS)
	{
		bufStatus = CircBufRemove(buffer, &bufOut);
	}

	UCUNIT_CheckIsEqual(bufStatus, BUF_EMPTY);

	/* If Wrap Add test works, then if head and tail are equal then tail must have wrapped */
	UCUNIT_CheckIsEqual(buffer->head, buffer->tail);

	CircBufDestroy(buffer);
	UCUNIT_TestcaseEnd();

}
void Testsuite_RunTests(void)
{
    Test_CircularBufferCreate();
    Test_BasicBuffer();
    Test_CircularBufferFull();
    Test_CircularBufferEmpty();
    Test_CircularBufferWrapAdd();
    Test_CircularBufferWrapRemove();

    UCUNIT_WriteSummary();
}

int UCUNIT_RUN(void)
{
    UCUNIT_Init();
    UCUNIT_WriteString("\n**************************************");
    UCUNIT_WriteString("\nName:     ");
    UCUNIT_WriteString("Circular Buffer UCUNIT test");
    UCUNIT_WriteString("\nCompiled: ");
    UCUNIT_WriteString(__DATE__);
    UCUNIT_WriteString("\nTime:     ");
    UCUNIT_WriteString(__TIME__);
    UCUNIT_WriteString("\nVersion:  ");
    UCUNIT_WriteString(UCUNIT_VERSION);
    UCUNIT_WriteString("\n**************************************");
    Testsuite_RunTests();
    UCUNIT_Shutdown();

    return 0;
}
