/*
 * PID.c
 *
 *  Created on: 2016��7��13��
 *      Author: Romeli
 */

#include "PID.h"

#define PID_Type PID_Diff				//PID Type switch
#define PID_Type2 PID_Post				//PID Type switch
#define PID_Post 0
#define PID_Diff 1

volatile float64 Output_Inside_Max = 1;	// Inside limit
volatile float64 Output_Inside_Min = 0;
volatile float64 Output_Outside_Max = 3000;	// Outside limit
volatile float64 Output_Outside_Min = 0;

volatile float64 Kp = 0.8;
volatile float64 Ki = 0.002;
volatile float64 Kd = 0;
volatile float64 pError = 0;
volatile float64 pError_Last = 0;
volatile float64 iTerm = 0;
volatile float64 dError = 0;
volatile float64 Set_Inside = 0;
volatile float64 Output_Inside = 0;

volatile float64 Kp2 = 80;
volatile float64 Ki2 = 20;
volatile float64 Kd2 = 0;
volatile float64 pError2 = 0;
volatile float64 pError_Last2 = 0;
volatile float64 iTerm2 = 0;
volatile float64 dError2 = 0;
volatile float64 Set_Outside = 0;
volatile float64 Output_Outside = 0;

float64 PID_Compute_Inside(float64 Now_Inside) {
#if  PID_Type==PID_Diff
	float64 Calc;
#endif
	pError_Last = pError;									//Get last pError
	pError = Set_Inside - Now_Inside;							//Compute pError
	iTerm += Ki * pError;										//Compute iTerm
	iTerm = iTerm > Output_Inside_Max ? Output_Inside_Max :		//Limit of iTerm
			iTerm < Output_Inside_Min ? Output_Inside_Min : iTerm;
	dError = pError_Last - pError;								//Compute dError
#if  PID_Type==PID_Diff
	Calc = Kp * pError + iTerm + Kd * dError;
	Output_Inside += Calc;
#elif PID_Type==PID_Post
	Output_Inside = Kp * pError + iTerm + Kd * dError;
#endif
	Output_Inside =
			Output_Inside > Output_Inside_Max ? Output_Inside_Max :	//Limit of Output_Inside
			Output_Inside < Output_Inside_Min ?
					Output_Inside_Min : Output_Inside;
	return Output_Inside;
}

float64 PID_Compute_Outside(float64 Now_Inside, float64 Now_Outside) {
#if  PID_Type2==PID_Diff
	float64 Calc;
#endif
	pError_Last2 = pError2;									//Get last pError
	pError2 = Set_Outside - Now_Outside;						//Compute pError
	iTerm2 += Ki2 * pError2;									//Compute iTerm
	iTerm2 = iTerm2 > Output_Outside_Max ? Output_Outside_Max :	//Limit of iTerm
				iTerm2 < Output_Outside_Min ? Output_Outside_Min : iTerm2;
	dError2 = pError_Last2 - pError2;							//Compute dError
#if  PID_Type2==PID_Diff
			Calc = Kp2 * pError2 + iTerm2 + Kd2 * dError2;
			Output_Outside += Calc;
#elif PID_Type2==PID_Post
	Output_Outside = Kp2 * pError2 + iTerm2 + Kd2 * dError2;
#endif
	Output_Outside =
			Output_Outside > Output_Outside_Max ? Output_Outside_Max ://Limit of Output_Inside
			Output_Outside < Output_Outside_Min ?
					Output_Outside_Min : Output_Outside;

	Set_Inside = Output_Outside;				//Oupt_Outside for Set_Inside
	return PID_Compute_Inside(Now_Inside);
}
void PID_Set_Outside_Max(float64 max) {
	Output_Outside_Max = max;
}
