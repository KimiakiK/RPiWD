/* 
 * File:   drv_uart.c
 */

#include <xc.h>

#include "drv_uart.h"

void drv_uart_Init(void)
{
    TXSTA = 0b00100100;     // Async、Tx Enable、8bit、High-Speed
    RCSTA = 0b10000000;     // Serial Port Enable、Rx Disable
    APFCON1bits.TXCKSEL = 0;// TX pin 0:RB2 1:RB5
    SP1BRGL = 25;           // 16MHz / 38400bps/ 16 - 1 = 25
}

void drv_uart_SyncTransmit(unsigned char data)
{
    // 送信データバッファが空になるまで待機
    while (PIR1bits.TXIF == 0) {}
    // 送信データ格納
    TXREG = data;
}
