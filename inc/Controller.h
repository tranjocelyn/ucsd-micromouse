/*
 * Controller.h
 *
 *  Created on: Apr 17, 2017
 *      Author: nguyenbui
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define FL_THRESHOLD 200
#define FR_THRESHOLD 210
#define DL_THRESHOLD 250
#define DR_THRESHOLD 190
#define PID_ENABLE 1
#define PID_DISABLE 0

void Controller_hardware_init(void );
void Controller_run(int left_distance, int right_distance, int left_speed, int right_speed);
void Controller_frontwall_corection(void );
void Controller_button1_interrupt_handler(void);
void Controller_button2_interrupt_handler(void);
int Controller_checkwall(void);
void Controller_readMazeFlash(void);
void Controller_writeFlash(void);
int Controller_mode_select();
#endif /* CONTROLLER_H_ */
