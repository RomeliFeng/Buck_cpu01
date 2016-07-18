/*
 * SPI.h
 *
 *  Created on: 2016Äê7ÔÂ9ÈÕ
 *      Author: Romeli
 */

#ifndef SPI_H_
#define SPI_H_

#include "F2837xS_device.h"        // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"    // F2837xS Examples Include File

void SPI_Init();
void InitSPIConfig();
void SPI_transfer(unsigned char data);

#endif /* USER_INC_SPI_H_ */
