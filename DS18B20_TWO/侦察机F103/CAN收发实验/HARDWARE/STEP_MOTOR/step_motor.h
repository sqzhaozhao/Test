#ifndef _STEP_MOTOR_H
#define _STEP_MOTOR_H
#include "sys.h"

//�������,0--��ʱ��ת���� 1˳ʱ��ת��
#define DIR_CTRL(n)		(n?GPIO_SetBits(GPIOD,GPIO_Pin_0):GPIO_ResetBits(GPIOD,GPIO_Pin_0))

////�����ź�
//#define CP_CTRL(n)		(n?GPIO_SetBits(GPIOD,GPIO_Pin_1):GPIO_ResetBits(GPIOD,GPIO_Pin_1))

////���ڼ�����Ҽ���λ�õĹ�翪��
//#define LIM_SWITCH        	GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_2)

////���ڼ�����λ�õĹ�翪��
//#define ZERO_SWITCH        GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_3)

void step_motor_init(void);
//void step_motor(uint8_t direction, uint16_t beat, uint16_t frequent);
void holder_ctrl(int exp_position);



#endif


