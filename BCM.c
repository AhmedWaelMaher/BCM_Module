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

/*
 * Defines
 */
#define IDLE 				0
#define SENDING_ADDRESS 	1
#define SENDING_ADDRESS_2   2
#define SENDING_BYTE 		3
#define FRAME_COMPLETE 		(g_Counter-2)==Bcm_Frame_St.No_Of_Bytes

#define NOT_BUSY		    0
#define BUSY 				1

#define NOT_COMPLETED 	    1
#define COMPLETED		    0

#define OUT_OF_SERVICE	   -1
#define LOCK 				0
#define UNLOCK 				1

#define NOT_MATCHED 		0
#define MATCHED			    1

/*
 * Global variables used for only for the BCM
 */
static volatile uint32 g_Counter = IDLE;
static volatile sint8 BCM_State = OUT_OF_SERVICE;
static volatile uint32 g_FlagISR = IDLE;
static volatile uint8 ID_flag = NOT_MATCHED;
static volatile uint32 RX_data = IDLE;
static volatile uint32 checksum = IDLE;
static volatile uint32 index_rx = IDLE;
static volatile uint8 Frame = NOT_COMPLETED;
static volatile void (*g_consumer_callBackFun)(void) = NULL;

/*
 * Functions Prototypes for call back functions
 */
static void g_callBackFunction_TX(void);
static void g_callBackFunction_RX(void);

/*
 * Function call back to consumer returning the success or the failure of the transmitting or recieving
 */
void consumer_setCallBack(void(*ptr)(void)){
	/*Assigning the local pointer to a global pointer for call back function*/
	g_consumer_callBackFun = ptr;
}

/*
 * Call Back Functions for transmitter or receiver of UART
 */
void g_callBackFunction_TX(void){
	/*counting variable for transmitting*/
	g_Counter++;
}
void g_callBackFunction_RX(void){
	/*assign pointer to UDR of the UART driver*/
	UART_recieveByte(&RX_data);
	if(UNLOCK == BCM_State){
		/*
		 * check that the buffer is unlock "User predict receiving data"
		 */
		if( SENDING_BYTE <= g_FlagISR){
			/*
			 * while receiving bytes of data after checking the ID and number of bytes
			 */
			if(index_rx != Bcm_Frame_St.No_Of_Bytes){
				/*
				 * the end of the bytes
				 */
				Bcm_Frame_St.Buffer[index_rx] = RX_data;//assign the received data in the user buffer
				Bcm_Frame_St.Check_Sum ^= Bcm_Frame_St.Buffer[index_rx];//check sum Xor with each byte
				index_rx++;
			}
			else{
				checksum = RX_data;
				Frame = COMPLETED;//Frame Completed
			}
		}
		else if((SENDING_ADDRESS_2 >= g_FlagISR) && ((Bcm_Frame_St.BCM_id == RX_data) || (MATCHED == ID_flag))){
			/*
			 * Checking the ID and calculating the size
			 */
			if(IDLE == g_FlagISR){
				/*
				 * IDLE state till ID matched
				 */
				g_FlagISR++;
				ID_flag++;
			}
			else if(SENDING_ADDRESS == g_FlagISR){
				/*
				 * the first 8 bits of address
				 */
				Bcm_Frame_St.No_Of_Bytes = (uint8)RX_data;
				Bcm_Frame_St.No_Of_Bytes = ((Bcm_Frame_St.No_Of_Bytes)<<8);
				g_FlagISR++;
			}
			else if(SENDING_ADDRESS_2 == g_FlagISR){
				/*
				 * the second 8 bits of address
				 */
				Bcm_Frame_St.No_Of_Bytes = ((uint16)RX_data | (uint16)Bcm_Frame_St.No_Of_Bytes);
				g_FlagISR++;
			}
		}
	}
}

/*
 * Init the BCM with its configurations
 */
BCM_Enum_Error BCM_Init(Bcm_Confg* Ptr){
	/*
	 * set call back for tx and rx
	 */
	Set_Call_Back_RX(g_callBackFunction_RX);
	Set_Call_Back_TX(g_callBackFunction_TX);

	uint8 status = SUCCESS;//return of the init function
	switch (Ptr->id){
	/*
	 * Choosing the protocol
	 */
	case UART:
		status = UART_init(Confg_Uart_Ptr);
		break;
	case SPI:
		break;
	case I2C:
		break;
	default:
		status = FAILED;
		break;
	}
	return status;
}

/*
 * send a new byte
 */
BCM_Enum_Error BCM_Send(uint16 Num_Of_bytes,uint8*Adderss){
	/*
	 * return of function
	 */
	uint8 status = SUCCESS;
	if(BCM_State == OUT_OF_SERVICE){
		Bcm_Frame_St.BCM_id = BCM_ID;
		Bcm_Frame_St.Buffer = Adderss;
		Bcm_Frame_St.No_Of_Bytes = Num_Of_bytes;
		Bcm_Frame_St.Check_Sum = IDLE;
		BCM_State = LOCK;
	}
	return status;
}

/*
 * dispatcher of transmitter
 */
BCM_Enum_Error BCM_Dispatcher_TX(void){
	/*
	 * return status
	 */
	uint8 status = SUCCESS;
	static uint32 counts = NOT_COMPLETED;
	static uint32 index_tx = IDLE;
	if(LOCK == BCM_State){
		/*
		 * check that user send data "the buffer is unlocked"
		 */
		if(counts != g_Counter){
			/*
			 * guarding the dispatcher till an ISR
			 */
			counts = g_Counter;
			if(IDLE == g_Counter){ //IDLE state
				status = UART_sendByte(Bcm_Frame_St.BCM_id);
			}
			else if(SENDING_ADDRESS == g_Counter){ //Sending byte state
				status = UART_sendByte(((uint16)Bcm_Frame_St.No_Of_Bytes)>>8);
			}
			else if(SENDING_ADDRESS_2 == g_Counter){ //Sending byte state
				status = UART_sendByte((uint8)Bcm_Frame_St.No_Of_Bytes);
			}
			else if(SENDING_BYTE <= g_Counter){ //Sending Complete state
				if(index_tx != Bcm_Frame_St.No_Of_Bytes){
					Bcm_Frame_St.Check_Sum ^= Bcm_Frame_St.Buffer[index_tx];
					status = UART_sendByte(Bcm_Frame_St.Buffer[index_tx]);
					index_tx++;
				}
				else{ //Frame is completed
					BCM_State = UNLOCK;
					g_Counter = LOCK;
					status = UART_sendByte(Bcm_Frame_St.Check_Sum);
					if(g_consumer_callBackFun != NULL){
						g_consumer_callBackFun();
					}
				}
			}
		}
	}
	return status;
}

/*
 * unlock the buffer for receiving
 */
void BCM_UnlockRX(void){
	BCM_State = UNLOCK;
}

void setup_Buffer(uint8 *l_bufferPtr){
	Bcm_Frame_St.Check_Sum = IDLE;
	Bcm_Frame_St.BCM_id = BCM_ID;
	Bcm_Frame_St.Buffer = l_bufferPtr;
}

/*
 * dispatcher function for receiver
 */
BCM_Enum_Error BCM_Dispatcher_RX(void){
	/*
	 * return status
	 */
	uint8 status= SUCCESS;
	if(UNLOCK == BCM_State){
		/*
		 * buffer is unlocked
		 */
		if(COMPLETED == Frame){ //Frame completed state
			if(index_rx >= (Bcm_Frame_St.No_Of_Bytes)){
				if(Bcm_Frame_St.Check_Sum == checksum){
					PORTA |=0x01;
					ID_flag = NOT_MATCHED;
					g_FlagISR = IDLE;
					BCM_State = LOCK;
					if(g_consumer_callBackFun != NULL){
						g_consumer_callBackFun();
					}
				}
				else{
					status = FAILED;//check sum is incorrect
				}
			}
			else{
			}
		}
	}
	return status;
}

/*
 * Deinit the BCM by Deinitializing the used protocols
 */
void BCM_Deinit(Bcm_Confg* Ptr){
	switch (Ptr->id){
		/*
		 * Choosing the protocol
		 */
		case UART:
			UART_Deinit();
			break;
		case SPI:
			break;
		case I2C:
			break;
		default:
			break;
		}
}
