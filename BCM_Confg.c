/******************************************************************************
 *
 * Module: BCM
 *
 * File Name: BCM_Confg.c
 *
 * Description: Source file for the BCM configurations driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "BCM_Confg.h"
#include "BCM.h"

/*
 * using UART for BCM
 */
Bcm_Confg Bcm_Confg_St={UART};
Bcm_Confg  *Bcm_Confg_St_Ptr=& Bcm_Confg_St;
Bcm_Frame Bcm_Frame_St;
