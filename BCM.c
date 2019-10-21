/******************************************************************************
 *
 * Module: BCM
 *
 * File Name: BCM.c
 *
 * Description: Source file for the BCM driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "BCM.h"

volatile uint32 g_Counter =0;
volatile sint8 g_State =OUT_OF_SERVICE;
volatile uint16 g_flagISR =0;
volatile uint16 g_flag =0;
volatile uint8 RX_data = 0;
volatile uint8 checksum =0;
volatile uint32 index_rx =0;
volatile uint8 Frame = NOT_COMPLETED;
volatile void (*g_consumer_callBackFun)(void) = NULL;

void consumer_setCallBack(void(*ptr)(void)){

	g_consumer_callBackFun = ptr;
}

void g_callBackFunction_TX(void){
	g_Counter++;
}

void g_callBackFunction_RX(void){

	static volatile uint32 RX_data =0;
	UART_recieveByte(&RX_data);


	if(g_State == UNLOCK){

		if(g_flagISR >= 3){
			if(index_rx != Bcm_Frame_St.No_Of_Bytes){
				Bcm_Frame_St.Buffer[index_rx] = RX_data;
				Bcm_Frame_St.Check_Sum ^= Bcm_Frame_St.Buffer[index_rx];
				index_rx++;
			}
			else{

				checksum = RX_data;
				Frame = COMPLETED;


			}
		}
		else if((g_flagISR  <= 2) && ((Bcm_Frame_St.BCM_id == RX_data) || (g_flag ==1))){
			if(0 == g_flagISR){

				g_flagISR++;
				g_flag++;
			}
			else if(1 == g_flagISR){
				Bcm_Frame_St.No_Of_Bytes = (uint8)RX_data;
				Bcm_Frame_St.No_Of_Bytes = ((Bcm_Frame_St.No_Of_Bytes)<<8);
				g_flagISR++;

			}
			else if(2 == g_flagISR){

				Bcm_Frame_St.No_Of_Bytes = ((uint16)RX_data | (uint16)Bcm_Frame_St.No_Of_Bytes);
				g_flagISR++;

			}
		}
	}
}

uint8 BCM_Init(Bcm_Confg* Ptr){
	Set_Call_Back_RX(g_callBackFunction_RX);
	Set_Call_Back_TX(g_callBackFunction_TX);
	uint8 status =OK;
	switch (Ptr->id){
	case UART:

		status= UART_init(Confg_Uart_Ptr);
		break;
	case SPI:
		break;
	case I2C:

		break;
	default:
		status=NOK;
	}
	return status;
}

uint8 BCM_Send(uint16 Num_Of_bytes,uint8*Adderss){
	uint8 status=OK;

	if(g_State==OUT_OF_SERVICE){

		Bcm_Frame_St.BCM_id=BCM_ID;
		Bcm_Frame_St.Buffer=Adderss;
		Bcm_Frame_St.No_Of_Bytes=Num_Of_bytes;
		Bcm_Frame_St.Check_Sum=0;
		g_State=LOCK;
	}
	return status;
}

uint8 BCM_Dispatcher_TX(void){
	uint8 status=OK;
	static uint8 counts =1;
	static uint32 index_tx =0;
	if(LOCK == g_State){
		if(counts != g_Counter){
			counts = g_Counter;
			if(IDLE == g_Counter){
				status=UART_sendByte(Bcm_Frame_St.BCM_id);
			}

			else if(SENDING_ADDRESS == g_Counter){

				status = UART_sendByte(((uint16)Bcm_Frame_St.No_Of_Bytes)>>8);

			}
			else if(SENDING_ADDRESS_2 == g_Counter){

				status = UART_sendByte((uint8)Bcm_Frame_St.No_Of_Bytes);

			}
			else if(SENDING_BYTE <= g_Counter){
				if(index_tx != Bcm_Frame_St.No_Of_Bytes){
					Bcm_Frame_St.Check_Sum ^= Bcm_Frame_St.Buffer[index_tx];
					status = UART_sendByte(Bcm_Frame_St.Buffer[index_tx]);
					index_tx++;
				}
				else{
					g_State=UNLOCK;
					g_Counter =0;
					status=UART_sendByte(Bcm_Frame_St.Check_Sum);
					if(g_consumer_callBackFun != NULL){
						g_consumer_callBackFun();
					}
				}
			}
		}
	}
	return status;
}

void BCM_UnlockRX(void){

	g_State = UNLOCK;
}

void setup_Buffer(uint8 *l_bufferPtr){

	Bcm_Frame_St.Check_Sum =0;
	Bcm_Frame_St.BCM_id = BCM_ID;
	Bcm_Frame_St.Buffer =l_bufferPtr;
}

uint8 BCM_Dispatcher_RX(void){

	uint8 status=OK;

	if(g_State == UNLOCK){
		if(Frame == COMPLETED){
			if(index_rx >= (Bcm_Frame_St.No_Of_Bytes)){

				if(Bcm_Frame_St.Check_Sum == checksum){
					g_flag=0;
					g_flagISR=0;
					g_State = LOCK;
					if(g_consumer_callBackFun != NULL){
						g_consumer_callBackFun();
					}
				}
				else{

				}
			}
			else{

			}
		}
	}
	return status;
}
