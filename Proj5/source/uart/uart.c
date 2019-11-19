/*
 * filename: uart.h
 * created by: Mark Hinkle
 * date created: 11/16/2019
 */
#include "uart.h"
#include "MKL25Z4.h"

// global var for interrupt handling
static volatile uart_nb_state_t uartST;

/*
 * brief: uartInit - Initializes hardware, baud, and data frame for UART0
 * param: N/A
 * ret: N/A
 */
void uartInit(bool int_en)
{
	/* init clocks */
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	/* disable tx & rx */
	UART0->C2 &= ~UART_C2_RE_MASK; //disable receive
	UART0->C2 &= ~UART_C2_TE_MASK; //disable transmit

	/* init ports and pins */
	//rx = A1 handled at init
	//tx = A2

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

	/* select whether or not to enable interrupts */
	uartEnableInterrupts(int_en);

	/* enable tx & rx */
	UART0->C2 |= UART_C2_RE_MASK; //enable receive
	UART0->C2 |= UART_C2_TE_MASK; //enable transmit
}

/*
 * brief: uartEnableInterrupts - Enables interrupts for non blocking uart
 * param: enable - true of false
 * ret: N/A
 */
void uartEnableInterrupts(bool enable)
{
	if(enable)
	{
		uartST.uartST_txReady 	= false;
		uartST.uartST_rxReady 	= false;
		uartST.uartST_rxDisable = false;

		UART0->C2 |= UART_C2_TIE_MASK; 		//interrupt when data can be transmitted
		UART0->C2 |= UART_C2_RIE_MASK; 		//interrupt when data has been received

		//TODO add interrupts for errors

		UART0->C2 |= UART_C2_RIE_MASK; 		//rx interrupt enable
		UART0->C2 |= UART_C2_TIE_MASK;		//tx interrupt enable
		NVIC_EnableIRQ(UART1_IRQn);
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
	while(uartBlockTransmitReady() != tx_ready); //wait
	return uartSendByte((uint8_t) c);		//send byte
}

/*
 * brief: uartBlockReadCharacter - Blocks until char received then reads value
 * param: c - character to be updated with read value
 * ret: success or fail
 */
uart_ret_t uartBlockReadCharacter(char * c)
{
	while(uartBlockReceiveReady() != rx_ready); //wait
	return uartReadByte((uint8_t *) c);		//read byte
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
		return echo_fail;

	ret = uartBlockSendCharacter(echo_byte);
	if(ret != tx_success)
		return echo_fail;

	return echo_success;
}

/*
 * brief: uartBlockApp - Adds received bytes to data structure and transmits report back in case of special char
 * param: N/A
 * ret: success or fail
 */
uart_ret_t uartBlockApp()
{
	return app_success;
}

/* * * * * NON-BLOCKING UART FUNCTIONS * * * * */

/*
 * brief: uartNonBlockEcho - Retransmits any characters that have been received
 * param: N/A
 * ret: success or fail
 */
uart_ret_t uartNonBlockEcho(void)
{
	return echo_success;
}

/*
 * brief: uartNonBlockApp - Adds received bytes to data structure and transmits report back in case of special char
 * param: N/A
 * ret: success or fail
 */
uart_ret_t uartNonBlockApp(void)
{
	return app_success;
}

/*
 * brief: UART0_IRQHandler - Sets flags that are read by NonBlock functions for processing
 * param: N/A
 * ret: N/A
 */
void UART0_IRQHandler()
{
	uint8_t flags = UART0->S1;

	if(flags & UART_S1_RDRF_MASK)
		uartST.uartST_rxReady = true;
	if(flags & UART_S1_TDRE_MASK)
		uartST.uartST_txReady = true;
}
