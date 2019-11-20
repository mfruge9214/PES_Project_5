/*
 * File: flash.c
 * Description: This file defines macros and function prototypes for LEDS
 * Authors: Mike Fruge and Mark Hinkle
 */

#include <stdint.h>
#include <stdio.h>


#include "board.h"

#define	RED_PORT	GPIOB
#define	RED_PIN		18U
#define GREEN_PORT	GPIOB
#define GREEN_PIN	19U
#define BLUE_PORT	GPIOD
#define BLUE_PIN	1U

#define LED_ON		0U
#define LED_OFF		1U

/***********************************************
 *
 * @brief	Initialize LEDs to off and route their pins
 *
 ***********************************************/
void gpioInit();

/***********************************************
 *
 * @brief	Set LED to RED and disable other colors
 *
 ***********************************************/
void gpioRedLEDOn();

/***********************************************
 *
 * @brief	Set LED to Green and disable other colors
 *
 ***********************************************/
void gpioGreenLEDOn();

/***********************************************
 *
 * @brief	Set LED to Blue and disable other colors
 *
 ***********************************************/
void gpioBlueLEDOn();

