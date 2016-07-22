/*
 * LED.c
 *
 *  Created on: 2016年7月22日
 *      Author: Romeli
 */
#include "LED.h"

void LED_Init() {
	LED_Gpio_Init();

	GpioDataRegs.GPBDAT.bit.GPIO61 = 1;
	GpioDataRegs.GPBDAT.bit.GPIO43 = 1;
	GpioDataRegs.GPBDAT.bit.GPIO42 = 1;
}
void LED_Gpio_Init() {
	EALLOW;
	//Red
	GpioCtrlRegs.GPBPUD.bit.GPIO61 = GPIO_PULLUP;
	GpioCtrlRegs.GPBDIR.bit.GPIO61 = GPIO_OUTPUT;
	GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = GPIO_MUX_CPU1;
	//Green
	GpioCtrlRegs.GPBPUD.bit.GPIO43 = GPIO_PULLUP;
	GpioCtrlRegs.GPBDIR.bit.GPIO43 = GPIO_OUTPUT;
	GpioCtrlRegs.GPBGMUX1.bit.GPIO43 = GPIO_MUX_CPU1;
	//Blue
	GpioCtrlRegs.GPBPUD.bit.GPIO42 = GPIO_PULLUP;
	GpioCtrlRegs.GPBDIR.bit.GPIO42 = GPIO_OUTPUT;
	GpioCtrlRegs.GPBGMUX1.bit.GPIO42 = GPIO_MUX_CPU1;
	EDIS;
}
void LED_Set(Color color) {
	switch (color) {
	case Red:
		GpioDataRegs.GPBDAT.bit.GPIO61 = 0;
		GpioDataRegs.GPBDAT.bit.GPIO43 = 1;
		GpioDataRegs.GPBDAT.bit.GPIO42 = 1;
		break;
	case Green:
		GpioDataRegs.GPBDAT.bit.GPIO61 = 1;
		GpioDataRegs.GPBDAT.bit.GPIO43 = 0;
		GpioDataRegs.GPBDAT.bit.GPIO42 = 1;
		break;
	case Blue:
		GpioDataRegs.GPBDAT.bit.GPIO61 = 1;
		GpioDataRegs.GPBDAT.bit.GPIO43 = 1;
		GpioDataRegs.GPBDAT.bit.GPIO42 = 0;
		break;
	case Yellow:
		GpioDataRegs.GPBDAT.bit.GPIO61 = 0;
		GpioDataRegs.GPBDAT.bit.GPIO43 = 0;
		GpioDataRegs.GPBDAT.bit.GPIO42 = 1;
		break;
	case Purple:
		GpioDataRegs.GPBDAT.bit.GPIO61 = 0;
		GpioDataRegs.GPBDAT.bit.GPIO43 = 1;
		GpioDataRegs.GPBDAT.bit.GPIO42 = 0;
		break;
	case CyanBlue:
		GpioDataRegs.GPBDAT.bit.GPIO61 = 1;
		GpioDataRegs.GPBDAT.bit.GPIO43 = 0;
		GpioDataRegs.GPBDAT.bit.GPIO42 = 0;
		break;
	case White:
		GpioDataRegs.GPBDAT.bit.GPIO61 = 0;
		GpioDataRegs.GPBDAT.bit.GPIO43 = 0;
		GpioDataRegs.GPBDAT.bit.GPIO42 = 0;
		break;
	}
}
