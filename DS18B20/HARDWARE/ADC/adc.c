#include "sys.h"
#include "adc.h"
#include "can.h"

unsigned short int AD_Value[DTAT_NUM][CHAN_NUM];

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA 	, ENABLE );	  //使能GPIOA通道时钟

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
															|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;    //PT100模拟量通道的ADC端口   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_Remap_SWJ_JTAGDisable;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 

}
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1	, ENABLE );	//使能ADC1时钟
	
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None;//软件启动转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = CHAN_NUM;//转换通道数(共转换多少个通道)
	ADC_Init(ADC1,&ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_71Cycles5);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,5,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,6,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,7,ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,8,ADC_SampleTime_71Cycles5);
	
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1); 
	while(ADC_GetCalibrationStatus(ADC1));
}
void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;    //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;        //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = DTAT_NUM*CHAN_NUM;//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA通道x拥有高优先级    DMA_Priority_Medium
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

void ADC_Config_Init(void)
{
	GPIO_Configuration();
	ADC_Configuration();
	DMA_Configuration();
}


int get_value[CHAN_NUM];


float K_Temper_Value[4]={0,0,0,0};
unsigned char Vcc = 5;
unsigned char R1=100,R2=100,R3=100;
float PT100_Value[4] = {0,0,0,0};
float Uo1=0,Uo2=0,Uo3=0,Uo4=0;

float  K1_correction = 0.55;
float  K2_correction = 0.6;
float  K3_correction = 0.36;
float  K4_correction = 0.5;

float holder_dat = 0;//云台旋转角度

void Get_ADC_Value(void)
{
	unsigned  int sum=0;
	unsigned short int Count=0;
	unsigned char i=0;

	
	for(i=0;i<CHAN_NUM;i++)
	{
		for(Count=0;Count<DTAT_NUM;Count++)
		{
			sum += AD_Value[Count][i];
		}
		get_value[i] = sum/DTAT_NUM;
		sum = 0;
	}
	holder_dat = (float)get_value[0]*3.3/4096;
	
	
	//PT100温度转化  4路
	Uo1 = ((get_value[0]-180) * 3300 / 4096) / 100.0;
	PT100_Value[0] = ((((Uo1*R1*R3) + (Uo1*R2*R3) + (Vcc*R2*R3)) / ((Vcc*R1) - (Uo1*R1) - (Uo1*R2))) /4) * K1_correction;
	
	Uo2 = ((get_value[1]-180) * 3300 / 4096) / 100.0;
	PT100_Value[1] = ((((Uo2*R1*R3) + (Uo2*R2*R3) + (Vcc*R2*R3)) / ((Vcc*R1) - (Uo2*R1) - (Uo2*R2))) /4) * K2_correction;
	
	Uo3 = ((get_value[2]-180) * 3300 / 4096) / 100.0;
	PT100_Value[2] = ((((Uo3*R1*R3) + (Uo3*R2*R3) + (Vcc*R2*R3)) / ((Vcc*R1) - (Uo3*R1) - (Uo3*R2))) /4) * K3_correction;
	Uo4 = ((get_value[3]-180) * 3300 / 4096) / 100.0;
	PT100_Value[3] = ((((Uo4*R1*R3) + (Uo4*R2*R3) + (Vcc*R2*R3)) / ((Vcc*R1) - (Uo4*R1) - (Uo4*R2))) /4) * K4_correction;
	
}





