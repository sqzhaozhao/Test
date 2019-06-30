#ifndef _PID_H
#define _PID_H
#include "math.h"

//PID�����ֵ
typedef struct
{
	float kp;				//����ϵ��
	float ki;				//����ϵ��
	float kd;				//΢��ϵ��
	
	float ki_t;				//����ʱ��
	float kd_t;				//΢��ʱ��
	
//	float exp_value;//����ֵ
//	float	meas_value;//���β���ֵ
	
	float err;			//����ƫ��		
	float err_last;	//��һ��ƫ��
	float err_pre;	//ǰһ��(����һ��)ƫ��
	
	float integ;//������
	float integ_lim_max;//�����޷����ֵ
	float integ_lim_min;//�����޷���Сֵ

	
	float pid_out_lim_max;	//PID����޷�����
	float pid_out_lim_min;	//PID����޷�����
	float k;
	
	float out_value;//PID���ֵ
	
}PID_PARA_VAL;

void PID_ParaInit(PID_PARA_VAL *pid_data);
float Cal_IncPID(PID_PARA_VAL *pid_data, float exp_value, float meas_value);
float Cal_AbsPID(PID_PARA_VAL *pid_data, float exp_value, float meas_value);


#endif

