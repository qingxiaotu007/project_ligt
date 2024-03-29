#include "uart.h"

//����printf��ӡ�˿ڵ�ѡ��
#define USARTX USART1

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  ����ָ�����ȵ��ֽ� **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//�ȴ����ݷ��ͳɹ�
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* ����һ���ֽ����ݵ�USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;

	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USARTX, (uint8_t) ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(USARTX, USART_FLAG_TXE) == RESET);		

	return (ch);
}

//�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(USARTX, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USARTX);
}


//����1��ʼ��
void USART1_Config(uint32_t USART_BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART1 configuration
	USART_InitStructure.USART_BaudRate = USART_BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
	/* Enable the USARTz Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USARTz Receive Interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}


//����2��ʼ��
void USART2_Config(uint32_t USART_BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART configuration
	USART_InitStructure.USART_BaudRate = USART_BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    
	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART Receive Interrupt */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
}

//����3��ʼ��
void USART3_Config(uint32_t USART_BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//USART configuration
	USART_InitStructure.USART_BaudRate = USART_BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
	
	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART Receive Interrupt */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
}

uint8_t Rx1Counter = 0;
uint8_t Rx1Data = 0;
uint8_t Rx1Buffer[256];

//This function handles USART1 global interrupt request.
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		Rx1Data = USART_ReceiveData(USART1);

		Rx1Buffer[Rx1Counter++] = Rx1Data;
	}
	
	//�ж������Ƿ������ϣ����ж������ռ�������һ�������Ƿ�Ϊ������־λ��.��
	if( Rx1Buffer[Rx1Counter-1] == '.' )
	{
		//�жϽ��յ����ݵ���ȷ�ԣ����ж������Ƿ�Ϊ�����
		if( strcmp( (char *)Rx1Buffer, "LED_ON.") == 0 )
		{
			//������ȷ��������Ӧ������
			//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			GPIOC->ODR ^= GPIO_Pin_13;
		}
		//���ô�����ݵ�λ��
		Rx1Counter = 0;           
		//���������ʷ����
		memset(Rx1Buffer, 0, sizeof(Rx1Buffer) );
	}

	//�����ݷ��ͻط��ͷ���������
	USART_SendData(USART1, Rx1Data); 
	
	//�����־λ
	USART_ClearITPendingBit(USART1, USART_IT_RXNE | USART_IT_TXE);
}

uint8_t Rx2Counter = 0;
uint8_t Rx2Data = 0;
uint8_t Rx2Buffer[256];

//This function handles USART1 global interrupt request.
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		Rx2Data = USART_ReceiveData(USART2);

		Rx2Buffer[Rx2Counter++] = Rx2Data;
	}
	
	//�ж������Ƿ������ϣ����ж������ռ�������һ�������Ƿ�Ϊ������־λ��.��
	if( Rx2Buffer[Rx2Counter-1] == '.' )
	{
		//�жϽ��յ����ݵ���ȷ�ԣ����ж������Ƿ�Ϊ�����
		if( strcmp( (char *)Rx2Buffer, "LED_ON.") == 0 )
		{
			//������ȷ��������Ӧ������
			//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			GPIOC->ODR ^= GPIO_Pin_13;
		}
		//���ô�����ݵ�λ��
		Rx2Counter = 0;           
		//���������ʷ����
		memset(Rx2Buffer, 0, sizeof(Rx2Buffer) );
	}

	//�����ݷ��ͻط��ͷ���������
	USART_SendData(USART2, Rx2Data); 
	
	//�����־λ
	USART_ClearITPendingBit(USART2, USART_IT_RXNE | USART_IT_TXE);
}

uint8_t	 Tx3Buffer[512];
volatile uint32_t	Rx3Counter	= 0;
volatile uint8_t	Rx3Data 	= 0;
volatile uint8_t	Rx3End 		= 0;
volatile uint8_t	Rx3Buffer[512]={0};

//This function handles USART1 global interrupt request.
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		Rx3Data = USART_ReceiveData(USART3);

		Rx3Buffer[Rx3Counter++] = Rx3Data;
	
		if(Rx3Counter >= sizeof( Rx3Buffer))
		{
			Rx3Counter = 0;
			Rx3End=1;
		}
	}
	
#if WIFI_AP
	//ע�⣺������͵����ݽ�������'!'�����͵����ݻᱣ���������У���ʱ��������˰���'!'����ȷ���ݣ����һ��������������飬��Ҫ�ٴη�����ȷ���ݲ���׼ȷ�жϣ�
	//�ж������Ƿ������ȫ�����ж�����ռ�������һ�������Ƿ�Ϊ������־λ����!��,���������Զ��壻
	if( Rx3Buffer[Rx3Counter-1] == '!')
	{
		if( strstr((char *)Rx3Buffer, "led1!") != NULL )		//���յ�����ʵ����Ϊ��led1#��
		{
			GPIOB->ODR ^= GPIO_Pin_3;							//��ת��״̬
			GPIOC->ODR ^= GPIO_Pin_13;
		}
		if( strstr((char *)Rx3Buffer, "led2!") != NULL)
		{
			GPIOB->ODR ^= GPIO_Pin_4;
		}
		if( strstr((char *)Rx3Buffer, "NO!") != NULL)
		{
			GPIOA->ODR ^= GPIO_Pin_5;
		}
		
		//���ڽ�������λ����0���������
		Rx3Counter = 0;
		memset((char *)Rx3Buffer, 0, sizeof(Rx3Buffer));
	}
#endif
	
	//�����ݷ��ͻص��������֣�������
	USART_SendData(USART1, Rx3Data); 
	
	//�����־λ
	USART_ClearITPendingBit(USART1,  USART_IT_TXE);
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}
