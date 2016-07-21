/*
 * EPwm.c
 *
 *  Created on: 2016��7��6��
 *      Author: Romeli
 *    PinMap:		|P2------Epwn2A|
 *      			|P3------Epwn2B|
 */

#include "EPwm.h"

#define Def_TBPRD 3999
#define Def_DBRED 64
#define Def_DBFED 64

volatile Uint16 TBPRD_Flag;
enum {
	nor_Grad = 3999, low_Grad = 9999, grd_Grad = 19999
} TBPRD_Grad;

void Epwm2_Init(void) {
	InitEPwm2Gpio();
	InitEPwm2Config();
}

void InitEPwm2Config(void) {
	//��������EPWM2
	CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;

	//ʧ��TBʱ��
	EALLOW;
	ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;

	EPwm2Regs.TBPRD = 4000;                       // Set timer period
	EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
	EPwm2Regs.TBCTR = 0x0000;                     // Clear counter

	// Setup TBCLK
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;         // Slow just to observe on the
												  // scope
	// Setup shadow register load on ZERO
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

	// Setup compare
	EPwm2Regs.CMPA.bit.CMPA = 0;
//	EPwm2Regs.CMPB.bit.CMPB = 512;
	// Set actions
	EPwm2Regs.AQCTLA.bit.PRD = AQ_SET;            // Set PWM2A on Zero
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;

	EPwm2Regs.AQCTLB.bit.PRD = AQ_CLEAR;          // Set PWM2A on Zero
	EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;

	// Active Low complementary PWMs - setup the deadband
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
	EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
	EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
	EPwm2Regs.DBRED.bit.DBRED = 64;
	EPwm2Regs.DBFED.bit.DBFED = 64;

	// Interrupt where we will modify the deadband
	EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
	EPwm2Regs.ETSEL.bit.INTEN = 1;                // Enable INT
	EPwm2Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

	//ʹ��TBʱ��
	EALLOW;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;
}

void Epwm2_Update(float64 Duty) {
	static Uint16 Max_Cmp = Def_TBPRD - Def_DBRED - Def_DBFED;	//calc cmpA
	Uint16 Cmp;
	if (Duty <= 0.20 && TBPRD_Flag != low_Grad) {	//nor_Grad: 4000; low_Grad:10000; grd_Grad:20000
		TBPRD_Flag = 1;
		EPwm2Regs.TBPRD = low_Grad;
		Max_Cmp = EPwm2Regs.TBPRD - EPwm2Regs.DBRED.bit.DBRED//recalc max cmpA
				- EPwm2Regs.DBFED.bit.DBFED;
	} else if (Duty <= 0.10 && TBPRD_Flag != grd_Grad) {
		TBPRD_Flag = 2;
		EPwm2Regs.TBPRD = grd_Grad;
		Max_Cmp = EPwm2Regs.TBPRD - EPwm2Regs.DBRED.bit.DBRED//recalc max cmpA
				- EPwm2Regs.DBFED.bit.DBFED;
	} else if (Duty > 0.20 && TBPRD_Flag != nor_Grad) {
		TBPRD_Flag = 0;
		EPwm2Regs.TBPRD = nor_Grad;
		Max_Cmp = EPwm2Regs.TBPRD - EPwm2Regs.DBRED.bit.DBRED//recalc max cmpA
				- EPwm2Regs.DBFED.bit.DBFED;
	}
	Cmp = Duty * EPwm2Regs.TBPRD;				//calc cmp from duty
	Cmp = Cmp > Max_Cmp ? Max_Cmp : Cmp;		//limit cmp
	EPwm2Regs.CMPA.bit.CMPA = Cmp;				//push cmp to res
}

__interrupt void epwm2_isr(void) {

// Clear INT flag for this timer
	EPwm2Regs.ETCLR.bit.INT = 1;

// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
