/*
 * pid.h
 *
 *  Created on: Apr 19, 2017
 *      Author: nguyenbui
 */

#ifndef PID_H_
#define PID_H_

int pid_left_motor(int err);
int pid_right_motor(int err);
int sensor_pid_left_motor(int err);
int sensor_pid_right_motor(int err);
void pid_reset(void);


#endif /* PID_H_ */
