/*
 * ADC.c
 *
 *  Created on: 2016��7��13��
 *      Author: Romeli
 */

#include "ADC.h"
#include "SWPrioritizedIsrLevels.h"
#include "stdlib.h"
#include "Timer.h"

_ADCData ADCData;


void ADC_Init(void) {
	ConfigureADC();
	SetupADCMode();
//	AdcaRegs.ADCSOCFRC1.bit.SOC0 = 1;
//	AdcbRegs.ADCSOCFRC1.bit.SOC0 = 1;
	EALLOW;
	EPwm2Regs.ETSEL.bit.SOCAEN = 1;	        // Enable SOC on A group
	EPwm2Regs.TBCTL.bit.CTRMODE=TB_COUNT_UP;
	EDIS;
}

void SetupADCMode(void) {
	Uint16 acqps;

	//determine minimum acquisition window (in SYSCLKS) based on resolution
	if (ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION) {
//		acqps = 14; //75ns
		acqps = 100; //500ns
	} else { //resolution is 16-bit
		acqps = 63; //320ns
//		acqps = 150; //750ns
//		acqps = 200; //1us
	}

//Select the channels to convert and end of conversion flag
	//ADCA
	EALLOW;
	AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert pin A0
	AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
	AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 7;	//ePWM2 ADCSOCA
	AdcaRegs.ADCSOC1CTL.bit.CHSEL = 4;	//SOC1 will convert pin A4
	AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
	AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 7;	//ePWM2 ADCSOCA

	AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
	AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
	//ADCB
	AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert pin B0
	AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
	AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 7;	//ePWM2 ADCSOCA

	AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT2 flag
	AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

	EDIS;
}

//Write ADC configurations and power up the ADC for both ADC A and ADC B
void ConfigureADC(void) {
	EALLOW;

	//write configurations
	AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
	AdcSetMode(ADC_ADCA, ADC_RESOLUTION_16BIT, ADC_SIGNALMODE_SINGLE);
	AdcSetMode(ADC_ADCB, ADC_RESOLUTION_16BIT, ADC_SIGNALMODE_SINGLE);

	//Set pulse positions to late
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//power up the ADCs
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	//delay for 1ms to allow ADC time to power up
	DELAY_US(1000);				//if epwn init before adc delay,epwn interrupt will not be active

	EDIS;
}
void ADC_GetData(void) {
	ADCData.A0.FinaData = AdcaResultRegs.ADCRESULT0;		//Get data from A0
	ADCData.A4.FinaData = AdcaResultRegs.ADCRESULT1;		//Get data from A4
	ADCData.B0.FinaData = AdcbResultRegs.ADCRESULT0;		//Get data from B0

//	//Start newADCSOCFRC1.bit.SOC0 = 1; convert
//	AdcaRegs.
//	AdcbRegs.ADCSOCFRC1.bit.SOC0 = 1;
}

//__interrupt void adca1_isr(void) {
//	// Set interrupt priority:
//	volatile Uint16 TempPIEIER = PieCtrlRegs.PIEIER1.all;
//	IER |= M_INT1;
//	IER &= MINT1;	  	               // Set "global" priority
//	PieCtrlRegs.PIEIER1.all &= MG1_1;   // Set "group"  priority
//	PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts
//	__asm("  NOP");
//	EINT;
//
//	// Insert ISR Code here.......
//	ADCData.A0.Data[ADCData.A0.Index] = AdcaResultRegs.ADCRESULT0;
//	ADCData.A0.Index++;
//	if (ADCData.A0.Index >= ADCSumNum) {
//		qsort(ADCData.A0.Data, ADCData.A0.Index, sizeof(Uint16), comp);
//		ADCData.A0.FinaData = ADCData.A0.Data[ADCData.A0.Index / 2];
//		ADCData.A0.Index = 0;
//		bbb = micros;
//	}
//	AdcaRegs.ADCSOCFRC1.bit.SOC0 = 1;
//	AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
//
//	// Restore registers saved:
//	DINT;
//	PieCtrlRegs.PIEIER1.all = TempPIEIER;
//
//	// Acknowledge this interrupt to receive more interrupts from group 1
//	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//}
//
//__interrupt void adcb1_isr(void) {
//	// Set interrupt priority:
//	volatile Uint16 TempPIEIER = PieCtrlRegs.PIEIER1.all;
//	IER |= M_INT1;
//	IER &= MINT1;	  	               // Set "global" priority
//	PieCtrlRegs.PIEIER1.all &= MG1_2;   // Set "group"  priority
//	PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts
//	__asm("  NOP");
//	EINT;
//
//	// Insert ISR Code here.......
//	ADCData.B0.Data[ADCData.B0.Index] = AdcbResultRegs.ADCRESULT0;
//	ADCData.B0.Index++;
//	if (ADCData.B0.Index >= ADCSumNum) {
//		qsort(ADCData.B0.Data, ADCData.B0.Index, sizeof(Uint16), comp);
//		ADCData.B0.FinaData = ADCData.B0.Data[ADCData.B0.Index / 2];
//		ADCData.B0.Index = 0;
//	}
//	AdcbRegs.ADCSOCFRC1.bit.SOC0 = 1;
//	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
//
//	// Restore registers saved:
//	DINT;
//	PieCtrlRegs.PIEIER1.all = TempPIEIER;
//
//	// Acknowledge this interrupt to receive more interrupts from group 1
//	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//}
//int comp(const void*a, const void*b) {
//	return ((int*) a)[0] - ((int*) b)[0];
//}
