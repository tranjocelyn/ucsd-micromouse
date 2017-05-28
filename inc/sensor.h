/*
 * sensor.h
 *
 *  Created on: Apr 17, 2017
 *      Author: jocelyntran
 */

#ifndef SENSOR_H_
#define SENSOR_H_

extern int reflectionRate;

extern volatile int32_t volMeter;
extern volatile int32_t voltage;

/*
 * Sensors: front left, front right, left diagonal, right diagonal
 * Pins configuration for Basic mouse:
 * 	Left Diagonal(LD) RX --> Pin PB0 --> ADC1 channel 8
 * 	Left Front (LF) RX --> Pin PB1 --> ADC1 channel 9
 *  Right Front (RF) RX --> Pin PC4 --> ADC1 channel 14
 *  Right Diagonal (RD) RX --> Pin PC5 --> ADC1 channel 15
 *  Voltage Meter --> Pin PA3 --> ADC1 channel 3
 */

extern volatile int32_t FLSensor;
extern volatile int32_t FRSensor;
extern volatile int32_t LDSensor;
extern volatile int32_t RDSensor;


void readSensor(void);
void readFrontSensor(void);
void read_side_sensors(void);
void readGyro(void);
void readVolMeter(void);
void lowBatCheck(void);

#endif /* SENSOR_H_ */
