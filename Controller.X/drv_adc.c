/* 
 * File:   drv_adc.c
 */

#include <xc.h>

#include "drv_adc.h"
#define _XTAL_FREQ 16000000     // 16MHz
void drv_adc_Init(void)
{
    ADCON1 = 0b01010000;    // 変換結果左寄せ、Fosc/16、Vref-=Vss、Vref+=Vdd
    ADCON0 = 0b00000001;    // ADC is enabled
}

unsigned char drv_adc_GetAdcValue(unsigned char ad_ch)
{
    // AD変換チャネル設定
    ADCON0bits.CHS = ad_ch;
    // AD変換結果がブレるので2回連続で変換する
    // AD変換開始
    ADCON0bits.GO_nDONE = 1;
    // AD変換完了待ち
    while (ADCON0bits.GO_nDONE == 1) {}
    // AD変換開始
    ADCON0bits.GO_nDONE = 1;
    // AD変換完了待ち
    while (ADCON0bits.GO_nDONE == 1) {}
    // 変換結果の上位8bitを返す
    return ADRESH;
}
