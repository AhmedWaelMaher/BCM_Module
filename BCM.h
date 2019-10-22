/******************************************************************************
 *
 * Module: BCM
 *
 * File Name: BCM.h
 *
 * Description: Header file for the BCM driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "BCM_Confg.h"
#include "Uart.h"

#ifndef BCM_H_
#define BCM_H_

/*
 * Different protocols
 */
#define UART 1
#define SPI  2
#define I2C  3

/*
 * Frame ID = #
 */
#define BCM_ID 35

typedef enum{
	FAILED,SUCCESS
}BCM_Enum_Error;

extern void Set_Call_Back(void(*ptr)(void));
extern uint8 BCM_Init(Bcm_Confg* Ptr);
extern uint8 BCM_Dispatcher_TX(void);
extern uint8 BCM_Dispatcher_RX(void);
extern uint8 BCM_Send(uint16 Num_Of_bytes,uint8*Adderss);
extern void setup_Buffer(uint8 *l_bufferPtr);
extern void consumer_setCallBack(void(*ptr)(void));
extern void BCM_UnlockRX(void);
extern void BCM_Deinit(Bcm_Confg* Ptr);

#endif /* BCM_H_ */
