/*
 * filename: uart.h
 * created by: Mark Hinkle
 * date created: 11/16/2019
 */
#ifndef _UART_H
#define _UART_H

#include <stdint.h>
#include <stdbool.h>

#define UART_OVERSAMPLE_RATE 	(16)
#define UART_BUS_CLOCK_FREQ		(24000000)
#define UART_BAUD_RATE			(115200)

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
	app_fail
} uart_ret_t;

/* non blocking only */
typedef struct
{
	bool	uartST_txReady;
	bool	uartST_rxReady;
	bool	uartST_rxDisable;
}uart_nb_state_t;


/*** function definitions ***/
void uartInit(bool blocking);
void uartEnableInterrupts(bool enable);
uart_ret_t uartSendByte(uint8_t b);
uart_ret_t uartReadByte(uint8_t * b);

uart_ret_t uartBlockTransmitReady(void);
uart_ret_t uartBlockReceiveReady(void);
uart_ret_t uartBlockSendCharacter(char c);
uart_ret_t uartBlockReadCharacter(char * c);
uart_ret_t uartBlockEcho(void);
uart_ret_t uartBlockApp(void);

uart_ret_t uartNonBlockEcho(void);
uart_ret_t uartNonBlockApp(void);
void UART0_IRQHandler();

#endif /* _UART_H */
