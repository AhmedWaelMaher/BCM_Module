/*
 * BCM_Cong.h
 *
 * Created: 10/19/2019 11:45:45 PM
 *  Author: Ashry
 */ 
#include "std_types.h"

#ifndef BCM_CONG_H_
#define BCM_CONG_H_
typedef struct {
	uint8 id
	}Bcm_Confg;
	
	typedef struct{
	uint8 Bcm_id;
	uint16 No_Of_Bytes;
	uint32 *Buffer;
	uint8 Check_Sum;
		}Bcm_Frame;
	
	#define UART 1
	#define SPI  2
	#define I2C  3
	#define  BCM_ID 99






#endif /* BCM_CONG_H_ */