#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "can.h" 


 int main(void)
 {	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	 
 	while(1)
	{
		
		
		delay_ms(100);
	}
}



