/******************************************************************************
 *
 * Module: BCM
 *
 * File Name: BCM_Confg.h
 *
 * Description: Header file for the BCM configurations driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "std_types.h"

#ifndef BCM_CONG_H_
#define BCM_CONG_H_

typedef struct {
	uint8 id;
}Bcm_Confg;

typedef struct{
	uint8 BCM_id;
	uint16 No_Of_Bytes;
	uint8 *Buffer;
	uint8 Check_Sum;
}Bcm_Frame;

#define UART 1
#define SPI  2
#define I2C  3
#define BCM_ID 35






#endif /* BCM_CONG_H_ */
