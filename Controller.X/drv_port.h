/* 
 * File:   drv_port.h
 */

#ifndef DRV_PORT_H
#define	DRV_PORT_H


/*
 * ピンアサイン
 * Pin  Func  I/O  Pull-up  接続先
 * 1    AN2   I    -        JoyStick
 * 2    AN3   I    -        JoyStick
 * 3    RA4   O    -        未使用
 * 4    RA5   I    On       Switch
 * 5    Vss   -    -        GND
 * 6    RB0   I    On       Switch
 * 7    RB1   I    On       Switch
 * 8    Tx    O    Off      Raspberry pi
 * 9    RB3   I    On       Switch
 * 10   RB4   I    On       Switch
 * 11   RB5   I    On       Switch
 * 12   RB6   I    On       Switch
 * 13   RB7   I    On       Switch
 * 14   Vdd   -    -        V3.3
 * 15   RA6   O    -        未使用
 * 16   RA7   O    -        未使用
 * 17   AN0   I    -        JoyStick
 * 18   AN1   I    -        JoyStick
 */

// スイッチ入力割り当て
#define DRV_PORT_SWITCH_1P_A    RB7
#define DRV_PORT_SWITCH_1P_B    RB4
#define DRV_PORT_SWITCH_1P_C    RB5
#define DRV_PORT_SWITCH_1P_D    RB6
#define DRV_PORT_SWITCH_2P_A    RB3
#define DRV_PORT_SWITCH_2P_B    RA5
#define DRV_PORT_SWITCH_2P_C    RB0
#define DRV_PORT_SWITCH_2P_D    RB1

// ジョイスティック入力割り当て
#define DRV_PORT_JOYSTICK_1P_H  1   // AN0
#define DRV_PORT_JOYSTICK_1P_V  0   // AN1
#define DRV_PORT_JOYSTICK_2P_H  3   // AN2
#define DRV_PORT_JOYSTICK_2P_V  2   // AN3

// ジョイスティック方向定義（送信データに影響）
#define DRV_PORT_JOYSTICK_UP    1
#define DRV_PORT_JOYSTICK_DOWN  0
#define DRV_PORT_JOYSTICK_LEFT  1
#define DRV_PORT_JOYSTICK_RIGHT 0

// ジョイスティック方向選択
#define DRV_PORT_JOYSTICK_1P_H_MAX  DRV_PORT_JOYSTICK_UP
#define DRV_PORT_JOYSTICK_1P_H_MIN  DRV_PORT_JOYSTICK_DOWN
#define DRV_PORT_JOYSTICK_1P_V_MAX  DRV_PORT_JOYSTICK_RIGHT
#define DRV_PORT_JOYSTICK_1P_V_MIN  DRV_PORT_JOYSTICK_LEFT
#define DRV_PORT_JOYSTICK_2P_H_MAX  DRV_PORT_JOYSTICK_UP
#define DRV_PORT_JOYSTICK_2P_H_MIN  DRV_PORT_JOYSTICK_DOWN
#define DRV_PORT_JOYSTICK_2P_V_MAX  DRV_PORT_JOYSTICK_RIGHT
#define DRV_PORT_JOYSTICK_2P_V_MIN  DRV_PORT_JOYSTICK_LEFT

extern void drv_port_Init(void);


#endif	/* DRV_PORT_H */

