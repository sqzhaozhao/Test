#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"//strlen��memset�õ�
#include "USART.h"
#include "Delay.h"
#include "DS18B20.h"
 
extern unsigned char DS18B20_ID[8][8];//��⵽�Ĵ�����ID������
extern unsigned char DS18B20_SensorNum;
 
int main(void)
{
	u8 num=0;
	USART1_init(9600);
	while(DS18B20_Init())//��ʼ��DS18B20,����18B20
	{
		printf("DS18B20 Check Failed!\r\n");  
	}
	printf("DS18B20 Ready!\r\n");
	while(1)
	{	  
		DS18B20_Search_Rom();
		printf("DS18B20_SensorNum:%d\r\n",DS18B20_SensorNum);
	  for(num=0;num<DS18B20_SensorNum;num++)
		{
			printf("ID:%02x%02x%02x%02x%02x%02x%02x%02x TM:%.2f\r\n",DS18B20_ID[num][0],DS18B20_ID[num][1],DS18B20_ID[num][2],DS18B20_ID[num][3],DS18B20_ID[num][4],DS18B20_ID[num][5],DS18B20_ID[num][6],DS18B20_ID[num][7],DS18B20_Get_Temp(num));
		}
		printf("\r\n");
		Delay_s(2);
	}	
}
