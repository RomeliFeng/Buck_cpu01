/*
 * Buck_EPwm.h
 *
 *  Created on: 2016��7��6��
 *      Author: Romeli
 */

#ifndef EPWM_H_
#define EPWM_H_

#include "F2837xS_device.h"     // Headerfile Include File
#include "F2837xS_Examples.h"   // Examples Include File

void Epwm2_Init(void);
void InitEPwm2Config(void);
void Epwm2_Update(float64 Duty);

#endif /* INC_BUCK_EPWM_H_ */
