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
Uint16 i=1;

void main(void) {

	/**********************************************SysCtrl Config*************************************************/
	InitSysCtrl();				//SysClk Init
	/**********************************************Perph Config*************************************************/
	Epwm2_Init();
	ADC_Init();
	SPI_Init();					//Work for OLED
	Timer_Init();				//Work for mills ms's Timebase
	Xint_Init();				//Work for EC11
	EC11_Init();
	LED_Init();
	Buzzer_Init();
	/**********************************************PIE Config*************************************************/
	PIE_Init();					//ADC Epwm2 Xint Timer
	/**********************************************User Config*************************************************/
	OLED_Init();
	t = millis;
	Epwm2_Update_Duty(0.3);
	while (1) {
//		OLED_Print_str(0,0,"213",5,C6x8);
//		OLED_Print_str(0,1,"231",0,C6x8);
//		OLED_Print_str(0,2,"213w",5,C6x8);
//		OLED_Print_str(0,3,"2231",0,C6x8);
		Buzzer_beep();
		Delay(1000);
	}
}
void Epwm2_Service() {
	ADC_GetData();
}
void EC11_Key_Click() {

}

void EC11_Clockwise() {

}
void EC11_Key_Press() {

}
void EC11_Anticlockwise() {

}

