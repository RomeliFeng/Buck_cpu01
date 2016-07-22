/*
 * ADC.h
 *
 *  Created on: 2016��7��13��
 *      Author: Romeli
 */

#ifndef ADC_H_
#define ADC_H_

#include "F2837xS_device.h"     // Headerfile Include File
#include "F2837xS_Examples.h"   // Examples Include File

#define ADCSumNum 8

void ADC_Init(void);
void SetupADCInterrupt(void);
void ConfigureADC(void);
void ADC_GetData(void);
int comp(const void*a,const void*b);

typedef struct {
	Uint8 Index;
	uint_fast16_t Data[100];
	uint_fast16_t FinaData;
} _ADCChanel;

typedef struct {
	_ADCChanel A0;
	_ADCChanel A4;
	_ADCChanel B0;
} _ADCData;

extern _ADCData ADCData;

#endif /* ADC_H_ */
