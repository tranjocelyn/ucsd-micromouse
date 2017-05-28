/*
 * encoder.h
 *
 *  Created on: Apr 17, 2017
 *      Author: nguyenbui
 */

#ifndef ENCODER_H_
#define ENCODER_H_

extern int32_t global_left_dist;
extern int32_t global_right_dist;
extern int32_t global_left_speed;
extern int32_t global_right_speed;

int32_t getLeftDistance(void);
int32_t getRightDistance(void);

void update_speed(void);

void Encoder_Configration(void);


#endif /* ENCODER_H_ */
