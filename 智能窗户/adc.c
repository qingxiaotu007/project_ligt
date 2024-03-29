#include "stm32f10x.h"    //������Ҫ��ͷ�ļ�
#include "delay.h"	      //������Ҫ��ͷ�ļ�
#include "adc.h"
 
void Adc_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	
	//1������ʱ��ADC1��ʱ�Ӻ�GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE );
	//2�����÷�Ƶ����Ϊ����ܳ���14MHz 72/6=12
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//3������GPIOA��ADC�ṹ�� PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	ADC_DeInit(ADC1); //��λADC1
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	//4��ʹ��ADC1
	ADC_Cmd(ADC1, ENABLE);
	//5��У׼���ȴ�У׼����
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
	
}

void Adc2_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 1������ʱ��ADC2��ʱ�Ӻ�GPIOB��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // 2�����÷�Ƶ����Ϊ����ܳ���14MHz 72/6=12
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   // ����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
    // 3������GPIOB��ADC�ṹ�� PB0 ��Ϊģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;        // ģ����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    ADC_DeInit(ADC2); // ��λADC2

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;       // ģ��ת�������ڵ�ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // ģ��ת�������ڵ���ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1; // ˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC2, &ADC_InitStructure); // ����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
    // 4��ʹ��ADC2
    ADC_Cmd(ADC2, ENABLE);
    // 5��У׼���ȴ�У׼����
    ADC_ResetCalibration(ADC2); // ʹ�ܸ�λУ׼

    while (ADC_GetResetCalibrationStatus(ADC2)); // �ȴ���λУ׼����

    ADC_StartCalibration(ADC2); // ����ADУ׼

    while (ADC_GetCalibrationStatus(ADC2)); // �ȴ�У׼����
}

 
uint16_t AD_GetValue(u8 ch)
{
		//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  		
	 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
 
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
 
uint16_t AD2_GetValue(u8 ch)
{
	// ����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_239Cycles5); // ADC2, ADCͨ��, ����ʱ��Ϊ239.5����

	ADC_SoftwareStartConvCmd(ADC2, ENABLE); // ʹ��ָ����ADC2�����ת����������

	while (!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)); // �ȴ�ת������

	return ADC_GetConversionValue(ADC2); // �������һ��ADC2�������ת�����
}


//���ȡ����������ƽ��ֵ���ȶ�
uint16_t Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD_GetValue(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 


uint16_t Get_Adc2_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD2_GetValue(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 
