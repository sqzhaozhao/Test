//---------------------------------------------------
//���ļ�ΪRS485ͨ�Ŵ���
//ת��оƬMAX485
//�˿ڣ�UART3,TX--B10, RX--B11
//			RE--C5, DE--C4
//ע����RE==0��DE == 0ʱ,���ݽ���;
//		��RE==1��DE == 1ʱ,���ݷ���
//����Ŀ�������ԲУ���RE��DEû�����ӣ���������������������
//---------------------------------------------------

#include "sys.h"	
#include "rs485.h"	 
#include "delay.h"
#include "string.h"


//��ʱʱ��
volatile u16 time_out = 0; 



#ifdef EN_USART3_RX   	//���ʹ���˽���


//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;   		  
  
void USART3_IRQHandler(void)
{
	u8 res;	    
 
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	 
		//��ʱ���
	 	if(time_out > MAX_TIME_OUT)	//MAX_TIME_OUT
		{
			time_out = 0;
			RS485_RX_CNT = 0;
			memset(RS485_RX_BUF, 0, sizeof(RS485_RX_BUF));
		}
		
		res =USART_ReceiveData(USART3); 	//��ȡ���յ�������
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1 
		} 
	}  											 
} 
#endif										 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);//ʹ��GPIOA,Dʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	
	//MAX485����/���տ������ţ�B12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD7�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//��ʼ��Ϊ����ģʽ
	TX_RX_EN(0);
 
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//ֹͣ��λ
 
	
 #ifdef EN_USART3_RX		  	//���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); ; //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
   
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

 #endif

//  RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
 
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	TX_RX_EN(1);			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART3,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	TX_RX_EN(0);				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}


//crc16У��
//pBuf--ҪУ������ݣ�ҪУ������ݵ��ֽ���
//����ֵ--�������У��ֵ
unsigned int Crc16(unsigned char *pBuf, unsigned char num) 
{ 
	unsigned char i,j; 
	unsigned int wCrc = 0xFFFF; 
	 
	for(i=0; i<num; i++) 
	{ 
		wCrc ^= (uint16_t)(pBuf[i]); 
		for(j=0; j<8; j++) 
		{ 
			if(wCrc & 1)
			{
				wCrc >>= 1; 
				wCrc ^= 0xA001; 
			} 
			else
			{		
				wCrc >>= 1;  
			}  
		 
		} 
	}
	return wCrc; 
}

void rs458_send_temp(uint8_t addr, int16_t temp)
{
	uint8_t buf[7];
	uint16_t crc;
	buf[0] = addr;
	buf[1] = 0x03;
	buf[2] = 0x01;
	buf[3] = (temp>>8) & 0xff;
	buf[4] = temp & 0xff;
	crc = Crc16(buf, 5);
	buf[5] = crc & 0xff;
	buf[6] = (crc>>8) & 0xff;
	RS485_Send_Data(buf, 7);
}

















