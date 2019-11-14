

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

/* Includes */

#include <stdint.h>

/* Definitions */


#define REALLOCATE_BUFFER	0

#define BUFSIZE				16
#define BUFSIZE_MULT		4

/* Structures */

typedef struct CircularBuffer_t
{
	char* buffer_start;		// Beginning of allocated buffer
	char* head;				// Pointer modified with ADD operations
	char* tail;				// Pointer modified with REMOVE operations
	uint8_t length;			// The current length of the buffer
	uint32_t capacity;		// The character capacity of the buffer
} CircularBuffer_t;


typedef enum
{
	BUF_SUCCESS = 0,
	BUF_FULL = 1,
	BUF_EMPTY = 2,
	BUF_FAIL = 3
} CircBufferReturn_t;


/* Function Prototypes */
CircularBuffer_t * CircBufCreate();
CircBufferReturn_t 	CircBufInit(CircularBuffer_t * buf, uint8_t size);
CircBufferReturn_t 	CircBufRealloc(CircularBuffer_t * buf);
CircBufferReturn_t	CircBufAdd(CircularBuffer_t * buf, char c);
CircBufferReturn_t	CircBufRemove(CircularBuffer_t * buf, char * c);
CircBufferReturn_t	CircBufIsFull(CircularBuffer_t * buf);
CircBufferReturn_t	CircBufIsEmpty(CircularBuffer_t * buf);
CircBufferReturn_t	CircBufIsValid(CircularBuffer_t * buf);
CircBufferReturn_t	CircBufIsInitialized(CircularBuffer_t * buf);
CircBufferReturn_t 	CircBufDestroy(CircularBuffer_t * buf);


#endif