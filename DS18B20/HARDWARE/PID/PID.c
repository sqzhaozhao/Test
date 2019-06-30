//*********************************************************
//该文件主要功能为：增量式PID与位置式PID控制算法
//作者：
//创建日期:2017/12/29
//修改记录：
//**********************************************************


#include "PID.h"


//功能：增量式PID参数初始化
//参数：previous,last,err


//维嘉履带较流畅
//	pid_data->kp = 0.5;
//	pid_data->ki = 0.1;
//	pid_data->kd = 0.05;

//加履带
//	pid_data->kp = 8;
//	pid_data->ki = 0.08;
//	pid_data->kd = 0.05;

//加履带 较稳但电机有抖动
//	pid_data->kp = 10;
//	pid_data->ki = 0.08;
//	pid_data->kd = 0.09;

//空载平稳，但是调节缓慢
//	pid_data->kp = 5;
//	pid_data->ki = 0.08;
//	pid_data->kd = 0.09;

//	pid_data->kp = 1.0;
//	pid_data->ki = 0.1;
//	pid_data->kd = 0.5;

//mil2 较稳 力量较大
//	pid_data->kp = 1.0;
//	pid_data->ki = 0.3;
//	pid_data->kd = 0.5;
void PID_ParaInit(PID_PARA_VAL *pid_data)
{
	//具体值根据实际而修改
	pid_data->kp = 4000.0;
	pid_data->ki = 0.1;
	pid_data->kd = 0.0;
	pid_data->pid_out_lim_max = 1800;//最大正向输出电流12A  = 120*100mA
	pid_data->pid_out_lim_min = -1800;//最大负向输出电流-12A  = -120*100mA

//	pid_data->exp_value = 0.0; //期望值
//	pid_data->meas_value = 0.0;//本次测量值
	
	pid_data->err = 0.0;			//本次偏差		
	pid_data->err_last = 0.0;	//上一次偏差
	pid_data->err_pre = 0.0;	//前一次(上上一次)偏差
	pid_data->integ = 0;
	pid_data->ki_t = 5;//积分项时间常数
	pid_data->kd_t = 5;//微分项时间常数
	
	pid_data->k = 1.0;//拐弯系数
	
	pid_data->out_value = 0.0;//PID输出值
	
	pid_data->integ_lim_max = 600000;
	pid_data->integ_lim_min = -600000;

}

//功能：增量式PID参数初始化
//参数：pid_data--PID系数及偏差参数
//			exp_value--控制的期望值
//			meas_value--控制的测量值
//返回：cal_value--PID计算的结果
float Cal_IncPID(PID_PARA_VAL *pid_data, float exp_value, float meas_value)
{
	float kp_vlaue,ki_vlaue,kd_vlaue;
	float cal_value;
	
	cal_value = pid_data->out_value;//记录上一次结果
	
	//计算本次偏差
	pid_data->err = exp_value - meas_value;
	
	//计算各项数据
	kp_vlaue = pid_data->err - pid_data->err_last;
	ki_vlaue = pid_data->err;
	kd_vlaue = pid_data->err - 2*pid_data->err_last + pid_data->err_pre;

	//计算PID输出值
//	cal_value += pid_data->kp * kp_vlaue
//						+ pid_data->ki * pid_data->ki_t * ki_vlaue
//						+	pid_data->kd  * kd_vlaue / pid_data->kd_t;
	cal_value += pid_data->kp * kp_vlaue
						+ pid_data->ki * ki_vlaue
						+	pid_data->kd  * kd_vlaue;
	
	//得到上一次与前一次的偏差
	pid_data->err_pre = pid_data->err_last;
	pid_data->err_last = pid_data->err;
	
	//限幅防止饱和
	if(cal_value > pid_data->pid_out_lim_max)
		cal_value = pid_data->pid_out_lim_max;
	if(cal_value < pid_data->pid_out_lim_min)
		cal_value = pid_data->pid_out_lim_min;
	
////////////当设定值与所有偏差均为0是，结果输出为0
//	if(exp_value==0 && pid_data->err==0 && pid_data->err_pre==0 && pid_data->err_last==0)
//	{
//		cal_value = 0;
//	}


////////////	
	
	pid_data->out_value = cal_value;
	
	return cal_value;
}

//绝对式PID
float Cal_AbsPID(PID_PARA_VAL *pid_data, float exp_value, float meas_value)
{
	float kp_vlaue,ki_vlaue,kd_vlaue;
	float cal_value;
		
	//计算本次偏差
	kp_vlaue = pid_data->err = exp_value - meas_value;
	
	//计算各项数v据
	pid_data->integ += (pid_data->err * pid_data->k);//积分项
	
	//积分限幅
	if(pid_data->integ > pid_data->integ_lim_max)
	{
		pid_data->integ = pid_data->integ_lim_max;
	}
	else if(pid_data->integ < pid_data->integ_lim_min)
	{
		pid_data->integ = pid_data->integ_lim_min;
	}
	
	kd_vlaue = pid_data->err - pid_data->err_last;

	//计算PID输出值
	cal_value = pid_data->kp * kp_vlaue
						+ pid_data->ki * pid_data->ki_t  * pid_data->integ
						+	pid_data->kd * kd_vlaue / pid_data->kd_t;
//	cal_value = pid_data->kp * kp_vlaue
//						+ pid_data->ki * pid_data->integ
//						+	pid_data->kd * kd_vlaue;/
	//得到上一次与前一次的偏差
	pid_data->err_last = pid_data->err;;
	
	//限幅防止饱和
	if(cal_value > pid_data->pid_out_lim_max)
		cal_value = pid_data->pid_out_lim_max;
	if(cal_value < pid_data->pid_out_lim_min)
		cal_value = pid_data->pid_out_lim_min;
	
	pid_data->out_value = cal_value;
	
	return cal_value;
}





