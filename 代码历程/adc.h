#ifndef __adc_H
#define __adc_H 
 
 
 
void Adc_Init(void);                                   
uint16_t AD_GetValue(u8 ch);                                       
uint16_t Get_Adc_Average(u8 ch,u8 times);
void Adc2_Init(void);                                   
uint16_t AD2_GetValue(u8 ch);                                       
uint16_t Get_Adc2_Average(u8 ch,u8 times);
 
#endif
