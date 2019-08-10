/* 
 * File:   out_com.c
 */

#include <xc.h>

#include "out_com.h"

#include "drv_uart.h"

static unsigned char outData[OUT_COM_DATA_LENGTH];

void out_com_Init(void)
{
    unsigned char data_index;
    
    for (data_index=0; data_index<OUT_COM_DATA_LENGTH; data_index++) {
        outData[data_index] = 0;
    }
}

void out_com_Main(void)
{
    unsigned char data_index;
    
    for (data_index=0; data_index<OUT_COM_DATA_LENGTH; data_index++) {
        // データを送信
        drv_uart_SyncTransmit(outData[data_index]);
    }
}

void out_com_SetData(unsigned char* data)
{
    unsigned char data_index;
    
    for (data_index=0; data_index<OUT_COM_DATA_LENGTH; data_index++) {
        // データをコピー
        outData[data_index] = data[data_index];
    }
}
