/*
 * Driver.c
 *
 *  Created on: Apr 19, 2017
 *      Author: nguyenbui
 */

#include "Controller.h"
#include "global.h"
#define LEFT_TURN_OFFSET 120 // calibrated on 05/19
#define RIGHT_TURN_OFFSET 130 // calibrated on 05/19


int left_cnt, right_cnt;
int left_spd, right_spd;
int radius;// = distance;

void Driver_turn_left(int distance, int angle, int speed){
	// Declaration of local variables
	int left_cnt, right_cnt;
	int left_spd, right_spd;
	int radius = distance;



	/* Check if user require to terminate current function */
	if (FUNC_TERMINATED){
		return;
	}


	// Rotate at the same position
	if (distance < 0){
		Controller_run(0,0,0,0);
		return;
	}

	// Go straight untill the turn position if distance is bigger than 1 tile
	if (distance > 90){
		Controller_run((distance-90)*L_100MM2COUNT_RATIO, (distance-90)*R_100MM2COUNT_RATIO, speed, speed);
		radius = 90;
	} else {
		radius = distance;
	}

	// Calculate left distance and right distance in counts
	left_cnt = (radius*2 - MOUSE_WIDTH)*M_PI/E_4*L_100MM2COUNT_RATIO*angle/360 + LEFT_TURN_OFFSET; // E_4+E_2 = E_6
	right_cnt = (radius*2 + MOUSE_WIDTH)*M_PI/E_4*R_100MM2COUNT_RATIO*angle/360 + LEFT_TURN_OFFSET; // E_4+E_2 = E_6

	// Make sure that left and right distance (coutns)is always POSITIVE
	if (left_cnt < 0) left_cnt = -left_cnt;
	if (right_cnt < 0) right_cnt = -right_cnt;

	// If Distance passed is 0, rotate the mice
	if (radius != 0){
		left_spd = (2*radius - MOUSE_WIDTH)*speed/2/radius;
		right_spd = (2*radius + MOUSE_WIDTH)*speed/2/radius;
	} else {
		left_spd = -speed;
		right_spd = speed;
	}

	// Cal propriate Controller method
	Controller_run(left_cnt, right_cnt, left_spd, right_spd);

}

void Driver_turn_right(int distance, int angle, int speed) {
// Declaration of local variables
	int left_cnt, right_cnt;
	int left_spd, right_spd;
	int radius = distance;

	/* Check if user require to terminate current function */
	if (FUNC_TERMINATED){
		return;
	}

	// Rotate at the same position
	if (distance < 0){
		Controller_run(0,0,0,0);
		return;
	}

	// Go straight untill the turn position if distance is bigger than 1 tile
	if (distance > 90){
		Controller_run((distance-90)*L_100MM2COUNT_RATIO, (distance-90)*R_100MM2COUNT_RATIO, speed, speed);
		radius = 90;
	} else {
		radius = distance;
	}



	left_cnt = (radius*2 + MOUSE_WIDTH)*M_PI/E_4*L_100MM2COUNT_RATIO*angle/360 + RIGHT_TURN_OFFSET; // E_4+E_2 = E_6
	right_cnt = (radius*2 - MOUSE_WIDTH)*M_PI/E_4*R_100MM2COUNT_RATIO*angle/360 + RIGHT_TURN_OFFSET; // E_4+E_2 = E_6

	// Make sure that left and right distance (coutns)is always POSITIVE
	if (left_cnt < 0) left_cnt = -left_cnt;
	if (right_cnt < 0) right_cnt = -right_cnt;


	if (radius != 0){
		left_spd = (2*radius + MOUSE_WIDTH)*speed/2/radius;
		right_spd = (2*radius - MOUSE_WIDTH)*speed/2/radius;
	} else {
		left_spd = speed;
		right_spd = -speed;
	}

	// COntroller
	Controller_run(left_cnt, right_cnt, left_spd, right_spd);

}


/* return whether successful or not */
void Driver_go_straight(int distance, int speed) {
	/* Check if user require to terminate current function */
	if (FUNC_TERMINATED){
		return;
	}

	Controller_run(distance*L_100MM2COUNT_RATIO, distance*R_100MM2COUNT_RATIO, speed, speed);
}

int Driver_checkwalls(){
	return Controller_checkwall();
}
