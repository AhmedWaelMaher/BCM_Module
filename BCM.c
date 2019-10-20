/*
 * BCM.c
 *
 * Created: 10/19/2019 11:44:18 PM
 *  Author: Ashry
 */ 
#include "BCM.h"
uint32 g_Counter=0;
uint8 g_Stat= UNLOCK;
 void g_function(){
	g_Counter++;
}
void Set_Call_Back(g_function);

uint8 Bcm_Init(Bcm_Confg* Ptr){
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
	
	uint8 Bcm_Send(uint16 Num_Of_bytes,uint32*Adderss){
		uint8 status=OK;
		if(g_Stat==UNLOCK){
		Bcm_Frame_St.Bcm_id=BCM_ID;
		Bcm_Frame_St.Buffer=Adderss;
		Bcm_Frame_St.No_Of_Bytes=Num_Of_bytes;
		Bcm_Frame_St.Check_Sum=0;
		status=UART_sendByte(Bcm_Frame_St.Bcm_id);
		g_Stat=LOCK;
		}
		return status;
		
	}
	
uint8 Bcm_Dispatcher(void){
	uint8 status=OK;
if(g_Stat==LOCK){	
if(g_Counter==1){
status = UART_sendByte(Bcm_Frame_St.No_Of_Bytes);
}
else if(g_Counter==2){
status = UART_sendByte(Bcm_Frame_St.No_Of_Bytes>>8);
}
else if((g_Counter>2)&&((g_Counter-2)<Bcm_Frame_St.No_Of_Bytes)){
status = UART_sendByte(Bcm_Frame_St.Buffer[g_Counter-2]);
Bcm_Frame_St.Check_Sum += Bcm_Frame_St.Buffer[g_Counter-2];
}
else if ((g_Counter-2)==Bcm_Frame_St.No_Of_Bytes){
status=UART_sendByte(Bcm_Frame_St.Check_Sum);
g_Stat=UNLOCK;
}
}

return status;
}




	