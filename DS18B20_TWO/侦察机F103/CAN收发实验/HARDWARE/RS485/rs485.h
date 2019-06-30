#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//RS485���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	  		  	
extern u8 RS485_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���

//��ʱ����
volatile extern u16 time_out; 

//485�����������ʱʱ��(ms),������ֵ��Ϊһ֡���ݽ������
#define MAX_TIME_OUT 	10	

////ģʽ����
//#define RS485_TX_EN		PDout(7)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART3_RX 	1			//0,������;1,����.

//485���ͽ��տ��ƣ�NΪ1ʱΪ����ģʽ������Ϊ����ģʽ
#define TX_RX_EN(N) 	((N)?GPIO_SetBits(GPIOB,GPIO_Pin_12):GPIO_ResetBits(GPIOB,GPIO_Pin_12))


void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);
unsigned int Crc16(unsigned char *pBuf, unsigned char num);
void rs458_send_temp(uint8_t addr, int16_t temp);


#endif	   
















