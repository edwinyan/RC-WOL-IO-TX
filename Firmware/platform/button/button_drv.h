#ifndef _BUTTON_DRV_H_
#define _BUTTON_DRV_H_

typedef enum{
    BUTTON_SRC_START = 0,	 
    BUTTON_SRC_FIRE=BUTTON_SRC_START,		//火警
    BUTTON_SRC_TERROR,						//匪警
    BUTTON_SRC_AMBULANCE,					//救护
    BUTTON_SRC_WHISTLE,						//汽笛
    BUTTON_SRC_BACKUP,						//备用
    BUTTON_SRC_LED1,						//警灯1
    BUTTON_SRC_LED2,						//警灯2
    BUTTON_SRC_VOICE ,    					//喊话
	
	BUTTON_SRC_NUM
}button_src_enum;

typedef struct {
	u8 last_value;	//上一次的按键电平值
	u8 button_value;	//当前的按键电平值
	u8 button_press;	//按键按下
	u8 button_state;	//按键状态值	1:触发 0:不触发
} button_t;

#define BUTTION_VOICE_GPIO		GPIOD
#define BUTTION_VOICE_PIN		GPIO_Pin_2

#define BUTTION_FIRE_GPIO		GPIOB
#define BUTTION_FIRE_PIN		GPIO_Pin_3

#define BUTTION_TERROR_GPIO		GPIOB
#define BUTTION_TERROR_PIN		GPIO_Pin_4

#define BUTTION_AMBULANCE_GPIO	GPIOB
#define BUTTION_AMBULANCE_PIN	GPIO_Pin_5

#define BUTTION_WHISTLE_GPIO	GPIOB
#define BUTTION_WHISTLE_PIN		GPIO_Pin_6

#define BUTTION_BACKUP_GPIO		GPIOB
#define BUTTION_BACKUP_PIN		GPIO_Pin_7

#define BUTTION_LED1_GPIO		GPIOB
#define BUTTION_LED1_PIN		GPIO_Pin_8

#define BUTTION_LED2_GPIO		GPIOB
#define BUTTION_LED2_PIN		GPIO_Pin_9

#define FIRE_ALARM			0x01
#define TERROR_ALARM		0x02
#define AMBULANCE_ALARM		0x04
#define WHISTLE_ALARM		0x08
#define BUTTON_BACKUP		0x10
#define BUTTON_LED1			0x20
#define BUTTON_LED2			0x40

void button_drv_init(void);
u8 button_value_get(button_src_enum src);
void button_scan(void);

#endif
