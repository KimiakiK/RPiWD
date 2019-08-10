/* 
 * File:   main.c
 */

#include <xc.h>

#include "config.h"

#include "drv_port.h"
#include "drv_adc.h"
#include "drv_uart.h"
#include "drv_timer.h"
#include "in_joystick.h"
#include "in_switch.h"
#include "out_com.h"
#include "app_data.h"

#define _XTAL_FREQ 16000000     // 16MHz

#define INTRRUPT_OFF    0
#define INTRRUPT_ON     1
static unsigned char timer0_intrrupt_state;

#define COUNTER_INIT         0
#define COUNTER_INCREMNET    2       // 2msインクリメント
#define COUNTER_TARGET       20      // 20msを目的
static unsigned char period_counter;

void main(void) {
    unsigned char c = 0;
    // 初期化
    OSCCON = 0b01111010;    //16MHz内部クロック
    
    drv_port_Init();
    drv_adc_Init();
    drv_uart_Init();
    drv_timer_Init();
    in_joystick_Init();
    in_switch_Init();
    out_com_Init();
    app_data_Init();
    
    // Timer0 Start
    drv_timer_Start();
    timer0_intrrupt_state = INTRRUPT_ON;
    period_counter = COUNTER_INIT;
    
    // 割り込み許可
    INTCONbits.GIE = 1;     // Enables all active interrupts
    
    // メインループ
    while (1) {
        // Timer0のオーバーフロー確認
        if (timer0_intrrupt_state == INTRRUPT_ON) {
            timer0_intrrupt_state = INTRRUPT_OFF;      // Clear
            
            // 2ms周期処理呼び出し
            in_joystick_Main();
            in_switch_Main();
            
            // 20ms周期作成
            period_counter += COUNTER_INCREMNET;
            if (period_counter >= COUNTER_TARGET) {
                period_counter = COUNTER_INIT;
                
                // 20ms周期処理呼び出し
                app_data_Main();
                out_com_Main();
            }
        }
    }
}


void __interrupt() intr(void)
{
    INTCONbits.TMR0IF = 0;      // Clear Timer0 Overflow Interrupt Flag
    TMR0 = DRV_TIMER_TMR0_INIT; // Initialize Timer0 Counter
    
    timer0_intrrupt_state = INTRRUPT_ON;
}
