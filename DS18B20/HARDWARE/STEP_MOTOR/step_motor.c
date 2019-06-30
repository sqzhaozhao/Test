//---------------------------------------------------
//该程序文件为云台直流电机闭环控制代码
//通过角度传感器测得云台位置，并将此作为反馈，通过PID控制PWM输出进而控制电蛔
//左边 中间位置 右边位置(-90~+90度)对应的传感器电压分别为0.6V 1.6v 2.9v
//电机反向控制端口为：PD0,  0--反转，1--正转
//电机线对应管脚：红色--VCC				黑色--GND
//								黄色--方向控制	蓝色--PWM   
//							
//---------------------------------------------------

#include  "step_motor.h"
#include  "delay.h"
#include  "PID.h"
#include  "adc.h"
#include  "timer.h"

//当云台转到中间位置时所对应传感器的电压值
float mid_sensor_data = 1.6;


PID_PARA_VAL pid_data;

//电机方向控制管脚定义与电机参数初始化(PID参数)
void step_motor_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD0端口时钟

	//步进电机控制 DIR- PUL-分别对应PD0 PD1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);					
	DIR_CTRL(0);//默认逆时针转动
	
	//PWM初始化
	tim3_pwm_init(1800-1,2-1);//20KHZ,占空比设定值大于100电机才转动
	
	PID_ParaInit(&pid_data);//PID参数初始化
}

//闭环控制云台位置
//控制范围:-90度-+90度
//exp_position:控制的期望角度，-90度-+90度
//注：遥控器变化90度，传感器电压值变化0.9V
void holder_ctrl(int exp_position)
{
		float exp_dat = 0.0;
		Get_ADC_Value();
	
		//将输入的角度值转化为实际的电压值
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





















