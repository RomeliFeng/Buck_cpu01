/*
 * PIE.c
 *
 *  Created on: 2016��7��11��
 *      Author: Romeli
 */
#include "PIE.h"

extern __interrupt void epwm2_isr(void);
extern __interrupt void xint1_isr(void);
extern __interrupt void xint2_isr(void);
extern __interrupt void cpu_timer0_isr(void);
//extern __interrupt void adca1_isr(void);
//extern __interrupt void adcb1_isr(void);

void PIE_Init() {
	// Disable CPU interrupts
	DINT;
	// This function is found in the F2837xS_PieCtrl.c file.
	InitPieCtrl();
	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;
	// This function is found in F2837xS_PieVect.c.
	InitPieVectTable();

	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;
	PieVectTable.EPWM2_INT = &epwm2_isr;
	PieVectTable.XINT1_INT = &xint1_isr;
	PieVectTable.XINT2_INT = &xint2_isr;
	PieVectTable.TIMER0_INT = &cpu_timer0_isr;
//	PieVectTable.ADCA1_INT = &adca1_isr;
//	PieVectTable.ADCB1_INT = &adcb1_isr;
	EDIS;

	// Enable CPU INT3 which is connected to EPWM1-3 INT:
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;			// Enable the PIE block
	PieCtrlRegs.PIEIER3.bit.INTx2 = 1;	   // Enable PIE Group 3 INT2; For EPwm2
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;    // Enable PIE Group 1 INT4; For Xint1
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;     // Enable PIE Group 1 INT5; For Xint2
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // Enable Timer0
//	PieCtrlRegs.PIEIER1.bit.INTx1 = 1;			// Enable ADCA1
//	PieCtrlRegs.PIEIER1.bit.INTx2 = 1;			// Enable ADCB1
	IER |= M_INT3;								// Enable PIE Group 3; For EPwm2
	IER |= M_INT1;                 // Enable CPU INT1 & CpuTimer0 & ADCA1 &ADCB1
	// Enable global Interrupts and higher priority real-time debug events:

	EINT;
	// Enable Global interrupt INTM
	ERTM;
	// Enable Global realtime interrupt DBGM
}
