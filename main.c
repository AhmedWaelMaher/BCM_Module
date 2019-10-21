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

#define SIZE 33

void App(void);
void App_1(void);

uint8 arr[SIZE]= {};
uint8 arr_1[SIZE]={
		'B','B','B','B','B',
};

uint8 app=0;

void App(void){
	LCD_displayString(arr);
	consumer_setCallBack(App_1);
	BCM_Send(SIZE,arr_1);
	app++;
}

void App_1(void){
	LCD_displayString(arr_1);
	app++;
}

int main(void)
{
	LCD_init();
	consumer_setCallBack(App);
	BCM_UnlockRX();
	setup_Buffer(arr);
	BCM_Init(Bcm_Confg_St_Ptr);
	SREG |=(1<<7);

	while (1)
	{
		if(app==0){
		BCM_Dispatcher_RX();
		}
		else if(app ==1){
			BCM_Dispatcher_TX();
		}
		else{

		}
	}
}

