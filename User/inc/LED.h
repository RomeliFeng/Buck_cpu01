/*
 * LED.h
 *
 *  Created on: 2016年7月22日
 *      Author: Romeli
 */

#ifndef LED_H_
#define LED_H_

#include "F2837xS_device.h"     // Headerfile Include File
#include "F2837xS_Examples.h"   // Examples Include File

typedef enum _Color{
	Red = 0, Green, Blue, Yellow, Purple, CyanBlue, White
} Color;

void LED_Init();
void LED_Gpio_Init();
void LED_Set(Color color);

#endif /* USER_INC_LED_H_ */
