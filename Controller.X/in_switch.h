/* 
 * File:   in_switch.h
 */

#ifndef IN_SWITCH_H
#define	IN_SWITCH_H


#define IN_SWITCH_1P_A      0
#define IN_SWITCH_1P_B      1
#define IN_SWITCH_1P_C      2
#define IN_SWITCH_1P_D      3
#define IN_SWITCH_2P_A      4
#define IN_SWITCH_2P_B      5
#define IN_SWITCH_2P_C      6
#define IN_SWITCH_2P_D      7

#define IN_SWITCH_NUM       8

#define IN_SWITCH_OFF       0
#define IN_SWITCH_ON        1

// スイッチ状態の遷移が確定するまでの回数
#define IN_SWTICH_TRANSITION    5

extern void in_switch_Init(void);
extern void in_switch_Main(void);
extern unsigned char in_switch_GetSwtich(unsigned char switch_index);

#endif	/* IN_SWITCH_H */

