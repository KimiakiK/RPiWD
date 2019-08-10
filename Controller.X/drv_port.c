/* 
 * File:   drv_port.c
 */

#include <xc.h>

#include "drv_port.h"

void drv_port_Init(void)
{
    PORTA  = 0b00000000;    // 出力初期化
    PORTB  = 0b00000000;
    WPUA   = 0b00100000;    // Pull-up選択
    WPUB   = 0b11111011;    // 0:Off 1:On
    OPTION_REGbits.nWPUEN=0;// Pull-up有効・無効 0:有効 1:無効
    ANSELA = 0b00001111;    // ADC使用端子選択
    ANSELB = 0b00000000;    // 0:Digital 1:Analog
    TRISA  = 0b00101111;    // 入出力選択
    TRISB  = 0b11111011;    // 0:Output 1:Input
}
