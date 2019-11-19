/*
 * File: Systick.h
 * Authors: Mike Fruge and Mark Hinkle
 * Description: Implementation of Systick specific to Project 5
 */
#include "core_cm0plus.h"
#include "MKL25Z4.h"
#include <stdint.h>

#define SYS_CLK_FREQ	48000000	/* System clock frequency in Hz */
#define SYS_FREQ_DIVIDER	10			/* Divides the SYSCLK to generate the timestamp */

#define START_CRITICAL()		__disable_irq();
#define END_CRITICAL()			__enable_irq();


typedef enum
{
	SysTick_Success,
	SysTick_Fail
}SysTick_Ret_t;

void SystickInit(void);
void GetSysTickTime(char stringOut[11]);
