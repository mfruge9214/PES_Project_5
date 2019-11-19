/*
 * File: Systick.c
 * Authors: Mike Fruge and Mark Hinkle
 * Description: Implementation of Systick specific to Project 5
 */

#include "Systick.h"
#include <stdio.h>
#include "Logger.h"

/* File Global Variables */

static uint8_t hours = 0;
static uint8_t minutes = 0;
static uint8_t seconds = 0;
static uint8_t tenths = 0;
static uint8_t SystickFlag;

/*
 * @brief Initialize the SysTick timer for the system
 *
 * @returns SysTick_Ret_t	Status of the initialization
 */
void SystickInit(void)
{
	/* Gathered from Systick_Config */
	SysTick->LOAD =	(SYS_CLK_FREQ/SYS_FREQ_DIVIDER);	// 48000000/10 = Interrupt every 480000 ticks = 1 ms
	NVIC_SetPriority(SysTick_IRQn, 3);

	SystickFlag = 0;
	SysTick->VAL = 0UL;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |	SysTick_CTRL_TICKINT_Msk   |	SysTick_CTRL_ENABLE_Msk;

}

void SysTick_Handler(void)
{
	START_CRITICAL();
	SystickFlag = 1;
	END_CRITICAL();
}

void SysTick_Increment(void)
{
	if(SystickFlag)
	{
		START_CRITICAL();
		SystickFlag = 0;

		tenths++;
		if(tenths == 10)
		{
			seconds++;
			tenths = 0;
		}

		if(seconds == 60)
		{
			minutes++;
			seconds = 0;
		}

		if(minutes == 60)
		{
			hours++;
			minutes = 0;
		}
		END_CRITICAL();
	}
}

void GetSysTickTime(char stringOut[TIMESTAMP_LEN])
{
	sprintf(stringOut, "%02d:%02d:%02d.%01d\0",
			hours,
			minutes,
			seconds,
			tenths);
}
