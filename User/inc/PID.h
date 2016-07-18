/*
 * PID.h
 *
 *  Created on: 2016��7��13��
 *      Author: Romeli
 */

#ifndef PID_H_
#define PID_H_

#include "F2837xS_device.h"        // F2837xS Headerfile Include File
#include "F2837xS_Examples.h"    // F2837xS Examples Include File

//void PID_Set(float64 P,float64 I,float64 D);
float64 PID_Compute_Inside(float64 Now_Inside);
float64 PID_Compute_Outside();
void PID_Set_Outside_Max(float64 max);

extern volatile float64 Set_Outside;
extern volatile float64 Set_Inside;

#endif /* PID_H_ */
