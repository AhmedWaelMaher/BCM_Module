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

#define LOCK 0
#define UNLOCK 1

#define IDLE 0
#define SENDING_ADDRESS 1
#define SENDING_ADDRESS_2  2
#define SENDING_BYTE 3
#define FRAME_COMPLETE (g_Counter-2)==Bcm_Frame_St.No_Of_Bytes
#define BUSY 1
#define NOT_BUSY 0
#define COMPLETED 0
#define NOT_COMPLETED 1
#define OUT_OF_SERVICE -1

extern Bcm_Confg Bcm_Confg_St;
extern Bcm_Confg  *Bcm_Confg_St_Ptr;
extern Bcm_Frame Bcm_Frame_St;

extern void Set_Call_Back(void(*ptr)(void));
extern uint8 BCM_Init(Bcm_Confg* Ptr);
extern void g_callBackFunction_TX(void);
extern void g_callBackFunction_RX(void);
extern uint8 BCM_Dispatcher_TX(void);
extern uint8 BCM_Dispatcher_RX(void);
uint8 BCM_Send(uint16 Num_Of_bytes,uint8*Adderss);
extern void setup_Buffer(uint8 *l_bufferPtr);
extern void consumer_setCallBack(void(*ptr)(void));
extern void BCM_UnlockRX(void);


#endif /* BCM_H_ */
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

#define LOCK 0
#define UNLOCK 1

#define IDLE 0
#define SENDING_ADDRESS 1
#define SENDING_ADDRESS_2  2
#define SENDING_BYTE 3
#define FRAME_COMPLETE (g_Counter-2)==Bcm_Frame_St.No_Of_Bytes
#define BUSY 1
#define NOT_BUSY 0
#define COMPLETED 0

extern Bcm_Confg Bcm_Confg_St;
extern Bcm_Confg  *Bcm_Confg_St_Ptr;
extern Bcm_Frame Bcm_Frame_St;

extern void Set_Call_Back(void(*ptr)(void));
extern uint8 BCM_Init(Bcm_Confg* Ptr);
extern void g_callBackFunction_TX(void);
extern void g_callBackFunction_RX(void);
extern uint8 BCM_Dispatcher_TX(void);
extern uint8 BCM_Dispatcher_RX(void);
uint8 BCM_Send(uint16 Num_Of_bytes,uint8*Adderss);
extern void setup_Buffer(uint8 *l_bufferPtr);
extern void consumer_setCallBack(void(*ptr)(void));
extern void BCM_UnlockRX(void);


#endif /* BCM_H_ */
