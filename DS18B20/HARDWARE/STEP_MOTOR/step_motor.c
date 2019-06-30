//---------------------------------------------------
//�ó����ļ�Ϊ��ֱ̨������ջ����ƴ���
//ͨ���Ƕȴ����������̨λ�ã���������Ϊ������ͨ��PID����PWM����������Ƶ�ת���
//��� �м�λ�� �ұ�λ��(-90~+90��)��Ӧ�Ĵ�������ѹ�ֱ�Ϊ0.6V 1.6v 2.9v
//���������ƶ˿�Ϊ��PD0,  0--��ת��1--��ת
//����߶�Ӧ�ܽţ���ɫ--VCC				��ɫ--GND
//								��ɫ--�������	��ɫ--PWM   
//							
//---------------------------------------------------

#include  "step_motor.h"
#include  "delay.h"
#include  "PID.h"
#include  "adc.h"
#include  "timer.h"

//����̨ת���м�λ��ʱ����Ӧ�������ĵ�ѹֵ
float mid_sensor_data = 1.6;


PID_PARA_VAL pid_data;

//���������ƹܽŶ�������������ʼ��(PID����)
void step_motor_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PD0�˿�ʱ��

	//����������� DIR- PUL-�ֱ��ӦPD0 PD1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);					
	DIR_CTRL(0);//Ĭ����ʱ��ת��
	
	//PWM��ʼ��
	tim3_pwm_init(1800-1,2-1);//20KHZ,ռ�ձ��趨ֵ����100�����ת��
	
	PID_ParaInit(&pid_data);//PID������ʼ��
}

//�ջ�������̨λ��
//���Ʒ�Χ:-90��-+90��
//exp_position:���Ƶ������Ƕȣ�-90��-+90��
//ע��ң�����仯90�ȣ���������ѹֵ�仯0.9V
void holder_ctrl(int exp_position)
{
		float exp_dat = 0.0;
		Get_ADC_Value();
	
		//������ĽǶ�ֵת��Ϊʵ�ʵĵ�ѹֵ
		exp_dat = mid_sensor_data + ((float)exp_position)*0.01;
		
//		Cal_AbsPID(&pid_data, exp_dat, holder_dat);
		Cal_AbsPID(&pid_data, exp_dat, holder_dat);

		if(pid_data.out_value>0)
		{
			DIR_CTRL(1);
			set_pwm(PWM_CH3, 100+ pid_data.out_value);//PB0
		}
		else if(pid_data.out_value<0)
		{
			DIR_CTRL(0);
			set_pwm(PWM_CH3, 100-pid_data.out_value);
		}
		else
		{
			DIR_CTRL(0);
			set_pwm(PWM_CH3, 0);//PB0
		}
}





















