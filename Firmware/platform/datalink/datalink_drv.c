#include "common.h"
#include "bsp.h"
#include "bsp_os.h"
#include "string.h"
#include "stdio.h"
#include "uart_drv.h"
#include "adc_drv.h"
#include "datalink_drv.h"
#include "button_drv.h"
#include "led_drv.h"


#define PACK_LEN	20

#define PACKET_HEAD1	0xAA
#define PACKET_HEAD2	0xAA
#define PACKET_TAIL1	0x55
#define PACKET_TAIL2	0x55

#define TYPE_ID			0x00
#define TYPE_DATA		0x01

extern u32 deviceID;
extern button_t button[7];
extern FIFO_T stFiFo;

u8 buffer[255]={0};
u8 rssi[]="RSSI";
vu8 stamp=0;	//心跳包时间戳
bool connected=FALSE;

void heart_beat_check(void)
{
	u8 read=0;
	u16 data_len=0;
	u8 i=0;
	
	if(Fifo_DataLen(&stFiFo))
	{
		memset(buffer,0x00,sizeof(buffer));
		
		data_len = Fifo_DataLen(&stFiFo);

		while(data_len--)
		{
			if(Fifo_Read(&stFiFo,&read))
				buffer[i++] = read;
		}

		if(strstr(buffer,rssi) != NULL)
		{
			//we got a heart beat packet
			stamp = 25;
			connected= TRUE;
			//MSG("got a heart beat packet\r\n");
		}
	}
}


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

	ret &= 0x07;    //只使用低3位
	
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

void datalink_packet(void)
{
	s16 adcTemp[2]={0};
	u8 packet[PACK_LEN]={0};
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



