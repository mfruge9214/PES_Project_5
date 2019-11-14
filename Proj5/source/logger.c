/*
 * File: logger.c
 * Description: This file handles the logger functionality for the program
 * Authors: Mike Fruge and Mark Hinkle
 */

#include "logger.h"

//TODO: Delete after development
#define TEST

/* Private Variable */
static bool LOG_ENABLE;
static LoggerLevel LOG_LEVEL;

static const char * LoggerLevelStrings[NUM_LEVELS] = {
		"Test: 	 ",
		"Debug:  ",
		"Status: "
};

static const char * FunctionNameStrings[NUM_FUNCTIONS] = {
		"main: 				",
		"RedLEDOn: 			",
		"BlueLEDOn: 		",
		"GreenLEDOn: 		",
		"StateMachineA: 	",
		"StateMachineB: 	",
		"TempReading: 		",
		"AverageWait: 		",
		"TempAlert: 		",
		"Disconnect: 		",
		"CalculateAverage: 	",
		"InitSMParameters: 	",
		"I2CWriteBytes: 	",
		"I2CReadBytes: 		",
		"TMP102Init: 		",
		"TMP102POST: 		",
		"TMP102TempRead: 	",
		"CircBufInit		",
		"CircBufRealloc		",
		"CircBufAdd			",
		"CircBufRemove		"
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
		printf("Log already Enabled\n\r");
	}
	else
	{
		LOG_ENABLE = true;
		printf("Log Enabled\n\r");
	}

}


void logDisable()
{
	printf("Disabling Logger\n\r");
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
	if(!LOG_ENABLE)
	{
		return;
	}

	switch(LOG_LEVEL)
	{
	case LOGGER_LEVEL_NORMAL:
		if(lvl == LL_Normal)
		{
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("%s\n\r", message);
		}
		break;

	case LOGGER_LEVEL_DEBUG:
		if((lvl == LL_Debug) || (lvl == LL_Normal))
		{
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("%s\n\r", message);
		}
		break;

	case LOGGER_LEVEL_TEST:
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("%s\n\r", message);
		break;

	default:
		printf("LOG_LEVEL is undefined \n\r");
		break;
	}
}


void logInteger(LoggerLevel lvl, FunctionName fn, uint32_t value)
{
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
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("%u\n\r", value);
		}
		break;

	case LOGGER_LEVEL_DEBUG:
		if((lvl == LL_Debug) || (lvl == LL_Normal))
		{
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("%u\n\r", value);
		}
		break;

	case LOGGER_LEVEL_TEST:
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("%u\n\r", value);
		break;

	default:
		printf("LOG_LEVEL is undefined \n\r");
		break;
	}
}


void logTemperature(LoggerLevel lvl, FunctionName fn, float value)
{
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
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("Temperature(C): %.3f\n\r", value);
		}
		break;

	case LOGGER_LEVEL_DEBUG:
		if((lvl == LL_Debug) || (lvl == LL_Normal))
		{
			printf(LoggerLevelStrings[lvl]);
			printf(FunctionNameStrings[fn]);
			printf("Temperature(C): %.3f\n\r", value);
		}
		break;

	case LOGGER_LEVEL_TEST:
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("Temperature(C): %.3f\n\r", value);
		break;

	default:
		printf("LOG_LEVEL is undefined \n\r");
		break;
	}
}


