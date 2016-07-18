/*
 * EPwm.c
 *
 *  Created on: 2016��7��6��
 *      Author: Romeli
 */

#include "EPwm.h"

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

	EPwm2Regs.TBPRD = 4095;                       // Set timer period
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

void Epwm2_Update(Uint16 CMP) {
	Uint16 Max_Cmp = EPwm2Regs.TBPRD - EPwm2Regs.DBRED.bit.DBRED
			- EPwm2Regs.DBFED.bit.DBFED;
	CMP = CMP > Max_Cmp ? Max_Cmp : CMP;
	EPwm2Regs.CMPA.bit.CMPA = CMP;
}

__interrupt void epwm2_isr(void) {

// Clear INT flag for this timer
	EPwm2Regs.ETCLR.bit.INT = 1;

// Acknowledge this interrupt to receive more interrupts from group 3
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
