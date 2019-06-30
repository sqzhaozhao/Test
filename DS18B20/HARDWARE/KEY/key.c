#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
 
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
																	GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_13|GPIO_Pin_14;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
}


u8 KEY_Scan(u8 mode)
{	 
	 u8 key_up=1;//�������ɿ���־
	uint8_t key_value = 0xff;
	key_value = (KEY7<<7)|(KEY6<<6)|(KEY5<<5)|(KEY4<<4)|
							(KEY3<<3)|(KEY2<<2)|(KEY1<<1)|KEY0;
	return key_value;
}
