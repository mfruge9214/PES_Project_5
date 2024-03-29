/*
 * filename: uart.h
 * created by: Mark Hinkle
 * date created: 11/16/2019
 */
#include "uart.h"
#include "MKL25Z4.h"
#include "circular_buffer.h"
#include "gpio.h"
#include "logger.h"


// receive and transmit buffers
static CircularBuffer_t * rxBuf;
static CircularBuffer_t * txBuf;

// Static Array for storing characters read
static uint8_t CharCountArray[ASCII_CHAR_CNT] = {0};

/*
 * brief: uartInit - Initializes hardware, baud, and data frame for UART0
 * param: N/A
 * ret: N/A
 */
void uartInit(bool int_en)
{
	/* init tx and rx buffers */
	rxBuf = CircBufCreate();
	CircBufferReturn_t ret 	= CircBufInit(rxBuf, BUFSIZE);
	if(ret != BUF_SUCCESS)
	{
		gpioRedLEDOn();
		logString(LL_Debug, FN_uartInit, "Creation of rx Buffer Failed\n\r\0");
		return;
	}

	txBuf = CircBufCreate();
	ret 	= CircBufInit(txBuf, BUFSIZE);
	if(ret != BUF_SUCCESS)
	{
		gpioRedLEDOn();
		logString(LL_Debug, FN_uartInit, "Creation of tx Buffer Failed\n\r\0");
		return;
	}


	/* init clocks */
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//TODO Remove after screen grab
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	SIM->SOPT5 &= ~(SIM_SOPT5_UART0RXSRC_MASK | SIM_SOPT5_UART0TXSRC_MASK);
//	SIM->SOPT2 |= SIM_SOPT2_UART0SRC_MASK;
//	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;


	/* disable tx & rx */
	UART0->C2 &= ~UART_C2_RE_MASK; //disable receive
	UART0->C2 &= ~UART_C2_TE_MASK; //disable transmit

	/* set data frame */
//	UART0->C1 |= UART_C1_PT_MASK; 		//odd parity
//	UART0->C1 |= UART_C1_PE_MASK; 		//enable parity
	UART0->C1 &= ~UART_C1_M_MASK; 		//8 bit data
	UART0->BDH &= ~UART_BDH_SBNS_MASK;	//1 stop bit

	/* set baud */
	uint16_t sbr = (uint16_t)((UART_BUS_CLOCK_FREQ)/(UART_BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);

	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);


	/* select whether or not to enable interrupts */
	uartEnableInterrupts(int_en);

	/* enable tx & rx */
	UART0->C2 |= UART_C2_RE_MASK; //enable receive
	UART0->C2 |= UART_C2_TE_MASK; //enable transmit
}

/*
 * brief: uartEnableInterrupts - Enables interrupts for non blocking uart
 * param: enable - true or false
 * ret: N/A
 */
void uartEnableInterrupts(bool enable)
{
	if(enable)
	{
		/* Dont Enable Transmit Interrupts Yet */
//		UART0->C2 |= UART_C2_TIE_MASK; 		//interrupt when data can be transmitted
		UART0->C2 |= UART_C2_RIE_MASK; 		//interrupt when data has been received
		UART0->C3 = uartNonBlockErrorFlags;

		NVIC_ClearPendingIRQ(UART0_IRQn);
		NVIC_EnableIRQ(UART0_IRQn);
	}
}

/*
 * brief: uartSendByte - Transmits a single byte over uart
 * param: b - byte to be sent
 * ret: N/A
 */
uart_ret_t uartSendByte(uint8_t b)
{
	UART0->D = b;
	return tx_success;
}

/*
 * brief: uartReadByte - Reads a single byte received over uart
 * param: b - byte to hold the read value
 * ret: success or fai;
 */
uart_ret_t uartReadByte(uint8_t * b)
{
	*b = UART0->D;
	return rx_success;
}


/* * * * * BLOCKING UART FUNCTIONS * * * * */

/*
 * brief: uartBlockTransmitReady - Signals whether a new character is able to be sent
 * param: N/A
 * ret: ready or busy
 */
uart_ret_t uartBlockTransmitReady()
{
	if(UART0->S1 & UART_S1_TDRE_MASK)
		return tx_ready;
	else
		return tx_busy;
}

/*
 * brief: uartBlockReceiveReady - Signals whether a new character has been received
 * param: N/A
 * ret: ready or busy
 */
uart_ret_t uartBlockReceiveReady()
{
	if(UART0->S1 & UART_S1_RDRF_MASK)
		return rx_ready;
	else
		return rx_busy;
}

/*
 * brief: uartBlockSendCharacter - Blocks until ready then sends a character
 * param: c - character to be sent
 * ret: success or fail
 */
uart_ret_t uartBlockSendCharacter(char c)
{
//	gpioGreenLEDOn();
	while(uartBlockTransmitReady() != tx_ready); //wait
	return uartSendByte((uint8_t) c);			 //send byte
}

/*
 * brief: uartBlockReadCharacter - Blocks until char received then reads value
 * param: c - character to be updated with read value
 * ret: success or fail
 */
uart_ret_t uartBlockReadCharacter(char * c)
{
//	gpioBlueLEDOn();
	while(uartBlockReceiveReady() != rx_ready); //wait
	return uartReadByte((uint8_t *) c);			//read byte
}

/*
 * brief: uartBlockEcho - Waits until a character is received and transmits it back
 * param: N/A
 * ret: success or fail
 */
uart_ret_t uartBlockEcho()
{
	char echo_byte;
	uart_ret_t ret;

	ret = uartBlockReadCharacter(&echo_byte);
	if(ret != rx_success)
	{
		gpioRedLEDOn();
		return echo_fail;
	}


	ret = uartBlockSendCharacter(echo_byte);
	if(ret != tx_success)
	{
		gpioRedLEDOn();
		return echo_fail;
	}

	return echo_success;
}

/*
 * brief: uartBlockApp - Adds received bytes to data structure and transmits report back in case of special char
 * param: N/A
 * ret: success or fail
 */
uart_ret_t uartBlockApp()
{
	char data;
	uart_ret_t ret;

	/* Read new character */
	ret = uartBlockReadCharacter(&data);
	if(ret != rx_success)
	{
		gpioRedLEDOn();
		return ret;
	}

	CircBufAdd(rxBuf, data);

	/* Check character value */
	if(data == TRANSMIT_CONDITION)
	{
		ret = uartBlockSendReport();
		if(ret != report_success)
		{
			gpioRedLEDOn();
			return ret;
		}
		logString(LL_Debug, FN_uartBlockApp, "Character Count Sent \0");
	}

	return app_success;
}

uart_ret_t uartBlockSendReport()
{
	uint8_t b;
	while(CircBufIsEmpty(rxBuf) != BUF_EMPTY)
	{
		CircBufRemove(rxBuf, &b);
		if((b >= ASCII_BASE) && (b <= ASCII_END))
		{
			CharCountArray[b - ASCII_BASE]++;
		}
	}

	for(uint8_t i = 0; i < ASCII_CHAR_CNT; i++)
	{
		// Skip characters that have not been received
		if(CharCountArray[i] != 0)
		{
			uartBlockSendCharacter(ASCII_BASE + i);
			uartBlockSendCharacter('-');
			if(CharCountArray[i] >= 10)
			{
				uartBlockSendCharacter(DEC_TO_ASCII(CharCountArray[i] / 10));
				uartBlockSendCharacter(DEC_TO_ASCII(CharCountArray[i] % 10));
			}
			else
			{
				uartBlockSendCharacter(DEC_TO_ASCII(CharCountArray[i]));
			}
			uartBlockSendCharacter(';');
			uartBlockSendCharacter(' ');
		}

	}
	uartBlockSendCharacter('\n');
	uartBlockSendCharacter('\r');
	return report_success;
}

/* * * * * NON-BLOCKING UART FUNCTIONS * * * * */

uart_ret_t uartNonBlockSendCharacter(char c)
{
	CircBufferReturn_t ret = CircBufAdd(txBuf, c);
	if(ret == BUF_FULL)
	{
		gpioRedLEDOn();
		return tx_fail;
	}

	else
		return tx_success;
}

uart_ret_t uartNonBlockReadCharacter(char * c)
{
	CircBufferReturn_t ret = CircBufRemove(rxBuf, c);
	if(ret == BUF_EMPTY)
		return rx_fail;
	else
		return rx_success;
}

/*
 * brief: uartNonBlockEcho - Retransmits any characters that have been received
 * param: N/A
 * ret: success or fail
 */
uart_ret_t uartNonBlockEcho(void)
{
	char data;

	if(CircBufIsEmpty(rxBuf) != BUF_EMPTY)
	{
		CircBufRemove(rxBuf, &data);	//transfer data from rxBuf
		CircBufAdd(txBuf, data);		//to txBuf
		uartNonBlockTransmitEnable;		//and then enable transmission
	}
	if(CircBufIsEmpty(txBuf) == BUF_EMPTY)
	{
		uartNonBlockTransmitDisable;	//stop transmitting if no more tx data available
	}

	return echo_success;
}

/*
 * brief: uartNonBlockApp - Transmits report over uart in case of special char
 * param: N/A
 * ret: success or fail
 */
uart_ret_t uartNonBlockApp()
{
	static uart_ret_t ret;
	char data;
	ret = uartNonBlockReadCharacter(&data);
	if(ret != rx_success)
		return app_success;

	if(data == TRANSMIT_CONDITION)
	{
		ret = uartNonBlockSendReport();
		if(ret != report_success)
		{
			gpioRedLEDOn();
			return ret;
		}

	}
	else
	{
		CircBufAdd(rxBuf, data);
	}
	return app_success;
}

uart_ret_t uartNonBlockSendReport()
{
//	START_CRITICAL();

	char b;
	while(CircBufIsEmpty(rxBuf) != BUF_EMPTY)
	{
		CircBufRemove(rxBuf, &b);
		CharCountArray[b - ASCII_BASE]++;
	}

	uartNonBlockTransmitEnable;					//send

	for(uint8_t i = 0; i < ASCII_CHAR_CNT; i++)
	{
		// Skip characters that have not been received
		if(CharCountArray[i] != 0)
		{
			//TODO change this to CircBufAdd(TxBuf, xxx)
			uartNonBlockSendCharacter(ASCII_BASE + i);
			uartNonBlockSendCharacter('-');
			if(CharCountArray[i] >= 10)
			{
				uartNonBlockSendCharacter(DEC_TO_ASCII(CharCountArray[i] / 10));
				uartNonBlockSendCharacter(DEC_TO_ASCII(CharCountArray[i] % 10));
			}
			else
			{
				uartNonBlockSendCharacter(DEC_TO_ASCII(CharCountArray[i]));
			}
			uartNonBlockSendCharacter(';');
			uartNonBlockSendCharacter(' ');

			while(CircBufIsEmpty(txBuf) != BUF_EMPTY);	//wait
		}
	}
	uartNonBlockSendCharacter('\n');
	uartNonBlockSendCharacter('\r');

	uartNonBlockTransmitDisable;

	return report_success;
}


void uartPrintf(char * string)
{
	int i = 0;

	while(string[i] != 0)
	{
		uartBlockSendCharacter(string[i]);
		i++;
	}
//	uartBlockSendCharacter('\n');
//	uartBlockSendCharacter('\r');
}

/*
 * brief: UART0_IRQHandler - Sets flags that are read by NonBlock functions for processing
 * param: N/A
 * ret: N/A
 */
void UART0_IRQHandler()
{
	START_CRITICAL();

	char data;

	if(UART0->S1 & UART_S1_RDRF_MASK)
	{
		data = UART0->D;
		CircBufAdd(rxBuf, data);
	}
	if((UART0->S1 & UART_S1_TDRE_MASK) && (UART0->C2 & UART_C2_TIE_MASK))
	{
		CircBufRemove(txBuf, &data);
		uartSendByte(data);
	}
	if(UART0->S1 & uartNonBlockErrorFlags)
	{
		UART0->S1 &= ~uartNonBlockErrorFlags;
		gpioRedLEDOn();

		//Log error
	}

	END_CRITICAL();
}
