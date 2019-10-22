/******************************************************************************
 *
 * Module: UART
 *
 * File Name: Uart.c
 *
 * Description: Source file for the UART driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "std_types.h"
#include "UART.h"

static volatile uint8 Tx_Flag=0;
volatile uint8 Rx_data=0;

PtrToFunction callBackFunc_tx;
PtrToFunction callBackFunc_rx;

void Set_Call_Back_TX(void (*ptr)(void)){
	callBackFunc_tx=ptr;
}

void Set_Call_Back_RX(void (*ptr)(void)){
	callBackFunc_rx=ptr;
}

void static tx(void){
	Tx_Flag =0;
}

uint8 UART_init(UART_cnfg_t * Confg_ptr ){
	uint8 status=OK;
	if (Confg_ptr->IS_init == NOT_INITIALIZED )
	{
		Confg_ptr->IS_init= INITIALIZED ;
		UCSRC|=(1<<URSEL);      // URSEL= 1 The URSEL must be one when writing the UCSRC
	}
	/* USART MODE */
	switch(Confg_ptr->USART_mode)
	{
	case SYNCHRONOUS  :
		UCSRC|=(1<<UMSEL);  // SYNCHRONOUS
		break;
	case ASYNCHRONOUS :
		UCSRC&=~(1<<UMSEL);  // ASYNCHRONOUS
		break;
	default:
		Confg_ptr->IS_init= NOT_INITIALIZED ;
		status=NOK;
		break;
	}
	/* COMMUNICATION MODEL */
	switch(Confg_ptr->Rx_En)
	{
	case ENABLE :
		UCSRB|=(1<<RXEN); // Receiver Enable
		break;
	case DISABLED :

		UCSRB&=~(1<<RXEN); // Receiver Disable
		break;
	}
	switch(Confg_ptr->Tx_En)
	{
	case ENABLE :
		UCSRB|=(1<<TXEN); // Transmit Enable
		break;
	case DISABLED :
		UCSRB&=~(1<<TXEN); // Transmit Disable
		break;
	}
	/*  DATA SIZE  */
	switch(Confg_ptr->data_size)
	{
	case FIVE_BITS :
		UCSRC &= ~((1<<UCSZ2) & (1<<UCSZ1) & (1<<UCSZ0) ); // UCSZ2=0 UCSZ1=0 UCSZ0=0
		break;
	case SIX_BITS :
		UCSRC &= ~((1<<UCSZ2) & (1<<UCSZ1)); // UCSZ2=0 UCSZ1=0
		UCSRC |=  (1<<UCSZ0) ; // UCSZ0=1
		break;
	case SEVEN_BITS :
		UCSRC &= ~((1<<UCSZ2) & (1<<UCSZ0)); // UCSZ2=0 UCSZ0=0
		UCSRC |=  (1<<UCSZ1) ; // UCSZ1=1
		break;
	case EIGHT_BITS :
		UCSRC &= ~ (1<<UCSZ2) ; // UCSZ2=0
		UCSRC |=  (1<<UCSZ1) | (1<<UCSZ0) ; // UCSZ1=1 UCSZ0=1
		break;
	case NINE_BITS :
		UCSRC |= ((1<<UCSZ2) | (1<<UCSZ1) | (1<<UCSZ0) ); //UCSZ2=1 UCSZ1=1 UCSZ0=1
		break;
	default:
		Confg_ptr->IS_init = NOT_INITIALIZED ;
		status=NOK;
		break;
	}
	/* PARITY */
	switch(Confg_ptr->parity)
	{
	case DISABLED :
		UCSRC &= ~( (1<<UPM1) & (1<<UPM0) );  // UPM1=O UPM0=O ODD DISABLED
		break;
	case ODD :
		UCSRC |=  (1<<UPM1) | (1<<UPM0) ; // UPM1=1 UPM0=1 ODD PARITY
		break;
	case EVEN :
		UCSRC &= (1<<UPM0); // UPM0=0 EVEN PARITY
		UCSRC |=  (1<<UPM1); // UPM1=1 EVEN PARITY
		break;
	default:
		Confg_ptr->IS_init= NOT_INITIALIZED ;
		status=NOK;
		break;
		return status ;
	}
	/*  STOP PATTERN  */
	switch(Confg_ptr->stop_pattern)
	{
	case ONE_BIT :
		UCSRC&=~(1<<USBS);	 // STOP BIT
		break;
	case TWO_BITS :
		UCSRC|=(1<<USBS);	 // 2 STOP
		break;
	default:
		Confg_ptr->IS_init= NOT_INITIALIZED ;
		status=NOK;
		break;
	}
	/* TRANSMISSION SPEED */
	switch(Confg_ptr->tarnsmission_speed)
	{
	case DOUBLE_SPEED :
		UCSRA|=(1<<U2X); // DOUBLE SPEED
		break;
	case NORMAL_SPEED :
	case NA :
		UCSRA&=~(1<<U2X); // NORMAL SPEED OR SYNCHRONOUS
		break;
	default:
		Confg_ptr->IS_init= NOT_INITIALIZED ;
		status=NOK;
		break;
	}
	/*  BAUD RATE  */
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	uint32 BAUD_PRESCALE = ((F_CPU / (Confg_ptr->baud_rate * 16UL)) - 1);
	UBRRH = BAUD_PRESCALE>>8;
	UBRRL = BAUD_PRESCALE;
	/* INTERRUPT */
	switch(Confg_ptr->Event_Handling)
	{
	case  POLLING :
		UCSRB&=~(1<<RXCIE);		// RX COMPLETE DISABLED
		UCSRB&=~(1<<TXCIE);		// TX COMPLETE DISABLED
		break;
	case INTERRUPT :
		UCSRB|=(1<<RXCIE);		// RX Enable
		UCSRB|=(1<<TXCIE);		// Tx Enable
		//UCSRB|=(1<<UDRIE);     // Empty reg enable
		break;
	default:
		Confg_ptr->IS_init= NOT_INITIALIZED ;
		status=NOK;
		break;
	}

	/*  CLK EDGE  */
	switch(Confg_ptr->clk_edge)
	{
	case RISING :
	case NA :
		UCSRC&=~(1<<UCPOL);  // RISING XCK Edge OR Asynchronous
		break;
	case FALLING :
		UCSRC|=(1<<UCPOL);  // FALLING XCK Edge
		break;
	default:
		Confg_ptr->IS_init= NOT_INITIALIZED ;
		status=NOK;
		break;
	}
	return status;
}

uint8 UART_sendByte(const uint8 data){
	uint8 status =OK;
	if ( (Confg_Uart_Ptr->IS_init==INITIALIZED) && (Confg_Uart_Ptr->Event_Handling== POLLING ) )
	{
		while (!(UCSRA & (1<<UDRE))){} // UDRE flag is set when the TX buffer (UDR) is empty
		UDR = data; ///Put the required data in the UDR register
	}
	else if ( (Confg_Uart_Ptr->IS_init==INITIALIZED) && (Confg_Uart_Ptr->Event_Handling== INTERRUPT ) ){
		if(Tx_Flag==0){
			Tx_Flag++;
			UDR=data;
		}
	}
	else{
		status = NOK;
	}
	return status;
}

uint8 UART_recieveByte(uint8 * data){
	uint8 status =OK;
	if ( (Confg_Uart_Ptr->IS_init==INITIALIZED) && (Confg_Uart_Ptr->Event_Handling== POLLING ) )
	{
		while (!(UCSRA & (1<<UDRE))){} // UDRE flag is set when the TX buffer (UDR) is empty
		*data=UDR; // Read the received data from  (UDR) ///Put the required data in the UDR register
	}
	else if ( (Confg_Uart_Ptr->IS_init==INITIALIZED) && (Confg_Uart_Ptr->Event_Handling== INTERRUPT ) ){
		*data=UDR;
	}
	else{
		status = NOK;
	}
	return status;
}

void __vector_13(void){
	if(callBackFunc_rx != NULL){
		callBackFunc_rx();
	}
}
void __vector_15(void){
	tx();
	if(callBackFunc_tx != NULL){
		callBackFunc_tx();
	}
}

void UART_Deinit(void){
	UCSRB&=~(1<<RXEN);
	UCSRB&=~(1<<TXEN);
}
