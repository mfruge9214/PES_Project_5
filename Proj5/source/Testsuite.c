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


void Test_CircularBufferRealloc(void)
{
	int i = 0;
	char bufOut;
	char string[BUFFER_TEST_SIZE + 1] = {'a', 'b', 'c', 'd', 'e'};
	CircBufferReturn_t bufStatus;
	CircularBuffer_t * buffer = CircBufCreate();
	UCUNIT_TestcaseBegin("Basic Buffer Test");

	/* Init buffer and ensure success */
	bufStatus = CircBufInit(buffer, BUFFER_TEST_SIZE);
	UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);

	/* Obtain values to check */
//	char* old_bufend = old_bufend = (char*) buf->buffer_start + (sizeof(char) * buf->capacity);

	/* Add characters to capacity */
	for(i=0 ; i<BUFFER_TEST_SIZE ; i++)
	{
		bufStatus = CircBufAdd(buffer, string[i]);
		UCUNIT_CheckIsEqual(bufStatus, BUF_SUCCESS);
	}

	/* Add one more character to force buffer reallocation */
	bufStatus = CircBufAdd(buffer, string[i]);

	UCUNIT_CheckIsEqual(buffer->capacity, BUFFER_TEST_SIZE * 2);
	UCUNIT_CheckIsEqual(buffer->length, BUFFER_TEST_SIZE + 1);
//	UCUNIT_CheckIsEqual(buffer->tail, old_buffend);

	i=0;
	while(CircBufIsEmpty(buffer) != BUF_EMPTY)
	{
		CircBufRemove(buffer, &bufOut);
		UCUNIT_CheckIsEqual(bufOut, string[i]);
	}

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
    Test_CircularBufferRealloc();

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
