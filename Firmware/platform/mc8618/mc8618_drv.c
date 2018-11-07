#include "common.h"
#include "stm32f4xx_gpio.h"
#include "mc8618_drv.h"
#include "bsp.h"
#include "bsp_os.h"
#include "uart_drv.h"
#include "button_drv.h"
#include "led_drv.h"

/*
mc8618 data packet

20bytes

byte0 - byte3   : deviceID
byte4 - byte5   : data head
byte6 - byte9   : 摇杆 adc
byte10          : buttons value
byte11 - byte14 : backup
byte15 - byte16 : crc16 checksum
byte17 - byte18 : data tail

完整的数据包: AT+ZIPSEND=0,20 0x0D deviceID head adc button backup crc16 tail
*/
/*----------------------------------------------------------------------------*/
u8 VOICE_SEND[]="AT+POC=10000000\r\n";
u8 VOICE_END[]="AT+POC=10000001\r\n";

//#define PACK_LEN	20

//extern button_t button[7];

u32 deviceID=0;
//u8 packet[PACK_LEN]={0};
//static u8 identifier[8] = {0};

void get_deviceID(void)
{
	u32 id[3];
	
	id[0] = *(vu32*)(CPUID_ADDR);
	id[1] = *(vu32*)(CPUID_ADDR+4);
	id[2] = *(vu32*)(CPUID_ADDR+8);

	deviceID = (id[0]>>1)+(id[1]>>2)+(id[2]>>3); //compute device id
	//sprintf(identifier,"%x",deviceID);
	//MSG("device ID = 0x%x\r\n",deviceID);
}

void mc8618_gpio_init(void)
{
	GPIO_InitTypeDef  gpio_init;

	gpio_init.GPIO_Pin 	 = MC8618_VOICE_POWER_PIN;
	gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_DOWN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(MC8618_VOICE_POWER_GPIO, &gpio_init);

	BSP_OS_TimeDly(1000);   //delay 1s
	MC8618_VOICE_POWER_OFF;
	BSP_OS_TimeDly(3000);   //delay 3s
	MC8618_VOICE_POWER_ON;
}
#if 0
u8 getBtnValues(void)
{
	u8 ret=0;

	if(button[BUTTON_SRC_FIRE].button_state == 1){
		ret |= FIRE_ALARM;
	}else{
		ret &= ~FIRE_ALARM;
	}
	if(button[BUTTON_SRC_TERROR].button_state == 1){
		ret |= TERROR_ALARM;
	}else{
		ret &= ~TERROR_ALARM;
	}
	if(button[BUTTON_SRC_AMBULANCE].button_state == 1){
		ret |= AMBULANCE_ALARM;
	}else{
		ret &= ~AMBULANCE_ALARM;
	}
	#if 0
	if(button[BUTTON_SRC_WHISTLE].button_state == 1){
		ret |= WHISTLE_ALARM;
	}else{
		ret &= ~WHISTLE_ALARM;
	}
	#endif

	ret &= 0x07;    //只使用低5位
	
	return ret;
}

u8 checkbufferdata(u8 *data,u8 size)
{
	u8 ret;
	u16 sum=0;

	while(size--)
	{
		sum += *(data++);
	}

	ret = sum %256;

	return ret;
}

void data_packet(void)
{
	s16 adcTemp[2]={0};
	u8 i;

	adcTemp[0] = adc_getvalue(ADC_CHANNEL_UPDOWN);	//上大下小
	adcTemp[1] = 4095 - adc_getvalue(ADC_CHANNEL_LEFTRIGHT); //改成左小右大

	//adcTemp[0] = adcTemp[0]/45.5f ;			//映射到（0 - 90）
	//adcTemp[1] = adcTemp[1]/22.75f + 180;    //映射到(180 - 360)

	if(adcTemp[0] < 1900)
	{
		packet[7] = 2;
		packet[8] = (1900 - adcTemp[0])/256;
	}else if (adcTemp[0] > 2150){
		packet[7] = 1;
		packet[8] = (adcTemp[0] - 2150)/256;
	}else{
		packet[7] = 0;
		packet[8] = 0;
	}

	if(adcTemp[1] < 1900)
	{
		packet[9] = 1;
		packet[10] = (1900 - adcTemp[1])/256;
	}else if (adcTemp[1] > 2150){
		packet[9] = 2;
		packet[10] = (adcTemp[1] - 2150)/256;
	}else{
		packet[9] = 0;
		packet[10] = 0;
	}

	//MSG("adc1=%d\r\n",adcTemp[0]);
	//MSG("adc2=%d\r\n",adcTemp[1]);
	
	packet[0] = PACKET_HEAD1;
	packet[1] = PACKET_HEAD2;

	packet[2] = TYPE_DATA;

	packet[3] = deviceID >> 24;
	packet[4] = deviceID >> 16;
	packet[5] = deviceID >> 8;
	packet[6] = deviceID;
	//packet[7] = (adcTemp[0] >> 8);		//updown adc high value
	//packet[8] = adcTemp[0];				//updown adc low value
	//packet[9] = (adcTemp[1] >> 8);		//leftright adc high value
	//packet[10] = adcTemp[1];				//leftright adc low value
	packet[11] = getBtnValues();			//buttons value
	packet[12] = button[BUTTON_SRC_LED1].button_state;
	packet[13] = button[BUTTON_SRC_LED2].button_state;
	packet[14] = button[BUTTON_SRC_WHISTLE].button_value;
	packet[15] = button[BUTTON_SRC_BACKUP].button_value;
	packet[16] = 0x00;
	packet[17] = checkbufferdata(packet+2,14);
	packet[18] = PACKET_TAIL1;
	packet[19] = PACKET_TAIL2;
#if 0
	for(i=0;i<PACK_LEN;i++)
	{
		//packet[i]=packet[i];
		MSG("0x%x,",packet[i]);
	}
	MSG("\r\n");
#endif

	uart_drv_data_send(packet,20);
	LED_B_ON;
	
}

#endif

void mc8618_init(void)
{
	mc8618_gpio_init();
	//get_deviceID();
}


void mc8618_voice_start(void)
{
	uart_drv_dbg_msg(VOICE_SEND);
}

void mc8618_voice_end(void)
{
	uart_drv_dbg_msg(VOICE_END);
}

void mc8618_voice_handler(void)
{	
	static u8 open=0;
	static u8 send=0;
	
	//data_packet();
	if(button_value_get(BUTTON_SRC_VOICE) == 1 && send ==0)
	{
		//MSG("open voice port\r\n");
		LED_W_ON;
		mc8618_voice_start();
		BSP_OS_TimeDly(500);
		open=1;
		send=1;
	}else{
		if(open == 1 && button_value_get(BUTTON_SRC_VOICE) == 0){
			//MSG("close voice port\r\n");
			mc8618_voice_end();
			open=0;
			send=0;
			LED_W_OFF;
		}
		
	}
	//mc8618_data_send(packet);
}

