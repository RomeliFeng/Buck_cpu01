/*
 * EC11.cpp
 *
 *  Created on: 2016��7��10��
 *      Author: Romeli
 *      PinMap:		|P17------Phase|
 *      			|P20------xint1|
 *      			|P21------xint2|
 */
#include "EC11.h"

volatile Uint8 EC11_Flag = 0;

__interrupt void xint1_isr(void) {
	Uint8 Flag;
	Flag = GpioDataRegs.GPADAT.bit.GPIO17 ^ GpioDataRegs.GPADAT.bit.GPIO20;
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

