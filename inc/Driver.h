/*
 * Driver.h
 *
 *  Created on: Apr 19, 2017
 *      Author: nguyenbui
 */

#ifndef DRIVER_H_
#define DRIVER_H_

#include "Controller.h"

void Driver_turn_left(int distance, int angle, int speed);

void Driver_turn_right(int distance, int angle, int speed);

void Driver_go_straight(int distance, int speed);

int Driver_checkwalls(void);

#endif /* DRIVER_H_ */
