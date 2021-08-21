//
// Created by louis on 21.08.2021.
//

#ifndef IOT_AZURE_C_ADS1115_H
#define IOT_AZURE_C_ADS1115_H

#define	ADS1115_GAIN_6		0
#define	ADS1115_GAIN_4		1
#define	ADS1115_GAIN_2		2
#define	ADS1115_GAIN_1		3
#define	ADS1115_GAIN_HALF	4
#define	ADS1115_GAIN_QUARTER	5

//	Data rate

#define	ADS1115_DR_8		0
#define	ADS1115_DR_16		1
#define	ADS1115_DR_32		2
#define	ADS1115_DR_64		3
#define	ADS1115_DR_128		4
#define	ADS1115_DR_250		5
#define	ADS1115_DR_475		6
#define	ADS1115_DR_860		7


static int myAnalogRead (struct wiringPiNodeStruct *node, int pin);
static void myDigitalWrite(struct wiringPiNodeStruct *node, int pin, int data);
static void myAnalogWrite(struct wiringPiNodeStruct *node, int pin, int data);
int ads1115Setup(const int pinBase, int i2cAddr, struct wiringPiNodeStruct *node);

#endif //IOT_AZURE_C_ADS1115_H
