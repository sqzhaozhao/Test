//*********************************************************
//���ļ���Ҫ����Ϊ������ʽPID��λ��ʽPID�����㷨
//���ߣ�
//��������:2017/12/29
//�޸ļ�¼��
//**********************************************************


#include "PID.h"


//���ܣ�����ʽPID������ʼ��
//������previous,last,err


//ά���Ĵ�������
//	pid_data->kp = 0.5;
//	pid_data->ki = 0.1;
//	pid_data->kd = 0.05;

//���Ĵ�
//	pid_data->kp = 8;
//	pid_data->ki = 0.08;
//	pid_data->kd = 0.05;

//���Ĵ� ���ȵ�����ж���
//	pid_data->kp = 10;
//	pid_data->ki = 0.08;
//	pid_data->kd = 0.09;

//����ƽ�ȣ����ǵ��ڻ���
//	pid_data->kp = 5;
//	pid_data->ki = 0.08;
//	pid_data->kd = 0.09;

//	pid_data->kp = 1.0;
//	pid_data->ki = 0.1;
//	pid_data->kd = 0.5;

//mil2 ���� �����ϴ�
//	pid_data->kp = 1.0;
//	pid_data->ki = 0.3;
//	pid_data->kd = 0.5;
void PID_ParaInit(PID_PARA_VAL *pid_data)
{
	//����ֵ����ʵ�ʶ��޸�
	pid_data->kp = 4000.0;
	pid_data->ki = 0.1;
	pid_data->kd = 0.0;
	pid_data->pid_out_lim_max = 1800;//��������������12A  = 120*100mA
	pid_data->pid_out_lim_min = -1800;//������������-12A  = -120*100mA

//	pid_data->exp_value = 0.0; //����ֵ
//	pid_data->meas_value = 0.0;//���β���ֵ
	
	pid_data->err = 0.0;			//����ƫ��		
	pid_data->err_last = 0.0;	//��һ��ƫ��
	pid_data->err_pre = 0.0;	//ǰһ��(����һ��)ƫ��
	pid_data->integ = 0;
	pid_data->ki_t = 5;//������ʱ�䳣��
	pid_data->kd_t = 5;//΢����ʱ�䳣��
	
	pid_data->k = 1.0;//����ϵ��
	
	pid_data->out_value = 0.0;//PID���ֵ
	
	pid_data->integ_lim_max = 600000;
	pid_data->integ_lim_min = -600000;

}

//���ܣ�����ʽPID������ʼ��
//������pid_data--PIDϵ����ƫ�����
//			exp_value--���Ƶ�����ֵ
//			meas_value--���ƵĲ���ֵ
//���أ�cal_value--PID����Ľ��
float Cal_IncPID(PID_PARA_VAL *pid_data, float exp_value, float meas_value)
{
	float kp_vlaue,ki_vlaue,kd_vlaue;
	float cal_value;
	
	cal_value = pid_data->out_value;//��¼��һ�ν��
	
	//���㱾��ƫ��
	pid_data->err = exp_value - meas_value;
	
	//�����������
	kp_vlaue = pid_data->err - pid_data->err_last;
	ki_vlaue = pid_data->err;
	kd_vlaue = pid_data->err - 2*pid_data->err_last + pid_data->err_pre;

	//����PID���ֵ
//	cal_value += pid_data->kp * kp_vlaue
//						+ pid_data->ki * pid_data->ki_t * ki_vlaue
//						+	pid_data->kd  * kd_vlaue / pid_data->kd_t;
	cal_value += pid_data->kp * kp_vlaue
						+ pid_data->ki * ki_vlaue
						+	pid_data->kd  * kd_vlaue;
	
	//�õ���һ����ǰһ�ε�ƫ��
	pid_data->err_pre = pid_data->err_last;
	pid_data->err_last = pid_data->err;
	
	//�޷���ֹ����
	if(cal_value > pid_data->pid_out_lim_max)
		cal_value = pid_data->pid_out_lim_max;
	if(cal_value < pid_data->pid_out_lim_min)
		cal_value = pid_data->pid_out_lim_min;
	
////////////���趨ֵ������ƫ���Ϊ0�ǣ�������Ϊ0
//	if(exp_value==0 && pid_data->err==0 && pid_data->err_pre==0 && pid_data->err_last==0)
//	{
//		cal_value = 0;
//	}


////////////	
	
	pid_data->out_value = cal_value;
	
	return cal_value;
}

//����ʽPID
float Cal_AbsPID(PID_PARA_VAL *pid_data, float exp_value, float meas_value)
{
	float kp_vlaue,ki_vlaue,kd_vlaue;
	float cal_value;
		
	//���㱾��ƫ��
	kp_vlaue = pid_data->err = exp_value - meas_value;
	
	//���������v��
	pid_data->integ += (pid_data->err * pid_data->k);//������
	
	//�����޷�
	if(pid_data->integ > pid_data->integ_lim_max)
	{
		pid_data->integ = pid_data->integ_lim_max;
	}
	else if(pid_data->integ < pid_data->integ_lim_min)
	{
		pid_data->integ = pid_data->integ_lim_min;
	}
	
	kd_vlaue = pid_data->err - pid_data->err_last;

	//����PID���ֵ
	cal_value = pid_data->kp * kp_vlaue
						+ pid_data->ki * pid_data->ki_t  * pid_data->integ
						+	pid_data->kd * kd_vlaue / pid_data->kd_t;
//	cal_value = pid_data->kp * kp_vlaue
//						+ pid_data->ki * pid_data->integ
//						+	pid_data->kd * kd_vlaue;/
	//�õ���һ����ǰһ�ε�ƫ��
	pid_data->err_last = pid_data->err;;
	
	//�޷���ֹ����
	if(cal_value > pid_data->pid_out_lim_max)
		cal_value = pid_data->pid_out_lim_max;
	if(cal_value < pid_data->pid_out_lim_min)
		cal_value = pid_data->pid_out_lim_min;
	
	pid_data->out_value = cal_value;
	
	return cal_value;
}





