#include "common.h"
#include "stm32f4xx_gpio.h"
#include "led_drv.h"

/*----------------------------------------------------------------------------*/

typedef struct{
    GPIO_TypeDef *port;
    u16         pin;
}led_config_t;


STATIC led_config_t led_config_array[LED_SRC_NUM] = {
    {LED_1_GPIO, LED_1_PIN},
    {LED_2_GPIO, LED_2_PIN},
    {LED_3_GPIO, LED_3_PIN},
    {LED_4_GPIO, LED_4_PIN}, 
};

/*----------------------------------------------------------------------------*/
//global functions
void led_drv_init(void)
{
    GPIO_InitTypeDef  gpio_init;
    led_src_enum src;
    
    gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;

    for(src = LED_SRC_START; src < LED_SRC_NUM; src++)
    {
        gpio_init.GPIO_Pin = led_config_array[src].pin;
        GPIO_Init(led_config_array[src].port, &gpio_init);
    }

    LED_R_OFF;
    LED_G_OFF;
    LED_B_OFF;

}

void led_on(led_src_enum src)
{
    ASSERT_D(src < LED_SRC_NUM);
    GPIO_SetBits(led_config_array[src].port, led_config_array[src].pin);
}

void led_off(led_src_enum src)
{
    ASSERT_D(src < LED_SRC_NUM);
    GPIO_ResetBits(led_config_array[src].port, led_config_array[src].pin);
}

