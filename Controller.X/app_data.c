/* 
 * File:   app_data.c
 */

#include <xc.h>

#include "app_data.h"

#include "in_joystick.h"
#include "in_switch.h"
#include "out_com.h"

#define DATA_1P_SWITCH      0
#define DATA_2P_SWITCH      1
#define DATA_1P_JOYSTICK    2
#define DATA_2P_JOYSTICK    3

static unsigned char outData[OUT_COM_DATA_LENGTH];

static void app_data_initData(void);
static void app_data_switchData(void);
static void app_data_joystickData(void);

void app_data_Init(void)
{
    app_data_initData();
}

void app_data_Main(void)
{
    // 送信データを初期化
    app_data_initData();
    // 送信データにスイッチ状態を格納
    app_data_switchData();
    // 送信データにジョイスティック位置を格納
    app_data_joystickData();
    // データ送信
    out_com_SetData(outData);
}

static void app_data_initData(void)
{
    unsigned char data_index;
    
    for (data_index=0; data_index<OUT_COM_DATA_LENGTH; data_index++) {
        // 各データの先頭に番号を付与
        outData[data_index] = data_index << 6;
    }
}

static void app_data_switchData(void)
{
    outData[DATA_1P_SWITCH] |= in_switch_GetSwtich(IN_SWITCH_1P_A) << 3;
    outData[DATA_1P_SWITCH] |= in_switch_GetSwtich(IN_SWITCH_1P_B) << 2;
    outData[DATA_1P_SWITCH] |= in_switch_GetSwtich(IN_SWITCH_1P_C) << 1;
    outData[DATA_1P_SWITCH] |= in_switch_GetSwtich(IN_SWITCH_1P_D) << 0;
    
    outData[DATA_2P_SWITCH] |= in_switch_GetSwtich(IN_SWITCH_2P_A) << 3;
    outData[DATA_2P_SWITCH] |= in_switch_GetSwtich(IN_SWITCH_2P_B) << 2;
    outData[DATA_2P_SWITCH] |= in_switch_GetSwtich(IN_SWITCH_2P_C) << 1;
    outData[DATA_2P_SWITCH] |= in_switch_GetSwtich(IN_SWITCH_2P_D) << 0;
}

static void app_data_joystickData(void)
{
    outData[DATA_1P_JOYSTICK] |= in_joystick_GetJoystickPos(IN_JOYSTICK_1P_H) << 3;
    outData[DATA_1P_JOYSTICK] |= in_joystick_GetJoystickPos(IN_JOYSTICK_1P_V) << 0;
    outData[DATA_2P_JOYSTICK] |= in_joystick_GetJoystickPos(IN_JOYSTICK_2P_H) << 3;
    outData[DATA_2P_JOYSTICK] |= in_joystick_GetJoystickPos(IN_JOYSTICK_2P_V) << 0;
}
