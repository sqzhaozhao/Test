//---------------------------------------------------
//本文件为RS485通信代码
//转化芯片MAX485
//端口：UART3,TX--B10, RX--B11
//			RE--C5, DE--C4
//注：当RE==0，DE == 0时,数据接收;
//		当RE==1，DE == 1时,数据发送
//购买的开发板很脑残，将RE与DE没有连接！！！！！！！！！！！
//---------------------------------------------------

#include "sys.h"	
#include "rs485.h"	 
#include "delay.h"
#include "string.h"


//超时时间
volatile u16 time_out = 0; 



#ifdef EN_USART3_RX   	//如果使能了接收


//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   		  
  
void USART3_IRQHandler(void)
{
	u8 res;	    
 
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	 
		//超时检测
	 	if(time_out > MAX_TIME_OUT)	//MAX_TIME_OUT
		{
			time_out = 0;
			RS485_RX_CNT = 0;
			memset(RS485_RX_BUF, 0, sizeof(RS485_RX_BUF));
		}
		
		res =USART_ReceiveData(USART3); 	//读取接收到的数据
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
		} 
	}  											 
} 
#endif										 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void RS485_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);//使能GPIOA,D时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
	//MAX485发送/接收控制引脚，B12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD7端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//初始化为接收模式
	TX_RX_EN(0);
 
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//停止复位
 
	
 #ifdef EN_USART3_RX		  	//如果使能了接收
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

  USART_Init(USART3, &USART_InitStructure); ; //初始化串口
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
   
  USART_Cmd(USART3, ENABLE);                    //使能串口 

 #endif

//  RS485_TX_EN=0;			//默认为接收模式
 
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	TX_RX_EN(1);			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART3,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	TX_RX_EN(0);				//设置为接收模式	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
}


//crc16校验
//pBuf--要校验的数据，要校验的数据的字节数
//返回值--计算出的校验值
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

















