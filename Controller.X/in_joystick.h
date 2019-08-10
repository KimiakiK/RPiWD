/* 
 * File:   in_joystick.h
 */

#ifndef IN_JOYSTICK_H
#define	IN_JOYSTICK_H

#define IN_JOYSTICK_1P_H    0
#define IN_JOYSTICK_1P_V    1
#define IN_JOYSTICK_2P_H    2
#define IN_JOYSTICK_2P_V    3

#define IN_JOYSTICK_NUM     4

// ジョイスティック入力強さ定義（送信データに影響）
#define IN_JOYSTICK_POS_MAX   3
#define IN_JOYSTICK_POS_MID   2
#define IN_JOYSTICK_POS_MIN   1
#define IN_JOYSTICK_POS_BIT_SIZE    2

// ジョイスティック入力強さの閾値
#define IN_JOYSTICK_POS_MAX_THRESHOLD   100 // この値以上をMAXとする
#define IN_JOYSTICK_POS_MID_THRESHOLD   70  // この値以上をMIDとする
#define IN_JOYSTICK_POS_MIN_THRESHOLD   40  // この値以上をMINとする

extern void in_joystick_Init(void);
extern void in_joystick_Main(void);
extern unsigned char in_joystick_GetJoystickPos(unsigned char joystick_index);

#endif	/* IN_JOYSTICK_H */

