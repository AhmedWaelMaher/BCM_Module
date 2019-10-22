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

#define RECEIVE 		0
#define SEND 			1
#define SETUP_INTERRUPT SREG |=(1<<7);
#define SIZE 			33

void consumerApp(void);
void consumerApp_1(void);

uint8 arr[SIZE]= {};
uint8 arr_1[SIZE]={
		'B','B','B','B','B',
};

uint8 app_counter=0;

void consumerApp(void){
	LCD_displayString(arr);
	consumer_setCallBack(consumerApp_1);
	BCM_Send(SIZE,arr_1);
	app_counter++;
}

void consumerApp_1(void){
	LCD_goToRowColumn(1,0);
	LCD_displayString(arr_1);
	app_counter++;
}

int main(void)
{
	LCD_init();
	consumer_setCallBack(consumerApp);
	BCM_UnlockRX();
	setup_Buffer(arr);
	BCM_Init(Bcm_Confg_St_Ptr);
	SETUP_INTERRUPT
	while (1)
	{
		if(RECEIVE == app_counter){
			BCM_Dispatcher_RX();
		}
		else if(SEND == app_counter){
			BCM_Dispatcher_TX();
		}
		else{
		}
	}
}

