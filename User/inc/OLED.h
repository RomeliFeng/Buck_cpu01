/*
 * OLED.h
 *
 *  Created on: 2016��5��22��
 *      Author: Romeli
 *  Depend on: "Delay.h" & "SPI.h"
 */

#ifndef OLED_H_
#define OLED_H_

#include "F2837xS_device.h"        // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"    // F2837xS Examples Include File

typedef enum {
	Cmd_Mode = 0, Data_Mode = 1
} WriteMode;

typedef enum {
	C6x8 = 6, C8x16 = 8, C16x16 = 16
} CharMode;

void OLED_Gpio_Init();
void OLED_Init();
void OLED_Write(unsigned char data, WriteMode mode);
void OLED_Set_pos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char bmp);
void OLED_Print_c(unsigned char x, unsigned char y, char data, CharMode mode);
void OLED_Print_str(unsigned char x, unsigned char y, char *str,
		unsigned char digit, CharMode mode);
void OLED_Print_num(unsigned char x, unsigned char y, long num,
		unsigned char digit, CharMode mode);
void OLED_Print_lf(unsigned char x, unsigned char y, double lf,
		unsigned char digit, unsigned char ndigit, CharMode mode);

#endif /* OLED_H_ */
