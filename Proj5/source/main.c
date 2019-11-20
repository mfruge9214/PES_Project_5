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
    uartInit(INT_ENABLE);
    logInit(loglevel);
    logEnable();

#if   (PROGRAM_MODE == TEST_MODE)
	Testsuite_RunTests();				//run testsuite and exit
	return 0;
#else
#if   (PROGRAM_MODE == ECHO_MODE)
    uart_fncPtr_t uartRun = uartEcho;	//set uartRun to uartEcho(block or nonblock)
#elif (PROGRAM_MODE == APP_MODE)
    uart_fncPtr_t uartRun = uartApp;	//set uartRun to uartApp(block or nonblock)

#endif /* PROGRAM_MODE == ECHO_MODE or APP_MODE */
#endif /* PROGRAM_MODE == TEST_MODE */

	/* Run uartFunction indefinitely - unless error detected */
    uart_ret_t err;
	while(1)
	{
		err = uartRun();
//		if((err != echo_success) || (err != app_success))
		if(err != echo_success)
		{
			//log error
			return -1;
		}
	}
    return 0;
}
