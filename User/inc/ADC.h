/*
 * ADC.h
 *
 *  Created on: 2016Äê7ÔÂ13ÈÕ
 *      Author: Romeli
 */

#ifndef ADC_H_
#define ADC_H_

#include "F2837xS_device.h"     // Headerfile Include File
#include "F2837xS_Examples.h"   // Examples Include File

#define ADCSumNum 8

int comp(const void*a,const void*b);
void ADC_Init(void);
void SetupADCInterrupt(void);
void ConfigureADC(void);
void ADC_GetData(void);

typedef struct {
	Uint8 Index;
	uint_fast16_t Data[100];
	uint_fast16_t FinaData;
} _ADCChanel;

typedef struct {
	_ADCChanel A0;
	_ADCChanel B0;
} _ADCData;

extern _ADCData ADCData;
extern Uint32 aaa;

#endif /* ADC_H_ */
