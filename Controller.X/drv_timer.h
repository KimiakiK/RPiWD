/* 
 * File:   drv_timer.h
 */

#ifndef DRV_TIMER_H
#define	DRV_TIMER_H


#define DRV_TIMER_TMR0_INIT (256 - 250)   // 2msでオーバーフローさせる 8us * 250 = 2ms

extern void drv_timer_Init(void);
extern void drv_timer_Start(void);


#endif	/* DRV_TIMER_H */

