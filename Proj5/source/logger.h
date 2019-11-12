/*
 * File: logger.h
 * Description: This file defines macros and function prototypes for logger
 * Authors: Mike Fruge and Mark Hinkle
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "fsl_debug_console.h"

/* Create Logging Structures */
/* Created in preparation for later assignments */

#define BUF_SIZE		50
#define NUM_LEVELS      3
#define NUM_FUNCTIONS 	17

/*** Logger Level Header Info ***/
typedef enum
{
	LL_Test 	= 0,
	LL_Debug 	= 1,
	LL_Normal 	= 2
} LoggerLevel;

#define LOGGER_LEVEL_TEST		LL_Test
#define LOGGER_LEVEL_DEBUG		LL_Debug
#define LOGGER_LEVEL_NORMAL		LL_Normal

/*** Function Name Header Info ***/
typedef enum
{
	FN_main					,
	FN_redLEDOn             ,
	FN_blueLEDOn			,
	FN_greenLEDOn			,
	FN_stateMachineA		,
	FN_stateMachineB		,
	FN_tempReading        	,
	FN_avgWait				,
	FN_tempAlert			,
	FN_disconnect			,
	FN_calculateAverage		,
	FN_initSMParameters		,
	FN_i2cWriteBytes		,
	FN_i2cReadBytes			,
	FN_TMP102Init			,
	FN_TMP102POST			,
	FN_TMP102TempRead		,
	FN_CircBufInit			,
	FN_CircBufRealloc
} FunctionName;

/***********************************************
 *
 * @brief	Initialize the global logger to its default state (disabled, WARNING, default buffer)
 *
 ***********************************************/
void logInit(LoggerLevel lvl);

/***********************************************
 *
 * @brief	Enable Logger for printouts
 *
 ***********************************************/
void logEnable();

/***********************************************
 * @brief	Disable logger activity
 ***********************************************/
void logDisable();

/***********************************************
 * @brief	Check status of logger
 *
 * @return true if logger is enabled, false if not
 *
 ***********************************************/
bool logStatus();

/***********************************************
 * @brief	Print out input message as well as logger info
 *
 * @param[in] lvl		Specified level of logging info
 *
 * @param[in] fn		Name of function that called logger
 *
 * @param[in] message	Information to be logged
 ***********************************************/
void logString(LoggerLevel lvl, FunctionName fn, char* message);

/***********************************************
 * @brief	Print out input value as well as logger info
 *
 * @param[in] lvl		Specified level of logging info
 *
 * @param[in] fn		Name of function that called logger
 *
 * @param[in] value		Value to be logged
 ***********************************************/
void logInteger(LoggerLevel lvl, FunctionName fn, uint32_t value);

/***********************************************
 * @brief	Print out formatted temp value as well as logger info
 *
 * @param[in] lvl		Specified level of logging info
 *
 * @param[in] fn		Name of function that called logger
 *
 * @param[in] value		Value to be logged
 ***********************************************/
void logTemperature(LoggerLevel lvl, FunctionName fn, float value);

