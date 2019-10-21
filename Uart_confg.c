/******************************************************************************
 *
 * Module: UART
 *
 * File Name: Uarrt_confg.c
 *
 * Description: Source file for the UART configurations driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "Uart_confg.h"

/*
 * uint8 USART_mode;
	uint8 Rx_En;
	uint8 Tx_En;
	uint8 data_size;
	uint8 parity;
	uint8 stop_pattern;
	uint8 tarnsmission_speed;
	uint32 baud_rate;
	uint8 Event_Handling;
	uint8 clk_edge;
	uint8 IS_init;
 */
UART_cnfg_t Confg_Uart = {ASYNCHRONOUS,ENABLE,ENABLE,EIGHT_BITS,DISABLED,ONE_BIT,NORMAL_SPEED,9600,INTERRUPT,NA,NOT_INITIALIZED};

UART_cnfg_t *Confg_Uart_Ptr = & Confg_Uart;
