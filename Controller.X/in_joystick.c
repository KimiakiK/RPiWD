/* 
 * File:   in_joystick.c
 */

#include <xc.h>

#include "in_joystick.h"

#include "drv_port.h"
#include "drv_adc.h"

static unsigned char joystickPos[IN_JOYSTICK_NUM];

#define RING_BUFFER_NUM     5
static unsigned char ringBuffer[IN_JOYSTICK_NUM][RING_BUFFER_NUM];
static unsigned char ringBufferIndex[IN_JOYSTICK_NUM];

static const unsigned char joystickTable[IN_JOYSTICK_NUM] =
{
    DRV_PORT_JOYSTICK_1P_H,
    DRV_PORT_JOYSTICK_1P_V,
    DRV_PORT_JOYSTICK_2P_H,
    DRV_PORT_JOYSTICK_2P_V
};

static const unsigned char joystickPosMaxTable[IN_JOYSTICK_NUM] =
{
    DRV_PORT_JOYSTICK_1P_H_MAX,
    DRV_PORT_JOYSTICK_1P_V_MAX,
    DRV_PORT_JOYSTICK_2P_H_MAX,
    DRV_PORT_JOYSTICK_2P_V_MAX
};

static const unsigned char joystickPosMinTable[IN_JOYSTICK_NUM] =
{
    DRV_PORT_JOYSTICK_1P_H_MIN,
    DRV_PORT_JOYSTICK_1P_V_MIN,
    DRV_PORT_JOYSTICK_2P_H_MIN,
    DRV_PORT_JOYSTICK_2P_V_MIN
};

static void in_joystick_addRingBuffer(unsigned char joystick_index, unsigned char value);
static void in_joystick_calcAverage(unsigned char joystick_index);
static void in_joystick_convertPos(unsigned char joystick_index);

void in_joystick_Init(void)
{
    unsigned char joystick_index;
    unsigned char buffer_index;
    
    for (joystick_index=0; joystick_index<IN_JOYSTICK_NUM; joystick_index++) {
        joystickPos[joystick_index] = 0;
        ringBufferIndex[joystick_index] = 0;
        for (buffer_index=0; buffer_index<RING_BUFFER_NUM; buffer_index++) {
            ringBuffer[joystick_index][buffer_index] = 0;
        }
    }
}

void in_joystick_Main(void)
{
    unsigned char joystick_index;
    unsigned char ad_value;
    
    for (joystick_index=0; joystick_index<IN_JOYSTICK_NUM; joystick_index++) {
        // AD変換結果取得
        ad_value = drv_adc_GetAdcValue(joystickTable[joystick_index]);
        // リングバッファに格納
        in_joystick_addRingBuffer(joystick_index, ad_value);
        // 平均を算出
        in_joystick_calcAverage(joystick_index);
        // 位置情報に変換
        in_joystick_convertPos(joystick_index);
    }
}

unsigned char in_joystick_GetJoystickPos(unsigned char joystick_index)
{
    return joystickPos[joystick_index];
}

static void in_joystick_addRingBuffer(unsigned char joystick_index, unsigned char value)
{
    // リングバッファに格納
    ringBuffer[joystick_index][ringBufferIndex[joystick_index]] = value;
    // リングバッファの格納先を移動
    ringBufferIndex[joystick_index] ++;
    if (ringBufferIndex[joystick_index] >= RING_BUFFER_NUM) {
        ringBufferIndex[joystick_index] = 0;
    }
}

static void in_joystick_calcAverage(unsigned char joystick_index)
{
    unsigned char average;
    unsigned char buffer_index;
    
    average = 0;
    
    // 平均を計算
    for (buffer_index=0; buffer_index<RING_BUFFER_NUM; buffer_index++) {
        average = average >> 1;
        average += ringBuffer[joystick_index][buffer_index] >> 1;
    }
    
    // 計算結果を格納
    joystickPos[joystick_index] = average;
}

static void in_joystick_convertPos(unsigned char joystick_index)
{
    unsigned char pos;
    unsigned char level;
    
    pos = 0;
    
    if (joystickPos[joystick_index] > 127) {
        level = joystickPos[joystick_index] - 128;
        pos |= joystickPosMaxTable[joystick_index] << IN_JOYSTICK_POS_BIT_SIZE;
    } else {
        level = 127 - joystickPos[joystick_index];
        pos |= joystickPosMinTable[joystick_index] << IN_JOYSTICK_POS_BIT_SIZE;
    }
    
    if (level >= IN_JOYSTICK_POS_MAX_THRESHOLD) {
        pos |= IN_JOYSTICK_POS_MAX;
    } else if (level >= IN_JOYSTICK_POS_MID_THRESHOLD) {
        pos |= IN_JOYSTICK_POS_MID;
    } else if (level >= IN_JOYSTICK_POS_MIN_THRESHOLD) {
        pos |= IN_JOYSTICK_POS_MIN;
    }
    
    joystickPos[joystick_index] = pos;
}
