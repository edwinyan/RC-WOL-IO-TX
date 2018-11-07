#ifndef _LED_DRV_H_
#define _LED_DRV_H_

typedef enum{
    LED_SRC_START = 0,
    LED_SRC_1 = LED_SRC_START,
    LED_SRC_2,
    LED_SRC_3,
    LED_SRC_4,

    LED_SRC_NUM
}led_src_enum;

#define LED_1_GPIO			GPIOB
#define LED_1_PIN			GPIO_Pin_13

#define LED_2_GPIO			GPIOB
#define LED_2_PIN			GPIO_Pin_14

#define LED_3_GPIO			GPIOB
#define LED_3_PIN			GPIO_Pin_15

#define LED_4_GPIO			GPIOB
#define LED_4_PIN			GPIO_Pin_12


void led_drv_init(void);
void led_on(led_src_enum src);
void led_off(led_src_enum src);


#define LED_R_ON      led_on(LED_SRC_4)
#define LED_G_ON      led_on(LED_SRC_1)
#define LED_B_ON      led_on(LED_SRC_2)
#define LED_W_ON	  led_on(LED_SRC_3)

#define LED_R_OFF     led_off(LED_SRC_4)
#define LED_G_OFF     led_off(LED_SRC_1)
#define LED_B_OFF     led_off(LED_SRC_2)
#define LED_W_OFF     led_off(LED_SRC_3)

#endif
