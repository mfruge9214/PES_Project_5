	/*
 * File: logger.c
 * Description: This file handles the logger functionality for the program
 * Authors: Mike Fruge and Mark Hinkle
 */

#include "logger.h"
#include "Systick.h"
#include "uart.h"

/* Private Variable */
static bool LOG_ENABLE;
static LoggerLevel LOG_LEVEL;

static const char * LoggerLevelStrings[NUM_LEVELS] = {
		"	Test: 	\0",
		"	Debug:  \0",
		"	Status: \0"
};

static const char * FunctionNameStrings[NUM_FUNCTIONS] = {
		"main: 				\0",
		"RedLEDOn: 			\0",
		"BlueLEDOn: 		\0",
		"GreenLEDOn: 		\0",
		"uartBlockApp: 		\0",
		"StateMachineB: 	\0",
		"TempReading: 		\0",
		"AverageWait: 		\0",
		"TempAlert: 		\0",
		"Disconnect: 		\0",
		"CalculateAverage: 	\0",
		"InitSMParameters: 	\0",
		"I2CWriteBytes: 	\0",
		"I2CReadBytes: 		\0",
		"TMP102Init: 		\0",
		"TMP102POST: 		\0",
		"TMP102TempRead: 	\0",
		"CircBufInit		\0",
		"CircBufRealloc		\0",
		"CircBufAdd			\0",
		"CircBufRemove		\0"
};

void logInit(LoggerLevel lvl)
{
	LOG_ENABLE = false;
	LOG_LEVEL = lvl;
}


void logEnable()
{
	if(LOG_ENABLE)
	{
		uartPrintf("Log already Enabled\n\r\0");
	}
	else
	{
		LOG_ENABLE = true;
		uartPrintf("Log Enabled\n\r\0");
	}

}


void logDisable()
{
	printf("Disabling Logger\n\r\0");
	if(LOG_ENABLE)
	{
		LOG_ENABLE = false;
	}
}


bool logStatus()
{
	return LOG_ENABLE;
}

void logString(LoggerLevel lvl, FunctionName fn, char* message)
{
	char timeStamp[TIMESTAMP_LEN];
	GetSysTickTime(timeStamp);
	if(!LOG_ENABLE)
	{
		return;
	}

	switch(LOG_LEVEL)
	{
	case LOGGER_LEVEL_NORMAL:
		if(lvl == LL_Normal)
		{
			uartPrintf(timeStamp);
			uartPrintf(LoggerLevelStrings[lvl]);
			uartPrintf(FunctionNameStrings[fn]);
			uartPrintf(" \0");
			uartPrintf(message);
			uartPrintf("\n\r\0");
		}
		break;

	case LOGGER_LEVEL_DEBUG:
		if((lvl == LL_Debug) || (lvl == LL_Normal))
		{
			uartPrintf(timeStamp);
			uartPrintf(LoggerLevelStrings[lvl]);
			uartPrintf(FunctionNameStrings[fn]);
			uartPrintf(" \0");
			uartPrintf(message);
			uartPrintf("\n\r\0");
		}
		break;

	case LOGGER_LEVEL_TEST:
		uartPrintf(timeStamp);
		uartPrintf(LoggerLevelStrings[lvl]);
		uartPrintf(FunctionNameStrings[fn]);
		uartPrintf(" \0");
		uartPrintf(message);
		uartPrintf("\n\r\0");
		break;

	default:
		uartPrintf(timeStamp);
		uartPrintf("LOG_LEVEL is undefined \n\r\0");
		break;
	}
}


void logInteger(LoggerLevel lvl, FunctionName fn, uint32_t value)
{
	char timeStamp[TIMESTAMP_LEN];
	GetSysTickTime(timeStamp);
	/* Ensure Logger Enabled */
	if(!LOG_ENABLE)
	{
		return;
	}

	switch(LOG_LEVEL)
	{
	case LOGGER_LEVEL_NORMAL:
		if(lvl == LL_Normal)
		{
			printf(timeStamp);
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("%u\n\r", value);
		}
		break;

	case LOGGER_LEVEL_DEBUG:
		if((lvl == LL_Debug) || (lvl == LL_Normal))
		{
			printf(timeStamp);
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("%u\n\r", value);
		}
		break;

	case LOGGER_LEVEL_TEST:
		printf(timeStamp);
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("%u\n\r", value);
		break;

	default:
		printf(timeStamp);
		printf("LOG_LEVEL is undefined \n\r");
		break;
	}
}


void logTemperature(LoggerLevel lvl, FunctionName fn, float value)
{
	char timeStamp[TIMESTAMP_LEN];
	GetSysTickTime(timeStamp);
	/* Ensure Logger Enabled */
	if(!LOG_ENABLE)
	{
		return;
	}

	switch(LOG_LEVEL)
	{
	case LOGGER_LEVEL_NORMAL:
		if(lvl == LL_Normal)
		{
			printf(timeStamp);
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("Temperature(C): %.3f\n\r", value);
		}
		break;

	case LOGGER_LEVEL_DEBUG:
		if((lvl == LL_Debug) || (lvl == LL_Normal))
		{
			printf(timeStamp);
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("Temperature(C): %.3f\n\r", value);
		}
		break;

	case LOGGER_LEVEL_TEST:
		printf(timeStamp);
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("Temperature(C): %.3f\n\r", value);
		break;

	default:
		printf(timeStamp);
		printf("LOG_LEVEL is undefined \n\r");
		break;
	}
}


