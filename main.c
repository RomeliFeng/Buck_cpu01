/*
 * Buck_main.c
 *
 *  Created on: 2016��7��6��
 *      Author: Romeli
 */
#include "main.h"
Uint32 t;
float64 I_now;
float64 U_now;
float64 I_max = 300;
float64 DUTY = 0;
Color a;

Uint16 data[100];
Uint16 data_sp = 0;

void main(void) {

	/**********************************************SysCtrl Config*************************************************/
	InitSysCtrl();				//SysClk Init
	/**********************************************Perph Config*************************************************/
	Epwm2_Init();
	SPI_Init();					//Work for OLED
	Timer_Init();				//Work for mills ms's Timebase
	Xint_Init();				//Work for EC11
	ADC_Init();
	EC11_Init();
	LED_Init();
	/**********************************************PIE Config*************************************************/
	PIE_Init();					//ADC Epwm2 Xint Timer
	/**********************************************User Config*************************************************/
	OLED_Init();
	t = micros;
	Epwm2_Update_Duty(0.3);
	while (1) {
		if (micros - t >= 100) {
//			PID_Set_Outside_Max(I_max);
			U_now = ADCData.A0.FinaData;
			I_now = ADCData.B0.FinaData * 0.04577;
			t = micros;
			Epwm2_Update_Duty(DUTY);
		}
		data[data_sp++] = AdcbResultRegs.ADCRESULT0;
		if (data_sp == 100)
			data_sp = 0;
	}
}
void Epwm2_Service() {
	ADC_GetData();
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

