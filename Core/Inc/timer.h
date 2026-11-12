/*
 * timer.h
 *
 *  Created on: Oct 30, 2021
 *      Author: DELL
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

void setTimer0(int duration);
int getTimer0_flag(void);

void setTimer1(int duration);
int getTimer1_flag(void);

void timer_run(void);

#endif /* INC_TIMER_H_ */
