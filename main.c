/*
 * Buck_main.c
 *
 *  Created on: 2016��7��6��
 *      Author: Romeli
 */
#include "main.h"
Uint32 t;
float64 I_now;

void main(void) {

	/**********************************************SysCtrl Config*************************************************/
	InitSysCtrl();				//SysClk Init
	/**********************************************Perph Config*************************************************/
	Epwm2_Init();
	SPI_Init();					//Work for OLED
	Timer_Init();				//Work for mills ms's Timebase
	Xint_Init();				//Work for EC11
	ADC_Init();
	/**********************************************PIE Config*************************************************/
	PIE_Init();					//ADC Epwm2 Xint Timer
	/**********************************************User Config*************************************************/
	OLED_Init();
	OLED_Print_str(0, 0, "12  454", C6x8);
	OLED_Print_num(0, 1, 123, C6x8);
	OLED_Print_lf(0, 2, 123.31, 2, C6x8);
	OLED_Print_lf(0, 3, 45.154, 3, C6x8);
	t = micros;
	while (1) {
		if (micros - t >= 100) {
			I_now = ADCData.A0.FinaData / 65535.0 * 3000.0;
			t = micros;
			Epwm2_Update(PID_Compute_Inside(I_now));
		}
	}
}

void EC11_Key_Click() {
	OLED_Fill(0x00);
	OLED_Print_str(0, 0, "Key_Click", C6x8);
}

void EC11_Clockwise() {
	OLED_Fill(0x00);
	OLED_Print_str(0, 1, "Clockwise", C6x8);
}
void EC11_Key_Press() {
	OLED_Fill(0x00);
	OLED_Print_str(0, 3, "Key_Press", C6x8);
}
void EC11_Anticlockwise() {
	OLED_Fill(0x00);
	OLED_Print_str(0, 4, "Anticlockwise", C6x8);
}
