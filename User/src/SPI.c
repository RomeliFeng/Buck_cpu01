/*
 * SPI.c
 *
 *  Created on: 2016Äê7ÔÂ9ÈÕ
 *      Author: Romeli
 */
#include "SPI.h"

union {
	unsigned int whole;
	struct {
		unsigned char low :8;
		unsigned char high :8;
	};
} DataType;

void SPI_Init() {
	InitSpiaGpio();
	InitSPIConfig();
}

void InitSPIConfig() {
	EALLOW;
	ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0;
	EDIS;
	SpiaRegs.SPICCR.bit.SPISWRESET = 0;		// SPI on reset,must be set to 1

	SpiaRegs.SPICTL.all = 0x0006;    		// Enable master mode, normal phase,
											// enable talk, and SPI int disabled.
//	SpiaRegs.SPIBRR.all = 0x007F;
	SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = 0x00;	// SPICLK set to /SPI_BIT_RATE+1

	SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;// Data is output on the rising edge of the SPICLK signal
	SpiaRegs.SPICCR.bit.HS_MODE = 0;		// disable High Mode
	SpiaRegs.SPICCR.bit.SPILBK = 0;			// disable SPI lookback
	SpiaRegs.SPICCR.bit.SPICHAR = 7;		// transmit 8-bit data
	SpiaRegs.SPICCR.bit.SPISWRESET = 1;		// disable SPI on reset

	SpiaRegs.SPIPRI.bit.FREE = 1;   // Set so breakpoints don't disturb xmission

	// Initialize SPI FIFO registers
	SpiaRegs.SPIFFTX.all = 0xE040;
	SpiaRegs.SPIFFRX.all = 0x2044;
	SpiaRegs.SPIFFCT.all = 0x0;

	DELAY_US(1);
}

void SPI_transfer(unsigned char data) {
	DataType.high = data;
	SpiaRegs.SPITXBUF = DataType.whole;
	;
	while (SpiaRegs.SPIFFTX.bit.TXFFST != 0x00)
		;
}
