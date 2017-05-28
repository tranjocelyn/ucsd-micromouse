#ifndef ADC_H
#define ADC_H


void ADC_Config(void);

/*
 * Sensors: front left, front right, left diagonal, right diagonal
 * Pins configuration for Basic mouse:
 * 	Left Diagonal(LD) RX --> Pin PB0 --> ADC1 channel 8
 * 	Left Front (FL) RX --> Pin PB1 --> ADC1 channel 9
 *  Right Front (FR) RX --> Pin PC4 --> ADC1 channel 14
 *  Right Diagonal (RD) RX --> Pin PC5 --> ADC1 channel 15
 *  Voltage Meter --> Pin PA3 --> ADC1 channel 3
 */

u16 readADC(ADC_TypeDef* ADCx, u8 channel, uint8_t sampleTime);

#define read_Vol_Meter     readADC(ADC1, 3, ADC_SampleTime_84Cycles)
#define read_FL_Sensor     readADC(ADC1, 9,  ADC_SampleTime_84Cycles)
#define read_FR_Sensor     readADC(ADC1, 14, ADC_SampleTime_84Cycles)
#define	read_LD_Sensor     readADC(ADC1, 8,  ADC_SampleTime_84Cycles)
#define read_RD_Sensor     readADC(ADC1, 15, ADC_SampleTime_84Cycles)

#endif
