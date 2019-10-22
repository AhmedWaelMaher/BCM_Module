/******************************************************************************
 *
 * Module: DIO
 *
 * File Name: Dio.c
 *
 * Description: Source file for the DIO AVR driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "BCM.h"
#include "Uart.h"
#include "lcd.h"

#define RECEIVE 		1
#define SEND 			0
#define SETUP_INTERRUPT SREG |=(1<<7);
#define SIZE 			33

void consumerApp(void);
void consumerApp_1(void);

uint8 arr_1[SIZE]= {};
uint8 arr[SIZE]= {
		//'A','B','C','D','A','B','C','D','A','B','C','D','A','B','C','D','A','B','C','D','A','B','C','D','A','B','C','D','A','B','C','D','#'
		//1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		'A','A','A','A','A'
};

uint8 app_counter=0;

void consumerApp(void){
	LCD_displayString(arr);
	app_counter++;
	consumer_setCallBack(consumerApp_1);
	BCM_UnlockRX();
	setup_Buffer(arr_1);
}

void consumerApp_1(void){
	LCD_goToRowColumn(1,0);
	LCD_displayString(arr_1);
	app_counter++;
}

int main(void)
{
	DDRA =0xff;
	LCD_init();
	consumer_setCallBack(consumerApp);
	BCM_Init(Bcm_Confg_St_Ptr);
	BCM_Send(SIZE,arr);
	SETUP_INTERRUPT
	while (1)
	{
		if(SEND == app_counter){
			BCM_Dispatcher_TX();
		}
		else if(RECEIVE == app_counter){
			BCM_Dispatcher_RX();
		}
		else{
		}
	}
}

