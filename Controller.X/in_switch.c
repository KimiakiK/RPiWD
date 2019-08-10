/* 
 * File:   in_switch.c
 */

#include <xc.h>

#include "in_switch.h"

#include "drv_port.h"

static unsigned char switchState[IN_SWITCH_NUM];
static unsigned char switchLastInput[IN_SWITCH_NUM];

static unsigned char switchContinue[IN_SWITCH_NUM];

static unsigned char in_switch_getSwitchInput(unsigned char switch_index);

void in_switch_Init(void)
{
    unsigned char switch_index;
    
    for (switch_index=0; switch_index<IN_SWITCH_NUM; switch_index++) {
        switchState[switch_index] = IN_SWITCH_OFF;
        switchLastInput[switch_index] = IN_SWITCH_OFF;
        switchContinue[switch_index] = 0;
    }
}

void in_switch_Main(void)
{
    unsigned char switch_index;
    unsigned char switch_input;
    
    for (switch_index=0; switch_index<IN_SWITCH_NUM; switch_index++) {
        // 入力を取得
        switch_input = in_switch_getSwitchInput(switch_index);
        //スイッチ状態確定判定
        if (switch_input == switchLastInput[switch_index]) {
            // 前回と同じ入力
            if (switchContinue[switch_index] < IN_SWTICH_TRANSITION) {
                //  スイッチ状態確定待ち
                switchContinue[switch_index] ++;
            } else {
                // スイッチ状態確定
                switchState[switch_index] = switch_input;
            }
        } else {
            // 前回と異なる入力
            switchLastInput[switch_index] = switch_input;
            switchContinue[switch_index] = 0;
        }
    }
}

unsigned char in_switch_GetSwtich(unsigned char switch_index)
{
    return switchState[switch_index];
}

static unsigned char in_switch_getSwitchInput(unsigned char switch_index)
{
    unsigned char switch_input;
    
    // 端子入力取得
    switch (switch_index) {
        case IN_SWITCH_1P_A:
            switch_input = DRV_PORT_SWITCH_1P_A;
            break;
        case IN_SWITCH_1P_B:
            switch_input = DRV_PORT_SWITCH_1P_B;
            break;
        case IN_SWITCH_1P_C:
            switch_input = DRV_PORT_SWITCH_1P_C;
            break;
        case IN_SWITCH_1P_D:
            switch_input = DRV_PORT_SWITCH_1P_D;
            break;
        case IN_SWITCH_2P_A:
            switch_input = DRV_PORT_SWITCH_2P_A;
            break;
        case IN_SWITCH_2P_B:
            switch_input = DRV_PORT_SWITCH_2P_B;
            break;
        case IN_SWITCH_2P_C:
            switch_input = DRV_PORT_SWITCH_2P_C;
            break;
        case IN_SWITCH_2P_D:
            switch_input = DRV_PORT_SWITCH_2P_D;
            break;
        default:
            switch_input = 0;
            break;
    }
    
    // 入力レベルをスイッチ状態に変換
    if (switch_input == 0) {
        switch_input = IN_SWITCH_ON;
    } else {
        switch_input = IN_SWITCH_OFF;
    }
    
    return switch_input;
}
