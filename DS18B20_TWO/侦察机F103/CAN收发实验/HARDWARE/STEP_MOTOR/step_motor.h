#ifndef _STEP_MOTOR_H
#define _STEP_MOTOR_H
#include "sys.h"

//方向控制,0--逆时针转动， 1顺时针转动
#define DIR_CTRL(n)		(n?GPIO_SetBits(GPIOD,GPIO_Pin_0):GPIO_ResetBits(GPIOD,GPIO_Pin_0))

////脉冲信号
//#define CP_CTRL(n)		(n?GPIO_SetBits(GPIOD,GPIO_Pin_1):GPIO_ResetBits(GPIOD,GPIO_Pin_1))

////用于检测左右极限位置的光电开关
//#define LIM_SWITCH        	GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_2)

////用于检测零点位置的光电开关
//#define ZERO_SWITCH        GPIO_ReadInputDataBit(GPIOD,GPIO_PIN_3)

void step_motor_init(void);
//void step_motor(uint8_t direction, uint16_t beat, uint16_t frequent);
void holder_ctrl(int exp_position);



#endif


