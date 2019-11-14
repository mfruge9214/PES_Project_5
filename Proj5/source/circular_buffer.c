/* Includes */

#include "circular_buffer.h"
#include "logger.h"
#include <stdlib.h>


/* Defines */



/* Functions */

CircularBuffer_t* CircBufCreate()
{
	return (CircularBuffer_t*) malloc(sizeof(CircularBuffer_t));
}

CircBufferReturn_t CircBufInit(CircularBuffer_t * buf, uint8_t size)
{
	/* Allocate requested memory */
	buf->buffer_start = (char*) malloc(sizeof(char) * size);

	/* Make sure memory is valid */
	if(!buf->buffer_start)
	{
		return BUF_FAIL;
	}

	/* Set first and last char ptrs, capacity, and length to initial values */
	buf->head = buf->buffer_start;
	buf->tail = buf->buffer_start;
	buf->capacity = size;
	buf->length = 0;

	return BUF_SUCCESS;
}


CircBufferReturn_t CircBufRealloc(CircularBuffer_t * buf)
{
	/* Check the buffer is actually full */
	if(buf->capacity != buf->length)
	{
		logString(LL_Debug, FN_CircBufRealloc, "Buffer not full, so not reallocating");
		return BUF_FAIL;
	}

	/* Reallocate the memory */

	char* temp;
	temp = (char*) realloc(buf->buffer_start, buf->capacity * BUFSIZE_MULT);
	buf->buffer_start = temp;

	/* Ensure reallocation worked */
	if(CircBufIsValid(buf))
	{
		logString(LL_Debug, FN_CircBufRealloc, "Re-allocation Failed");
		return BUF_FAIL;
	}

	/* Adjust values to reflect change */
	buf->capacity *= BUFSIZE_MULT;


	/*	TODO: Need to re-fill the buffer with what it had before */


	return BUF_SUCCESS;

}


CircBufferReturn_t	CircBufAdd(CircularBuffer_t * buf, char c)
{
	CircBufferReturn_t ret;

	/* Check that the buffer is not full */
	if(CircBufIsFull(buf) == BUF_FULL)
	{
		logString(LL_Debug, FN_CircBufAdd, "Buffer full");

		ret = BUF_FULL;

		if(REALLOCATE_BUFFER)
		{
			ret = CircBufRealloc(buf);
		}

		else return ret;
	}

	/* Add element by placing into current tail position and moving tail forward 1 or wrapping */
	*(buf->head) = c;
	(buf->head)++;
	(buf->length)++;


	char* bufend = (char*) buf->buffer_start + (sizeof(char) * buf->capacity);

	/* Check if it needs to be wrapped to the beginning */
	if(buf->head == bufend)
	{
		buf->head = buf->buffer_start;
	}


	return BUF_SUCCESS;
}


CircBufferReturn_t	CircBufRemove(CircularBuffer_t * buf, char * charOut)
{
	if(CircBufIsEmpty(buf) == BUF_EMPTY)
	{
		logString(LL_Debug, FN_CircBufRemove, "Buffer Empty");
		return BUF_EMPTY;
	}

	/* TODO: Make this section a critical section */
	/* Extract the data into the charOut parameter */
	*charOut = *(buf->tail);
	(buf->tail)++;
	(buf->length)--;

	char* bufend = (char*) buf->buffer_start + (sizeof(char) * buf->capacity);

	/* Check if it needs to be wrapped to the beginning */
	if(buf->tail == bufend)
	{
		buf->tail = buf->buffer_start;
	}

	return BUF_SUCCESS;
}


CircBufferReturn_t	CircBufIsFull(CircularBuffer_t * buf)
{
	if((buf->capacity == buf->length) && (buf->head == buf->tail))
	{
		return BUF_FULL;
	}

	else return BUF_SUCCESS;
}


CircBufferReturn_t	CircBufIsEmpty(CircularBuffer_t * buf)
{
	if((buf->length == 0) && (buf->head == buf->tail))
		{
			return BUF_EMPTY;
		}
	else return BUF_SUCCESS;
}


CircBufferReturn_t	CircBufIsValid(CircularBuffer_t * buf)
{
	if(buf->buffer_start) return BUF_SUCCESS;
	else return BUF_FAIL;
}

CircBufferReturn_t	CircBufIsInitialized(CircularBuffer_t * buf)
{
	/* Ensure the buffer pointers are all valid */
	if( buf->buffer_start && buf->head && buf->tail)
	{
		return BUF_SUCCESS;
	}

	else return BUF_FAIL;
}

CircBufferReturn_t CircBufDestroy(CircularBuffer_t * buf)
{
	free(buf->buffer_start);

	free(buf);

	return BUF_SUCCESS;
}
