/*
 * File:   IOs.c
 * Author: Chase MacKenzie, Patricia Agdamag, Aaron Montesines
 *
 * Created on October 1, 2025, 8:47 PM
 */


#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "clkChange.h"

extern uint8_t led1State;
extern uint8_t led2State;
extern uint8_t TMR1flag;

void IOinit(void) {
    
    /** This is usually where you would add run-once code
     * e.g., peripheral initialization. For the first labs
     * you might be fine just having it here. For more complex
     * projects, you might consider having one or more initialize() functions
     */
    
    AD1PCFG = 0xFFFF; /* keep this line as it sets I/O pins that can also be analog to be digital */
       
    // I/O Initializations
    TRISAbits.TRISA4 = 1;
    CNPU1bits.CN0PUE = 1;
    
    TRISBbits.TRISB7 = 1;
    CNPU2bits.CN23PUE = 1;
    
    TRISBbits.TRISB4 = 1;
    CNPU1bits.CN1PUE = 1;
    
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9 = 1;
    
    TRISAbits.TRISA6 = 0;
    LATAbits.LATA6 = 1;
    
    newClk(500);            // Sets clock to 500kHz
    
    // Timer 2 Config
    T2CONbits.T32 = 0;      //operate timer 2 as 16 bit timer
    T2CONbits.TCKPS = 1;    // Prescaler 1:8
    T2CONbits.TCS = 0;      // Use internal clock
    T2CONbits.TSIDL = 0;    //operate in idle mode.
    
    // Timer 2 interrupt Config
    IPC1bits.T2IP = 1;      // Set to lowest priority
    IFS0bits.T2IF = 0;  
    IEC0bits.T2IE = 1;      // Enable timer interrupt
    
    PR2 = 15625;            // Set count value
    TMR2 = 0;               // Reset timer to 0.
    
    T2CONbits.TON = 1;      //Start timer2 LED2 since it blinks continuously
   
};

void IOcheck(void) {
    
    // Checks if Button 1 and Button 2 are both pressed.
    if (PORTBbits.RB4 == 0 && PORTBbits.RB7 == 0) {
        LATBbits.LATB9 = 1;
        delay_ms(1);
        LATBbits.LATB9 = 0;
        delay_ms(1);

    }
    
    // Checks if Button 1 is pressed
    else if (PORTBbits.RB7 == 0) {
        LATBbits.LATB9 = 1;
        delay_ms(250);
        LATBbits.LATB9 = 0;
        delay_ms(250);

    }
    
    // Checks if Button 2 is pressed
    else if (PORTBbits.RB4 == 0) {
        LATBbits.LATB9 = 1;
        delay_ms(1000);
        LATBbits.LATB9 = 0;
        delay_ms(1000);

    }

    // Checks if Button 3 is pressed
    else if (PORTAbits.RA4 == 0) {
        LATBbits.LATB9 = 1;
        delay_ms(6000);
        LATBbits.LATB9 = 0;
        delay_ms(6000);
    }
    
    else {
        LATBbits.LATB9 = 0;
    }
    
};


// Timer 2 interrupt service routine
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0; // Clear timer 2 interrupt flag
    
    if (led2State) {
        LATAbits.LATA6 = 0; // Turn off LED2
        led2State = 0;      // Set State of LED2 as Off
    } else {
        LATAbits.LATA6 = 1; // Turn on LED2
        led2State = 1;      // Set State of LED2 as ON
    }
}; 

// You might it helpful to define the interrupt service routine for Timer 1 here
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void){
    //Don't forget to clear the timer 1 interrupt flag!
    IFS0bits.T1IF = 0; // Clearing timer1 flag
    T1CONbits.TON = 0; // stops timer until next button press
        TMR1flag = 1; // Sets flag to 1 to let delay_ms() know that interrupt finished.

};
