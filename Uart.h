/******************************************************************************
 *
 * Module: UART
 *
 * File Name: Uart.h
 *
 * Description: Header file for the UART driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "Uart_confg.h"

#define  OK 1
#define  NOK 0

extern UART_cnfg_t Confg_Uart ;
extern UART_cnfg_t *Confg_Uart_Ptr ;
extern uint8 UART_init(UART_cnfg_t * Confg_ptr );
extern uint8 UART_sendByte(const uint8 data);
extern uint8 UART_recieveByte(uint8 * data);
typedef void(*PtrToFunction)(void); 
void Set_Call_Back(void (*ptr)(void));

void __vector_13(void) __attribute__((signal,used));
void __vector_14(void) __attribute__((signal,used));
void __vector_15(void) __attribute__((signal,used));


#endif /* UART_H_ */
