/* Includes */



/* Definitions */


#define REALLOCATE_BUFFER	0
#define BUFSIZE_MULT		10

/* Structures */

typedef struct CircularBuffer_t
{
	char* buffer_start;
	char* first;
	char* last;
	uint8_t length;
	uint32_t capacity;
} CircularBuffer_t;


typedef enum
{
	BUF_SUCCESS = 0,
	BUF_FULL = 1,
	BUF_EMPTY = 2,
	BUF_FAIL = 3
} CircBufferReturn_t;


/* Function Prototypes */

CircBufferReturn_t CircBufInit(CircularBuffer_t * buf, uint8_t size);
CircBufferReturn_t CircBufRealloc(CircularBuffer_t * buf);
CircBufferReturn_t	CircBufAdd(CircularBuffer_t * buf);
