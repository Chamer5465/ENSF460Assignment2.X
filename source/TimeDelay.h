/* 
 * File:   TimeDelay.h
 * Author: Patricia Agdamag
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMEDELAY_H
#define	TIMEDELAY_H

#include <xc.h> // include processor files - each processor file is guarded.  

void delay_ms(uint16_t time_ms);

#endif

