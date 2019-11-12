/* Includes */

#include "circular_buffer.h"
#include "logger.h"


/* Defines */




/* Functions */

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
	buf->first = buf.buffer_start;
	buf->last = buf.buffer_start;
	buf->capacity = size;
	buf->length = 0;

	return BUF_SUCCESS;
}


CircBufferReturn_t CircBufRealloc(CircularBuffer_t * buf)
{
	/* Check the buffer is actually full */
	if(buf->capacity != buf->length)
	{
		logString(LL_Debug, CircBufRealloc, "Buffer not full, so not reallocating");
		return BUF_FAIL;
	}

	/* Reallocate the memory */

	char* temp;
	temp = (char*) realloc(buf->buffer_start, buf->capacity * BUFSIZE_MULT);
	buf->buffer_start = temp;

	if(! buf->buffer_start)
	{
		logString(LL_Debug, CircBufRealloc, "Re-allocation Failed");
		return BUF_FAIL;
	}

	/* Adjust values to reflect change */
	buf->capacity *= BUFSIZE_MULT;

}


CircBufferReturn_t	CircBufAdd(CircularBuffer_t * buf)
{
	/* Check that the buffer is not full */
}
