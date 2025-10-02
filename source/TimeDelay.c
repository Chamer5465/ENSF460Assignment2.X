/*
 * File:   TimeDelay.c
 * Author: Chase MacKenzie, Patricia Agdamag, Aaron Montesines
 *
 * Created on October 1, 2025, 8:48 PM
 */


#include "xc.h"
#include "TimeDelay.h"
extern uint8_t TMR1flag;

void delay_ms(uint16_t time_ms) {
    
    // Step 1: Select an appropriate clock speed.
    //          Was set to 500kHz in IOinit();
    
    // Reset flag before starting.
    TMR1flag = 0;
    
    // Step 2: Configure T1CON register bits.
    T1CONbits.TCS = 0;
    T1CONbits.TSIDL = 0; //operate in idle mode.
    
    // Step 3: Clear TMR1
    TMR1 = 0;
    
    // Step 4: Configure Timer1 specific bits in interrupt registers IPC0, IEC0 
    //          IFS1
    IPC0bits.T1IP = 1;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;  // Enable timer interrupt
    
    // Step 5: Compute PR1 based on desired time delay.
    // Checks if prescaler needs to be increased depending on delay.
    
    if (time_ms < 5000) {
    T1CONbits.TCKPS = 0b01; // 1:8 prescaler
    // Fcy = 250kHz.
    PR1 = (uint16_t)((time_ms * 250000 / 8) / 1000); 
    } else {
        T1CONbits.TCKPS = 0b10; // 1:64 prescaler
        PR1 = (uint16_t)((time_ms * 250000 / 64) / 1000);
    }

    // Step 6: Start Timer 
    T1CONbits.TON = 1;

    // Step 7: Checks if interrupt finished, otherwise stay in idle mode.
    while (!TMR1flag) {
        Idle();
    }
    // Step 8: Upon Interrupt,if MCU is in idle state, exit idle state and its
    //          code jumps to Timer-2 interrupt routine (ISR). Stop timer here.
    IFS0bits.T1IF = 0;
    
};