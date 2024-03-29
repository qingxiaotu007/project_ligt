#include "uart.h"

//串口printf打印端口的选择
#define USARTX USART1

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  发送指定长度的字节 **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//等待数据发送成功
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for(i=0; i<num; i++)
	{
		/* 发送一个字节数据到USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do 
	{
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while(*(str + k)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USARTX, (uint8_t) ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USARTX, USART_FLAG_TXE) == RESET);		

	return (ch);
}

//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(USARTX, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USARTX);
}


//串口1初始化
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


//串口2初始化
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

//串口3初始化
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
	
	//判断数据是否接收完毕，即判断连续空间里的最后一个数据是否为结束标志位“.”
	if( Rx1Buffer[Rx1Counter-1] == '.' )
	{
		//判断接收到数据的正确性，即判断数据是否为所需的
		if( strcmp( (char *)Rx1Buffer, "LED_ON.") == 0 )
		{
			//数据正确，则做相应的事情
			//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			GPIOC->ODR ^= GPIO_Pin_13;
		}
		//重置存放数据的位置
		Rx1Counter = 0;           
		//清空数组历史数据
		memset(Rx1Buffer, 0, sizeof(Rx1Buffer) );
	}

	//把数据发送回发送方，即回显
	USART_SendData(USART1, Rx1Data); 
	
	//清除标志位
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
	
	//判断数据是否接收完毕，即判断连续空间里的最后一个数据是否为结束标志位“.”
	if( Rx2Buffer[Rx2Counter-1] == '.' )
	{
		//判断接收到数据的正确性，即判断数据是否为所需的
		if( strcmp( (char *)Rx2Buffer, "LED_ON.") == 0 )
		{
			//数据正确，则做相应的事情
			//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			GPIOC->ODR ^= GPIO_Pin_13;
		}
		//重置存放数据的位置
		Rx2Counter = 0;           
		//清空数组历史数据
		memset(Rx2Buffer, 0, sizeof(Rx2Buffer) );
	}

	//把数据发送回发送方，即回显
	USART_SendData(USART2, Rx2Data); 
	
	//清除标志位
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
	//注意：如果发送的数据结束符非'!'，发送的数据会保存在数组中，此时如果发送了包含'!'的正确数据，则第一个动作是清空数组，需要再次发送正确数据才是准确判断；
	//判断数据是否接收完全，即判断数组空间里的最后一个数据是否为结束标志位：‘!’,结束符可自定义；
	if( Rx3Buffer[Rx3Counter-1] == '!')
	{
		if( strstr((char *)Rx3Buffer, "led1!") != NULL )		//接收到的真实数据为“led1#”
		{
			GPIOB->ODR ^= GPIO_Pin_3;							//翻转灯状态
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
		
		//串口接收数据位置置0，清空数组
		Rx3Counter = 0;
		memset((char *)Rx3Buffer, 0, sizeof(Rx3Buffer));
	}
#endif
	
	//把数据发送回到串口助手，即回显
	USART_SendData(USART1, Rx3Data); 
	
	//清除标志位
	USART_ClearITPendingBit(USART1,  USART_IT_TXE);
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}
