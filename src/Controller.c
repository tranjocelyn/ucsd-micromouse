/*
 * Controller.c
 *
 *  Created on: Apr 17, 2017
 *      Author: nguyenbui
 */
#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"
#include "encoder.h"
#include "stdlib.h"
#include "adc.h"
#include "pid.h"
#include "sensor.h"
#include "button.h"
#include "global.h"
/*
 * This function is used to initialize all hardware needed.
 */
volatile int FUNC_TERMINATED = 0;

void Controller_hardware_init(void) {

	Systick_Configuration();

	LED_Configuration();
	ALL_LED_OFF;

	PWM_Configuration();
	setLeftPwm(0);
	setRightPwm(0);

	Encoder_Configration();

	ADC_Config();

	button_Configuration();

	delay_ms(1000);
}

/*
 * This fution is used to set the speed of both motor
 */
void Controller_run(int left_distance, int right_distance, int left_speed, int right_speed){

	// Declaration of all variables
	volatile int left_EN; // left encoder count
	volatile int right_EN; // right encoder count
	volatile int pid_err, temp=0;
	volatile int lpwm, rpwm;

	volatile int wall_err = 0;


	/* Check if user require to terminate current function */
	if (FUNC_TERMINATED){
		setLeftPwm(0);
		setRightPwm(0);
		return;
	}

	// Reset PID
	pid_reset();


	// Break --
	// WHen ever speed set to 0. break for 200ms
	if ((left_speed == 0) && (right_speed ==0)){
		int curr_time = Millis;
		while ( (Millis - curr_time) < 200){
			pid_err = - global_left_speed;
			lpwm = pid_left_motor(pid_err);
			pid_err = - global_right_speed;
			rpwm = pid_right_motor(pid_err);
			setLeftPwm(lpwm);
			setRightPwm(rpwm);
		}

		setLeftPwm(0);
		setRightPwm(0);

		return;
	}

	// Save the value in the Encoder counter reg
	left_EN = getLeftDistance();
	right_EN = getRightDistance();

	int crr_case = 0;
	// This line of code can cause trouble if the encoder rigister is overflow
	// Loop until mice finish the given distance
	// Distance is allway positive, this was calculated from caller (Driver)
	while ( (abs(getLeftDistance() - left_EN) < (left_distance))||
					(abs(getRightDistance() - right_EN) < (right_distance))){


		// Correct position base on wall sensor infomation
		// Only corret when going straight
		if (left_speed == right_speed){
			// read infomation
			read_side_sensors();

			// CASE 1: have both wall
			if ((LDSensor > DIAGNAL_LEFT_THRESHOLD) &&((RDSensor > DIAGNAL_RIGHT_THRESHOLD))){
				if (crr_case != 1){
					temp = 0;
					crr_case = 1;
				}
				if (temp > 200){
					if (LDSensor > (CENTER_TO_LEFT_WALL)){
						wall_err = (LDSensor - CENTER_TO_LEFT_WALL)/SENSOR_RATIO;
						// Check if error is valid for correction ( too far from wall)
						if (wall_err > (MAX_SENSOR_ERR)) wall_err = MAX_SENSOR_ERR;
						if (wall_err < -(MAX_SENSOR_ERR)) wall_err = -(MAX_SENSOR_ERR);
					} else if (RDSensor > (CENTER_TO_RIGHT_WALL)){
						wall_err = -(RDSensor - CENTER_TO_RIGHT_WALL)/SENSOR_RATIO;
						// Check if error is valid for correction ( too far from wall)
						if (wall_err > (MAX_SENSOR_ERR)) wall_err = MAX_SENSOR_ERR;
						if (wall_err < -(MAX_SENSOR_ERR)) wall_err = -(MAX_SENSOR_ERR);
					}  else {
						temp = 0;
						wall_err = 0;
					}
				} else {
					temp ++;
					wall_err = 0;
				}

			// CASE 2: have left wall
			} else if (LDSensor > DIAGNAL_LEFT_THRESHOLD){
				if (crr_case != 2){
					temp = 0;
					crr_case = 2;
				}
				if (temp > 200){
					if (LDSensor > (CENTER_TO_LEFT_WALL-80)){
						wall_err = (LDSensor - CENTER_TO_LEFT_WALL)/SENSOR_RATIO;
						// Check if error is valid for correction ( too far from wall)
						if (wall_err > (MAX_SENSOR_ERR)) wall_err = MAX_SENSOR_ERR;
						if (wall_err < -(MAX_SENSOR_ERR)) wall_err = -(MAX_SENSOR_ERR);
					}  else {
						temp = 0;
						wall_err = 0;
					}
				} else {
					temp ++;
					wall_err = 0;
				}

			// CASE 3: have right wall
				//TODO: implement something to distiquist these case
				// reset value when switch case
			} else if (RDSensor > DIAGNAL_RIGHT_THRESHOLD){
				if (crr_case != 3){
					temp = 0;
					crr_case = 3;
				}
				if (temp > 200){
					if (RDSensor > (CENTER_TO_RIGHT_WALL-60)){
						wall_err = -(RDSensor - CENTER_TO_RIGHT_WALL)/SENSOR_RATIO;
						// Check if error is valid for correction ( too far from wall)
						if (wall_err > (MAX_SENSOR_ERR)) wall_err = MAX_SENSOR_ERR;
						if (wall_err < -(MAX_SENSOR_ERR)) wall_err = -(MAX_SENSOR_ERR);
					} else {
						temp = 0;
						wall_err = 0;
					}
				} else {
					temp ++;
					wall_err = 0;
				}

			// CASE 4: Dont see any wall - reset value
			} else {
				temp = 0;
				wall_err = 0;
			}


/*
			// CASE 2: have left wall
			if (LDSensor > DIAGNAL_LEFT_THRESHOLD){
				if (temp > 300){
					wall_err = (LDSensor - CENTER_TO_LEFT_WALL)/SENSOR_RATIO;
					// Check if error is valid for correction ( too far from wall)
					if (wall_err > (MAX_SENSOR_ERR)) wall_err = MAX_SENSOR_ERR;
					if (wall_err < -(MAX_SENSOR_ERR)) wall_err = -(MAX_SENSOR_ERR);
				} else {
					temp ++;
				}
			}

			// Case 3: Use right wall for correction
			else if (RDSensor > (CENTER_TO_RIGHT_WALL+300)){
				if (temp >300){
					wall_err = -(RDSensor - CENTER_TO_RIGHT_WALL)/SENSOR_RATIO;
					if (wall_err > (MAX_SENSOR_ERR)) wall_err = MAX_SENSOR_ERR;
					if (wall_err < -(MAX_SENSOR_ERR)) wall_err = -(MAX_SENSOR_ERR);
				} else {
					temp ++;
				}

			} else {
				temp = 0;
				wall_err = 0;
			}

*/
		} else {
			wall_err = 0;
		}



		// Ignore wall error
		// TODO: Implement follow wall

		// Left motor error to PID
		pid_err = left_speed - global_left_speed + wall_err;
		lpwm = sensor_pid_left_motor(pid_err);

		// Right motor error to PID
		pid_err = right_speed - global_right_speed - wall_err;
		rpwm = sensor_pid_right_motor(pid_err);

		// Set pwm
		setLeftPwm(lpwm);
		setRightPwm(rpwm);

		// Check if user require to terminate current function
		if (FUNC_TERMINATED){
			setLeftPwm(0);
			setRightPwm(0);
			return;
		}


	}

}


void Controller_frontwall_corection(){

	int current_time = Millis;
	int front_left, front_right;

	// Reset PID
	pid_reset();
	while((Millis - current_time) < 500){

		/* CHECK IF USER SEND A SIGNAL TO TERMINATE CURRENT FUCTION */
		if (FUNC_TERMINATED){
			setLeftPwm(0);
			setRightPwm(0);
			return;
		}

		readFrontSensor();
		front_left = (CENTER_TO_FRONT_LEFT - FLSensor)/FRONT_WALL_SENSOR_RATIO;
		front_right = (CENTER_TO_FRONT_RIGHT - FRSensor)/FRONT_WALL_SENSOR_RATIO;

		if (front_left > FRONT_WALL_CORRECTION_SPEED_LIMIT)
			front_left = FRONT_WALL_CORRECTION_SPEED_LIMIT;
		if (front_left < -FRONT_WALL_CORRECTION_SPEED_LIMIT)
			front_left = -FRONT_WALL_CORRECTION_SPEED_LIMIT;
		if (front_right > FRONT_WALL_CORRECTION_SPEED_LIMIT)
			front_right = FRONT_WALL_CORRECTION_SPEED_LIMIT;
		if (front_right < -FRONT_WALL_CORRECTION_SPEED_LIMIT)
			front_right = -FRONT_WALL_CORRECTION_SPEED_LIMIT;

		// Left motor error to PID
		front_left = sensor_pid_left_motor(front_left - global_left_speed);
		// Right motor error to PID
		front_right = sensor_pid_right_motor(front_right - global_right_speed);

		// Set pwm
		setLeftPwm(front_left);
		setRightPwm(front_right);

	}
	// Stay still
	Controller_run(0,0,0,0);
}



int Controller_checkwall(void)
{
	int returnvalue = 0;

	readSensor();
	if (FLSensor > FRONT_LEFT_THRESHOLD && FRSensor > FRONT_RIGHT_THRESHOLD)
		returnvalue |= (1 << FRONTWALL_BIT_POSITION );
	if (LDSensor > DIAGNAL_LEFT_THRESHOLD)
		returnvalue |= (1 << LEFTWALL_BIT_POSITION);
	if (RDSensor > DIAGNAL_RIGHT_THRESHOLD)
		returnvalue |= (1 << RIGHTWALL_BIT_POSITION);

	return returnvalue;

}

/*
 * Button 1 interrupt handler
 */
void Controller_button1_interrupt_handler(){
	FUNC_TERMINATED = 1;
}

/*
 * Button 1 interrupt handler
 */
void Controller_button2_interrupt_handler(){
	FUNC_TERMINATED = 1;
}


/*
 * WRITE TO FLASH
 */
void Controller_writeFlash(void){
  int i, j;

  FLASH_Unlock();


  FLASH_ClearFlag( FLASH_FLAG_EOP|FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
/**
  * @brief  Erases a specified FLASH Sector.
  *
  * @param  FLASH_Sector: The Sector number to be erased.
  *          This parameter can be a value between FLASH_Sector_0 and FLASH_Sector_11
  *
  * @param  VoltageRange: The device voltage range which defines the erase parallelism.
  *          This parameter can be one of the following values:
  *            @arg VoltageRange_1: when the device voltage range is 1.8V to 2.1V,
  *                                  the operation will be done by byte (8-bit)
  *            @arg VoltageRange_2: when the device voltage range is 2.1V to 2.7V,
  *                                  the operation will be done by half word (16-bit)
  *            @arg VoltageRange_3: when the device voltage range is 2.7V to 3.6V,
  *                                  the operation will be done by word (32-bit)
  *            @arg VoltageRange_4: when the device voltage range is 2.7V to 3.6V + External Vpp,
  *                                  the operation will be done by double word (64-bit)
  *
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PROGRAM,
  *                       FLASH_ERROR_WRP, FLASH_ERROR_OPERATION or FLASH_COMPLETE.
  */
  FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3);

  for(i=0; i<MAZE_SIZE; i++)
    for(j=0; j<MAZE_SIZE; j++)
    	FLASH_ProgramWord((MAZE_ADRESS + (i*MAZE_SIZE+j)*4), maze[i][j]);

  FLASH_Lock();
}


void Controller_readMazeFlash(void){
  u32 i, j;
  for(i=0; i<MAZE_SIZE; i++)
    for(j=0; j<MAZE_SIZE; j++)
      maze[i][j] = *(int *)(MAZE_ADRESS + (i*MAZE_SIZE+j)*4);
}


int Controller_mode_select(){
	int mode = MODE_DEFAULT;
	int old_left_dst = getLeftDistance();
	int old_right_dst = getRightDistance();



	int encode_val, exit = 0;
	int count;


	while (exit != 100000){

		encode_val = getRightDistance() - old_right_dst;
		exit = getLeftDistance() - old_left_dst;


		// This determine to exit the loop
		count = 0;
		while (exit > 2000){
			exit = getLeftDistance() - old_left_dst;
			ALL_LED_ON;
			delay_ms(300);
			ALL_LED_OFF;
			delay_ms(300);
			count ++;
			if (count  == 10){
				exit = 100000;
				break;
			}
		}

		if (encode_val < 3001){
			mode = MODE_EXPLORE;//Left Green led
			ALL_LED_OFF;
			LED1_ON;
		} else if ((encode_val > 3000)&&(encode_val < 5001)){
			mode = MODE_READ_FLASH; // left red led
			ALL_LED_OFF;
			LED2_ON;
		} else if ((encode_val > 5000)&&(encode_val < 7001)){
			mode = MODE_WRITE_FLASH; // left blue
			ALL_LED_OFF;
			LED3_ON;
		} else if ((encode_val > 7000)&&(encode_val < 9001)){
			mode = MODE_TEST_SPEED_RUN;//right green
			ALL_LED_OFF;
			LED4_ON;
		}else if ((encode_val > 9000)&&(encode_val < 11001)){
			mode = MODE_TEST_SHARP_TURN;//right red // Random  - left prio
			ALL_LED_OFF;
			LED5_ON;
		}else if ((encode_val > 11000)&&(encode_val < 13001)){
			mode = MODE_EXPLORE_WITHOUT_SAVE_MAZE ;//right blue
			ALL_LED_OFF;
			LED6_ON;
		}else{
			mode = MODE_TEST_GO_STRAIGHT;//2 side led Random 2 - right prio
			LED1_ON;
			LED4_ON;
		}
	}

	delay_ms(100);
	return mode;
}
