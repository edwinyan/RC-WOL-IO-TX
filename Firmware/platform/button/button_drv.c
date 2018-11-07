#include "common.h"
#include "stm32f4xx_gpio.h"
#include "button_drv.h"
#include "led_drv.h"

/*----------------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *port;
    u16         pin;
	//GPIOPuPd_TypeDef pupd;
}button_config_t;

STATIC button_config_t button_config_array[BUTTON_SRC_NUM] = {
	{BUTTION_FIRE_GPIO, BUTTION_FIRE_PIN},
	{BUTTION_TERROR_GPIO, BUTTION_TERROR_PIN },
	{BUTTION_AMBULANCE_GPIO, BUTTION_AMBULANCE_PIN},
	{BUTTION_WHISTLE_GPIO, BUTTION_WHISTLE_PIN},
	{BUTTION_BACKUP_GPIO, BUTTION_BACKUP_PIN},	
	{BUTTION_LED1_GPIO, BUTTION_LED1_PIN},
	{BUTTION_LED2_GPIO, BUTTION_LED2_PIN},
	{BUTTION_VOICE_GPIO, BUTTION_VOICE_PIN},
};

button_t button[7] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};	
/*----------------------------------------------------------------------------*/
//global functions
void button_drv_init(void)
{
    GPIO_InitTypeDef  gpio_init;
    button_src_enum src;
    
    gpio_init.GPIO_Mode  = GPIO_Mode_IN;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_DOWN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;

    for(src = BUTTON_SRC_START; src < BUTTON_SRC_NUM; src++)
    {
        gpio_init.GPIO_Pin = button_config_array[src].pin;
        GPIO_Init(button_config_array[src].port, &gpio_init);
    }
}


u8 button_value_get(button_src_enum src)
{
    ASSERT_D(src < BUTTON_SRC_NUM);
    return GPIO_ReadInputDataBit(button_config_array[src].port, button_config_array[src].pin);
}

void button_scan(void)
{
	u8 i,j;

#if 1
	for(i=0;i<7;i++)
	{
		button[i].button_value = button_value_get(i);
		
		if(button[i].button_value == 1 && button[i].last_value == 0)
		{
			switch(i)
			{
				case BUTTON_SRC_LED1:
				case BUTTON_SRC_LED2:
					button[i].button_state ++;
					if(button[i].button_state %2 != 0)
					{
						LED_R_ON;
					}else{
						LED_R_OFF;
					}
					if(button[i].button_state == 4)
						button[i].button_state = 0;
					break;
				case BUTTON_SRC_FIRE:
				case BUTTON_SRC_TERROR:			//beep
				case BUTTON_SRC_AMBULANCE:		//dudu
				//case BUTTON_SRC_WHISTLE:		//ÈËÉù
					if(button[i].button_state==1)
					{
						button[i].button_press=0;
					}else{
						button[i].button_press=1;						
					}
					for(j=0;j<4;j++)
					{
						button[j].button_state=0;
					}
						
					button[i].button_state = button[i].button_press;
					break;
				default:
					break;
			}		
			//button[i].button_press = 1;
		}
		
		button[i].last_value = button[i].button_value;		
	}
		
#endif
}

/*----------------------------------------------------------------------------*/



