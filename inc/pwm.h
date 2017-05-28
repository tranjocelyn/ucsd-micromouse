/*
 * pwm.h
 *
 *  Created on: Apr 17, 2017
 *      Author: nguyenbui
 */

#ifndef PWM_H_
#define PWM_H_

void PWM_Configuration(void);
void setLeftPwm(int32_t speed);
void setRightPwm(int32_t speed);

#endif /* PWM_H_ */
