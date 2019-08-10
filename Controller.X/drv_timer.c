/* 
 * File:   drv_timer.c
 */

#include <xc.h>

#include "drv_timer.h"

void drv_timer_Init(void)
{
    OPTION_REGbits.TMR0CS = 0;  // Timer0 Clock Source 0:Internal 1:T0CKI pin
    OPTION_REGbits.PSA = 0;     // Prescaler 0:ON 1:OFF
    OPTION_REGbits.PS = 0b100;  // 1 / ((Fosc/4) / 32) = 8us
    
}

void drv_timer_Start(void)
{
    TMR0 = DRV_TIMER_TMR0_INIT; // Initialize Timer0 Counter
    
    INTCONbits.TMR0IF = 0;      // Clear Timer0 Overflow Interrupt Flag
    INTCONbits.TMR0IE = 1;      // Timer0 interrupt 0:Disable 1:Enable
    
}