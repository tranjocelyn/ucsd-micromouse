/*
 * pid.c
 *
 *  Created on: Apr 19, 2017
 *      Author: nguyenbui
 */

/*
 * All the constant scale to 1000
 * int LKP= 1100;
 * int LKI= 17;
 * int LKD= 28;
 * int RKP= 1100;
 * int RKI= 17;
 * int RKD= 28;
 */
#define LKP 1100
#define LKI 17
#define LKD 28
#define RKP 1100
#define RKI 17
#define RKD 28


#define SS_LKP 15000
#define SS_LKI 160
#define SS_LKD 80
#define SS_RKP 15000
#define SS_RKI 160
#define SS_RKD 80


volatile int left_old_err, left_intergral;
volatile int right_old_err, right_intergral;

int pid_left_motor(int err){
	int derivative = (err - left_old_err);
	left_intergral = left_intergral + err ;
	int lpwm = (LKP*err + LKI*left_intergral + LKD*derivative);
	left_old_err = err; // Store the old error
	return lpwm = lpwm/1000; // Scale down cz we need very small change of KI
}


int pid_right_motor(int err){
	int derivative = (err - right_old_err);
	right_intergral = right_intergral + err ;
	int rpwm = (RKP*err + RKI*right_intergral + RKD*derivative);
	right_old_err = err; // Store the old error
	return rpwm = rpwm/1000; // Scale down cz we need very small change of KI
}


int sensor_pid_left_motor(int err){
	int derivative = (err - left_old_err);
	left_intergral = left_intergral + err ;
	int lpwm = (SS_LKP*err + SS_LKI*left_intergral + SS_LKD*derivative);
	left_old_err = err; // Store the old error
	return lpwm = lpwm/1000; // Scale down cz we need very small change of KI
}


int sensor_pid_right_motor(int err){
	int derivative = (err - right_old_err);
	right_intergral = right_intergral + err ;
	int rpwm = (SS_RKP*err + SS_RKI*right_intergral + SS_RKD*derivative);
	right_old_err = err; // Store the old error
	return rpwm = rpwm/1000; // Scale down cz we need very small change of KI
}


void pid_reset(void){
	left_old_err = 0;
	left_intergral = 0;

	right_old_err = 0;
	right_intergral = 0;
}
