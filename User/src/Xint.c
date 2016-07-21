/*
 * Xint.c
 *
 *  Created on: 2016��7��13��
 *      Author: Romeli
 *      Work For EC11
 */
#include "Xint.h"

void Xint_Init(void)
{
	Xint_Gpio_Init();
	InitXintConfig();
}

void Xint_Gpio_Init(void) {
	// GPIO20 and GPIO21 are inputs
	EALLOW;
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;          // input
	GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 2;        // Sync
	GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = 0x1F;    // Each sampling window is 32*SYSCLKOUT

	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;         // GPIO
	GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;          // input
	GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 2;        // XINT2 Qual using 6 samples
	GpioCtrlRegs.GPACTRL.bit.QUALPRD2 = 0x1F;    // Each sampling window is 32*SYSCLKOUT

	// GPIO20 is XINT1, GPIO21 is XINT2
	GPIO_SetupXINT1Gpio(20);
	GPIO_SetupXINT2Gpio(21);
}

void InitXintConfig(void)
{
	// Configure XINT1
	XintRegs.XINT1CR.bit.POLARITY = 3; 			// Falling edge and a rising edge interrupt
	XintRegs.XINT2CR.bit.POLARITY = 0;      	// Falling edge interrupt

	// Enable XINT1 and XINT2
	XintRegs.XINT1CR.bit.ENABLE = 1;        	// Enable XINT1
	XintRegs.XINT2CR.bit.ENABLE = 1;        	// Enable XINT2
}
