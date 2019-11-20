/*
 * filename: uart.h
 * created by: Mark Hinkle
 * date created: 11/16/2019
 */
#ifndef _UART_H
#define _UART_H

#include <stdint.h>
#include <stdbool.h>
#include "circular_buffer.h"

#define UART_OVERSAMPLE_RATE 	(16)
#define UART_BUS_CLOCK_FREQ		(24000000)
#define UART_BAUD_RATE			(115200)

#define TRANSMIT_CONDITION		(0x0A)

#define ASCII_BASE				0x21
#define ASCII_END				0x7A
#define ASCII_CHAR_CNT			(ASCII_END - ASCII_BASE)


#define uartNonBlockTransmitEnable		(UART0->C2 |= UART_C2_TIE_MASK)
#define uartNonBlockTransmitDisable		(UART0->C2 &= ~UART_C2_TIE_MASK)

typedef enum
{
	tx_ready,		//uartBlockTransmitReady
	tx_busy,		//uartBlockTransmitReady
	tx_success,		//uartSendByte //uartBlockSendCharacter
	tx_fail,		//uartSendByte //uartBlockSendCharacter
	rx_ready,		//uartReceiveReady
	rx_busy,		//uartReceiveReady
	rx_success,		//uartReadByte //uartBlockReadCharacter
	rx_fail,		//uartReadByte //uartBlockReadCharacter
	echo_success,	//uartBlockEcho
	echo_fail,		//uartBlockEcho
	app_success,
	app_fail,
	report_success, //uartSendReport
	report_fail		//uartSendReport
} uart_ret_t;

/* non blocking only */
typedef struct
{
	bool	uartST_txReady;
	bool 	uartST_txEnabled;
	bool	uartST_rxReady;
	bool	uartST_rxEnabled;
	uint8_t txByte;
	uint8_t rxByte;
}uart_nb_state_t;


/*** function definitions ***/
void uartInit(bool blocking);
void uartEnableInterrupts(bool enable);
uart_ret_t uartSendByte(uint8_t b);
uart_ret_t uartReadByte(uint8_t * b);
uart_ret_t uartSendReport(void);

uart_ret_t uartBlockTransmitReady(void);
uart_ret_t uartBlockReceiveReady(void);
uart_ret_t uartBlockSendCharacter(char c);
uart_ret_t uartBlockReadCharacter(char * c);
uart_ret_t uartBlockEcho(void);
uart_ret_t uartBlockApp(CircularBuffer_t * buf);

uart_ret_t uartNonBlockEcho(void);
uart_ret_t uartNonBlockApp(CircularBuffer_t * buf);
uart_ret_t uartSendReport();
uart_ret_t uartPrintf(char * string);
void UART0_IRQHandler();

#endif /* _UART_H */
