/*
 * EC11.cpp
 *
 *  Created on: 2016��7��10��
 *      Author: Romeli
 *      PinMap:		|P59--------Dir|*
 *      			|P20------xint1|*
 *      			|P21------xint2|*
 */
#include "EC11.h"

volatile Uint8 EC11_Flag = 0;

void EC11_Init() {
	EALLOW;
	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;		 // GPIO
	GpioCtrlRegs.GPBDIR.bit.GPIO59 = 0;          // input
	GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 2;        // Sync
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD3 = 0x1F; // Each sampling window is 32*SYSCLKOUT
	EDIS;
}
__interrupt void xint1_isr(void) {
	Uint8 Flag;
	Flag = GpioDataRegs.GPBDAT.bit.GPIO59 ^ GpioDataRegs.GPADAT.bit.GPIO20;
	if (Flag == EC11_Flag) {
		if (EC11_Flag == 1)
			EC11_Clockwise();
		else
			EC11_Anticlockwise();
	}
	EC11_Flag = Flag;
	// Acknowledge this interrupt to get more from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

__interrupt void xint2_isr(void) {
	DELAY_US(50000);
	if (GpioDataRegs.GPADAT.bit.GPIO21 == 0) {
		DELAY_US(300000);
		if (GpioDataRegs.GPADAT.bit.GPIO21 == 1) {
			EC11_Key_Click();
		} else {
			EC11_Key_Press();
		}
	}
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//void __attribute__((weak)) EC11_Key_Click() {
//	return;
//}

//void EC11_Key_Press() {
//	return;
//}

//void EC11_Clockwise() {
//	return;
//}

//void EC11_Anticlockwise() {
//	return;
//}

