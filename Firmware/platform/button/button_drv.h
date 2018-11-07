#ifndef _BUTTON_DRV_H_
#define _BUTTON_DRV_H_

typedef enum{
    BUTTON_SRC_START = 0,	 
    BUTTON_SRC_FIRE=BUTTON_SRC_START,		//��
    BUTTON_SRC_TERROR,						//�˾�
    BUTTON_SRC_AMBULANCE,					//�Ȼ�
    BUTTON_SRC_WHISTLE,						//����
    BUTTON_SRC_BACKUP,						//����
    BUTTON_SRC_LED1,						//����1
    BUTTON_SRC_LED2,						//����2
    BUTTON_SRC_VOICE ,    					//����
	
	BUTTON_SRC_NUM
}button_src_enum;

typedef struct {
	u8 last_value;	//��һ�εİ�����ƽֵ
	u8 button_value;	//��ǰ�İ�����ƽֵ
	u8 button_press;	//��������
	u8 button_state;	//����״ֵ̬	1:���� 0:������
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
