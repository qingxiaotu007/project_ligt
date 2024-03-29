#include "hc_501.h"

void HC_501_Config(void)
{
  	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*ʹ��GPIOʱ�� PB4 ʹ��AFIOʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	/*����PB4��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode ��������ѡ�йܽŵĹ���״̬�� //��������ģʽ  �͵�ƽ��Ч
	GPIO_Init(GPIOB,&GPIO_InitStructure);//���� GPIO_InitStruct ��ָ���Ĳ�����ʼ������ GPIOx �Ĵ���
	
	/*����EXT4 �� PB4 ����*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);//ѡ�� GPIO �ܽ������ⲿ�ж���·
	
	/*����EXTI4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;//EXTI_Line ѡ���˴�ʹ�ܻ���ʧ�ܵ��ⲿ��·�� //EXTI_Line4�ⲿ�ж��� 4
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//���� EXTI ��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//EXTI_Trigger �����˱�ʹ����·�Ĵ������ء�//EXTI_Mode_Interrupt����������·�½���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//EXTI_LineCmd ��������ѡ����·����״̬
	EXTI_Init(&EXTI_InitStructure);//���� EXTI_InitStruct ��ָ���Ĳ�����ʼ������ EXTI �Ĵ���
	
	/*����EXTI4���ȼ����ж�������ڵ�ַ */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//NVIC_IRQChanne4 �ò�������ʹ�ܻ���ʧ��ָ���� IRQ ͨ����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//�ò��������˳�Ա NVIC_IRQChannel �е���ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�ò��������˳�Ա NVIC_IRQChannel �еĴ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�ò���ָ�����ڳ�Ա NVIC_IRQChannel �ж���� IRQ ͨ����ʹ�ܻ���ʧ�ܡ�
	NVIC_Init(&NVIC_InitStructure);//���� NVIC_InitStruct ��ָ���Ĳ�����ʼ������ NVIC �Ĵ���
}


 
