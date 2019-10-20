/*
 * BCM.h
 *
 * Created: 10/19/2019 11:45:02 PM
 *  Author: Ashry
 */ 

#include "BCM_Cong.h"
#include "Uart.h"

#ifndef BCM_H_
#define BCM_H_
#define LOCK 0
#define  UNLOCK 1
extern Bcm_Confg Bcm_Confg_St;
extern Bcm_Confg  *Bcm_Confg_St_Ptr;
extern Bcm_Frame Bcm_Frame_St;
#endif /* BCM_H_ */