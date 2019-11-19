/*
 * filename: main.h
 * created by: Mark Hinkle and Mike Fruge
 * date created: 11/18/19
 */

#ifndef _MAIN_H
#define _MAIN_H

#include "uart.h"

/*~~~~~~~~~~~  UART_MODE ~~~~~~~~~~~*/
#define NONBLOCKING_MODE		0
#define BLOCKING_MODE			1
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Set to 0 for Interrupt Based UART
 * Set to 1 for Blocking/Polling Based UART  */
#define UART_MODE 				BLOCKING_MODE


/*~~~~~~~~~~ PROGRAM_MODE ~~~~~~~~~~*/
#define ECHO_MODE				0
#define APP_MODE				1
#define TEST_MODE				2
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Set to 0 or ECHO_MODE to run in Echo mode
 * Set to 1 or APP_MODE  to run in Application mode
 * Set to 2 or TEST_MODE to run in Test mode */
#define PROGRAM_MODE			ECHO_MODE



typedef uart_ret_t (* uart_fcnPtr)(void);

#if UART_MODE
#define INT_ENABLE				0
uart_fcnPtr uartEcho 			= uartBlockEcho;
uart_fcnPtr uartApp		 		= uartBlockApp;
#else
#define INT_ENABLE				1
uart_fcnPtr uartEcho 			= uartNonBlockEcho;
uart_fcnPtr uartApp		 		= uartNonBlockApp;
#endif /* UART_MODE */

#if 	(PROGRAM_MODE == 0)
#define loglevel	LOGGER_LEVEL_DEBUG
#elif	(PROGRAM_MODE == 1)
uart_ret_t (* uartRun)(void) 	= uartApp;
#define loglevel	LOGGER_LEVEL_DEBUG
#elif	(PROGRAM_MODE == 2)
#define loglevel	LOGGER_LEVEL_TEST
#else
#error "Undefined Program Mode"
#endif	/* PROGRAM_MODE */




#endif /* _MAIN_H */
