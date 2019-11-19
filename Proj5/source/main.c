/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Proj5.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "core_cm0plus.h"

/* TODO: insert other include files here. */
#include "main.h"
#include "circular_buffer.h"
#include "logger.h"
#include "Systick.h"

/* UCUNIT test files */
#include "Testsuite.h"

uint32_t int_flag;

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* Initialize development modules */
    SystickInit();
    NVIC_EnableIRQ(SysTick_IRQn);
    logInit(loglevel);
    logEnable();

//    int i = 0;
//    char bufferOut;

    /* Run tests and exit */
	if(PROGRAM_MODE == TEST_MODE)
	{
		Testsuite_RunTests();
		return 1;
	}

	while(1)
	{
		if(PROGRAM_MODE == ECHO_MODE)
		{
			uartEcho();
		}
		else if(PROGRAM_MODE == APP_MODE)
		{
			uartApp();
		}
	}

//	else
//	{
//		/* Program goes in here */
//		CircularBuffer_t * circBuf = CircBufCreate();
//		CircBufferReturn_t	bufReturn;
//		char string[20] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't'};
//
//		bufReturn = CircBufInit(circBuf, BUFSIZE);
//
//		bufReturn = CircBufAdd(circBuf, '1');
//
//		bufReturn = CircBufAdd(circBuf, '2');
//
//		bufReturn = CircBufRemove(circBuf, &bufferOut);
//
//		while(bufReturn == BUF_SUCCESS)
//		{
//			bufReturn = CircBufAdd(circBuf, string[i]);
//			i++;
//		}
//
//		bufReturn = BUF_SUCCESS;
//
//		while(bufReturn == BUF_SUCCESS)
//		{
//			bufReturn = CircBufRemove(circBuf, &bufferOut);
//		}
//		bufReturn = CircBufDestroy(circBuf);

    return 0 ;
}
