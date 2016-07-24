/*
 * Buzzer.c
 *
 *  Created on: 2016年7月24日
 *      Author: Romeli
 */
#include "Buzzer.h"
#include "Timer.h"

#define Buzzer_DB 5
bool Buzzer_Flag = false;
Uint32 Buzzer_T;

void Buzzer_Init() {
	EALLOW;
	GpioCtrlRegs.GPCPUD.bit.GPIO66 = GPIO_PULLUP;
	GpioCtrlRegs.GPCDIR.bit.GPIO66 = GPIO_OUTPUT;
	GpioCtrlRegs.GPCGMUX1.bit.GPIO66 = GPIO_MUX_CPU1;
	EDIS;
	GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
}

void Buzzer_beep() {
	Buzzer_T = millis;
	Buzzer_Flag = true;
	GpioDataRegs.GPCDAT.bit.GPIO66 = 1;
}

void Buzzer_isr() {
	if (Buzzer_Flag) {
		if (millis - Buzzer_T > Buzzer_T) {
			Buzzer_Flag = false;
			GpioDataRegs.GPCDAT.bit.GPIO66 = 0;
		}
	}
}
