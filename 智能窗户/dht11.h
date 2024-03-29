#ifndef __RAIN_H
#define __RAIN_H

#include "stm32f10x.h"
#include <stdbool.h>
#include "delay.h"
#include "uart.h"

//#define  (n)  ( (n) ? GPIO_SetBits(GPIOA,GPIO_Pin_3) : GPIO_ResetBits(GPIOA,GPIO_Pin_3) )

//#define  RAIN_READ	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)

//void RAIN_Config(void);
//void RAIN_SetPinOutputMode(void);
//void RAIN_SetPinInputMode(void);
//void RAIN_StartSignal(void);
//bool  RAIN_GetACK(void);
//uint8_t RAIN_ReadBit(void);
//bool RAIN_GetValue(uint8_t buf[5


#include "stm32f10x.h"

 
/* 设置GPIO脚，默认为PB11 */
#define DHT11_IO 		            GPIOB
#define DHT11_PIN		            GPIO_Pin_7
#define DHT11_APB2PeriphRCC     RCC_APB2Periph_GPIOB
/* 初始化函数，如果DHT11存在响应则返回1，否则0 */
u8 DHT11_Init(void);
/* 从DHT11读取数据，没有小数部分 */
 u8 DHT11_Read_Data(u8 *temp,u8 *humi);



#endif
