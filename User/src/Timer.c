/*
 * Timer.c
 *
 *  Created on: 2016��7��12��
 *      Author: Romeli
 */

#include "Timer.h"
#include "SWPrioritizedIsrLevels.h"

volatile uint_fast32_t millis = 0;

void Timer_Init() {
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 200000, 1);	//Init CpuTimer1 in 200M Cycle:1mS
	CpuTimer0Regs.TCR.bit.TIE = 1;			//Enable CpuTimer1 Interrupt
	CpuTimer0Regs.TCR.bit.TSS = 0;			//Enable running
}

__interrupt void cpu_timer0_isr(void) {
//	// Set interrupt priority:
//	volatile Uint16 TempPIEIER = PieCtrlRegs.PIEIER1.all;
//	IER |= M_INT1;
//	IER &= MINT1;	  	               // Set "global" priority
//	PieCtrlRegs.PIEIER1.all &= MG1_7;   // Set "group"  priority
//	PieCtrlRegs.PIEACK.all = 0xFFFF;   // Enable PIE interrupts
//	__asm("  NOP");
//	EINT;

	// Insert ISR Code here.......
	millis++;

//	// Restore registers saved:
//	DINT;
//	PieCtrlRegs.PIEIER1.all = TempPIEIER;

	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
