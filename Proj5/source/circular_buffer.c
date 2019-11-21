/* Includes */

#include "circular_buffer.h"
#include "Systick.h"
#include "logger.h"
#include <stdlib.h>


/* Defines */



/* Functions */


/**************************
 * 	CircBufCreate();
 *
 * 	@brief	Allocate a pointer to CircBuf to be referenced throughout
 *
 * 	@returns	Pointer to buffer structure
 *
 **************************/
CircularBuffer_t* CircBufCreate(void)
{
	return (CircularBuffer_t*) malloc(sizeof(CircularBuffer_t));
}


/************************
 * 	CircBufInit
 *
 * 	@brief	Initialize a circular buffer structure
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 * 	@param[in] uint8_t size		Size of buffer
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 *************************/
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
	buf->numReallocs = 0;

	return BUF_SUCCESS;
}



/************************
 * 	CircBufRealloc
 *
 * 	@brief	Reallocate a circular buffer to BUFFSIZE_MULT x capacity
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 * 	@ note	Only called when REALLOCATE_BUFFER is defined as 1
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 *************************/
CircBufferReturn_t CircBufRealloc(CircularBuffer_t * buf)
{
	/* Check the buffer is actually full */
	if(buf->capacity != buf->length)
	{
		logString(LL_Debug, FN_CircBufRealloc, "Buffer not full, so not reallocating\0");
		return BUF_FAIL;
	}

	/* Reallocate the memory */

	logString(LL_Debug, FN_CircBufRealloc, "Reallocating Buffer\0");

	CircularBuffer_t * temp = CircBufCreate();
	temp->buffer_start = (char*) realloc(buf->buffer_start, buf->capacity * BUFSIZE_MULT);

	temp->capacity = buf->capacity * BUFSIZE_MULT;
	temp->head = temp->buffer_start;
	temp->tail = temp->buffer_start;
	temp->length = 0;
	temp->numReallocs = buf->numReallocs + 1;


	START_CRITICAL();

	/* Adjust values to reflect change */

	/* Create holding values */
	char* old_bufend;
	char cTransfer;

	old_bufend = (char*) buf->buffer_start + (sizeof(char) * buf->capacity);

	/* Set new values */

	/* Loop to move characters while keeping them in order */

	do{
		/* Remove the character from the buffer */
		CircBufRemove(buf, &cTransfer);

		CircBufAdd(temp, cTransfer);

	} while(CircBufIsEmpty(buf) != BUF_EMPTY);

	/* Set buf = temp */
	free(buf->buffer_start);

	buf->buffer_start = temp->buffer_start;
	buf->head = temp->head;
	buf->tail = temp->tail;
	buf->numReallocs = temp->numReallocs;
	buf->capacity = temp->capacity;
	buf->length = temp->length;

	free(temp);
	/* Done manipulating interruptible data, so end critical section */
	END_CRITICAL();

	return BUF_SUCCESS;
}

/************************
 * 	CircBufAdd
 *
 * 	@brief	Add an element to the circular buffer
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 * 	@param[in] char c		Element to be added
 *
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 * 					BUF_SUCCESS		Operation Successful
 * 					BUF_FULL		Buffer is full
 *************************/
CircBufferReturn_t	CircBufAdd(CircularBuffer_t * buf, char c)
{
	CircBufferReturn_t ret;

	/* Check that the buffer is not full */
	if(CircBufIsFull(buf) == BUF_FULL)
	{
		logString(LL_Debug, FN_CircBufAdd, "Buffer full\0");

		ret = BUF_FULL;

		if(REALLOCATE_BUFFER && (buf->numReallocs < 5))
		{
			ret = CircBufRealloc(buf);
		}

		else return ret;
	}

	START_CRITICAL();
	/* Add element by placing into current head position and moving head forward 1 or wrapping */
	*(buf->head) = c;
	(buf->head)++;
	(buf->length)++;


	char* bufend = (char*) buf->buffer_start + (sizeof(char) * buf->capacity);

	/* Check if it needs to be wrapped to the beginning */
	if(buf->head == bufend)
	{
		buf->head = buf->buffer_start;
	}

	END_CRITICAL();
	return BUF_SUCCESS;
}



/************************
 * 	CircBufRemove
 *
 * 	@brief	Remove an element from the circular buffer
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 * 	@param[out] char * charOut				Element that was removed
 *
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 * 					BUF_SUCCESS		Operation Successful
 * 					BUF_EMPTY		Buffer is empty
 *************************/
CircBufferReturn_t	CircBufRemove(CircularBuffer_t * buf, char * charOut)
{
	if(CircBufIsEmpty(buf) == BUF_EMPTY)
	{
		logString(LL_Debug, FN_CircBufRemove, "Buffer Empty\0");
		return BUF_EMPTY;
	}

	START_CRITICAL();

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

	END_CRITICAL();

	return BUF_SUCCESS;
}


/************************
 * 	CircBufIsFull
 *
 * 	@brief	Check if the buffer is full
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 *
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 * 					BUF_SUCCESS		Buffer is not full
 * 					BUF_FULL		Buffer is full
 *************************/
CircBufferReturn_t	CircBufIsFull(CircularBuffer_t * buf)
{
	if((buf->capacity == buf->length) && (buf->head == buf->tail))
	{
		return BUF_FULL;
	}

	else return BUF_SUCCESS;
}



/************************
 * 	CircBufIsEmpty
 *
 * 	@brief	Check if the buffer is empty
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 * 					BUF_SUCCESS		Buffer is not empty
 * 					BUF_FULL		Buffer is empty
 *************************/
CircBufferReturn_t	CircBufIsEmpty(CircularBuffer_t * buf)
{
	if((buf->length == 0) && (buf->head == buf->tail))
		{
			return BUF_EMPTY;
		}
	else return BUF_SUCCESS;
}

/************************
 * 	CircBufIsValid
 *
 * 	@brief	Check if the buffer is set to a valid address
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 * 					BUF_SUCCESS		Buffer is valid
 * 					BUF_FAIL		Buffer is not valid
 *************************/
CircBufferReturn_t	CircBufIsValid(CircularBuffer_t * buf)
{
	if(buf->buffer_start) return BUF_SUCCESS;
	else return BUF_FAIL;
}


/************************
 * 	CircBufIsInitialized
 *
 * 	@brief	Check if the buffer is initialized
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 * 	@note		Checks buffer_start, head, and tail pointers to ensure they are not null
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 * 					BUF_SUCCESS		Buffer is initialized
 * 					BUF_FAIL		Buffer is not initialized
 *************************/
CircBufferReturn_t	CircBufIsInitialized(CircularBuffer_t * buf)
{
	/* Ensure the buffer pointers are all valid */
	if( buf->buffer_start && buf->head && buf->tail)
	{
		return BUF_SUCCESS;
	}

	else return BUF_FAIL;
}


/************************
 * 	CircBufDestroy
 *
 * 	@brief	Free all memory allocated to the buffer
 *
 * 	@param[in] CircularBuffer_t * buf		Pointer to CircularBuffer
 *
 * 	@returns	CircBufferReturn_t 	Status of operation
 * 					BUF_SUCCESS		Memory Freed
 *************************/
CircBufferReturn_t CircBufDestroy(CircularBuffer_t * buf)
{
	free(buf->buffer_start);

	free(buf);

	return BUF_SUCCESS;
}
