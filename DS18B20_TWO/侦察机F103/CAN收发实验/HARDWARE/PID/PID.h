#ifndef _PID_H
#define _PID_H
#include "math.h"

//PID相关数值
typedef struct
{
	float kp;				//比例系数
	float ki;				//积分系数
	float kd;				//微分系数
	
	float ki_t;				//积分时间
	float kd_t;				//微分时间
	
//	float exp_value;//期望值
//	float	meas_value;//本次测量值
	
	float err;			//本次偏差		
	float err_last;	//上一次偏差
	float err_pre;	//前一次(上上一次)偏差
	
	float integ;//积分项
	float integ_lim_max;//积分限幅最大值
	float integ_lim_min;//积分限幅最小值

	
	float pid_out_lim_max;	//PID输出限幅上限
	float pid_out_lim_min;	//PID输出限幅下限
	float k;
	
	float out_value;//PID输出值
	
}PID_PARA_VAL;

void PID_ParaInit(PID_PARA_VAL *pid_data);
float Cal_IncPID(PID_PARA_VAL *pid_data, float exp_value, float meas_value);
float Cal_AbsPID(PID_PARA_VAL *pid_data, float exp_value, float meas_value);


#endif

