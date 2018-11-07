#ifndef _ADC_DRV_H_
#define _ADC_DRV_H_

#define ADC1_IN6_PIN	GPIO_Pin_6
#define ADC1_IN7_PIN	GPIO_Pin_7

#define ADC_NUM_CHANNELS	2

#define ADC_CHANNEL_UPDOWN			0		
#define ADC_CHANNEL_LEFTRIGHT		1		

void  Adc_Init(void);
u16 adc_getvalue(u8 num);



#endif
