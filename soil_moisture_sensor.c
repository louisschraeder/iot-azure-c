//
// Created by louis on 17.08.2021.
//

#include "./soil_moisture_sensor.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define	CONFIG_OS_MASK		(0x8000)	// Operational Status Register
#define	CONFIG_OS_SINGLE	(0x8000)	// Write - Starts a single-conversion
// Read    1 = Conversion complete

// The multiplexor

#define	CONFIG_MUX_MASK		(0x7000)

// Differential modes

#define	CONFIG_MUX_DIFF_0_1	(0x0000)	// Pos = AIN0, Neg = AIN1 (default)
#define	CONFIG_MUX_DIFF_0_3	(0x1000)	// Pos = AIN0, Neg = AIN3
#define	CONFIG_MUX_DIFF_1_3	(0x2000)	// Pos = AIN1, Neg = AIN3
#define	CONFIG_MUX_DIFF_2_3	(0x3000)	// Pos = AIN2, Neg = AIN3 (2nd differential channel)

// Single-ended modes

#define	CONFIG_MUX_SINGLE_0	(0x4000)	// AIN0
#define	CONFIG_MUX_SINGLE_1	(0x5000)	// AIN1
#define	CONFIG_MUX_SINGLE_2	(0x6000)	// AIN2
#define	CONFIG_MUX_SINGLE_3	(0x7000)	// AIN3

// Programmable Gain Amplifier

#define	CONFIG_PGA_MASK		(0x0E00)
#define	CONFIG_PGA_6_144V	(0x0000)	// +/-6.144V range = Gain 2/3
#define	CONFIG_PGA_4_096V	(0x0200)	// +/-4.096V range = Gain 1
#define	CONFIG_PGA_2_048V	(0x0400)	// +/-2.048V range = Gain 2 (default)
#define	CONFIG_PGA_1_024V	(0x0600)	// +/-1.024V range = Gain 4
#define	CONFIG_PGA_0_512V	(0x0800)	// +/-0.512V range = Gain 8
#define	CONFIG_PGA_0_256V	(0x0A00)	// +/-0.256V range = Gain 16

#define	CONFIG_MODE		(0x0100)	// 0 is continuous, 1 is single-shot (default)

// Data Rate

#define	CONFIG_DR_MASK		(0x00E0)
#define	CONFIG_DR_8SPS		(0x0000)	//   8 samples per second
#define	CONFIG_DR_16SPS		(0x0020)	//  16 samples per second
#define	CONFIG_DR_32SPS		(0x0040)	//  32 samples per second
#define	CONFIG_DR_64SPS		(0x0060)	//  64 samples per second
#define	CONFIG_DR_128SPS	(0x0080)	// 128 samples per second (default)
#define	CONFIG_DR_475SPS	(0x00A0)	// 475 samples per second
#define	CONFIG_DR_860SPS	(0x00C0)	// 860 samples per second

// Comparator mode

#define	CONFIG_CMODE_MASK	(0x0010)
#define	CONFIG_CMODE_TRAD	(0x0000)	// Traditional comparator with hysteresis (default)
#define	CONFIG_CMODE_WINDOW	(0x0010)	// Window comparator

// Comparator polarity - the polarity of the output alert/rdy pin

#define	CONFIG_CPOL_MASK	(0x0008)
#define	CONFIG_CPOL_ACTVLOW	(0x0000)	// Active low (default)
#define	CONFIG_CPOL_ACTVHI	(0x0008)	// Active high

// Latching comparator - does the alert/rdy pin latch

#define	CONFIG_CLAT_MASK	(0x0004)
#define	CONFIG_CLAT_NONLAT	(0x0000)	// Non-latching comparator (default)
#define	CONFIG_CLAT_LATCH	(0x0004)	// Latching comparator

// Comparitor queue

#define	CONFIG_CQUE_MASK	(0x0003)
#define	CONFIG_CQUE_1CONV	(0x0000)	// Assert after one conversions
#define	CONFIG_CQUE_2CONV	(0x0001)	// Assert after two conversions
#define	CONFIG_CQUE_4CONV	(0x0002)	// Assert after four conversions
#define	CONFIG_CQUE_NONE	(0x0003)	// Disable the comparator (default)

#define	CONFIG_DEFAULT		(0x8583)	// From the datasheet


static const uint16_t dataRates [8] =
        {
        CONFIG_DR_8SPS, CONFIG_DR_16SPS, CONFIG_DR_32SPS, CONFIG_DR_64SPS, CONFIG_DR_128SPS, CONFIG_DR_475SPS, CONFIG_DR_860SPS
        } ;

static const uint16_t gains [6] =
        {
        CONFIG_PGA_6_144V, CONFIG_PGA_4_096V, CONFIG_PGA_2_048V, CONFIG_PGA_1_024V, CONFIG_PGA_0_512V, CONFIG_PGA_0_256V
        } ;


#define gpioEarth 0
#define gpioRain 2

int readEarth(int * earth)
{
    pinMode (gpioEarth, INPUT);

    if (digitalRead(gpioEarth) == LOW) {
        *earth = 0;
    }
    else
    {
        *earth = 1;
    }

    return 0;
}

int readRain(int * rain)
{
    pinMode (gpioRain, INPUT);

    if (digitalRead(gpioRain) == LOW) {
        *rain = 0;
    }
    else
    {
        *rain = 1;
    }

    return 0;
}

/*
 * analogRead:
 *	Pin is the channel to sample on the device.
 *	Channels 0-3 are single ended inputs,
 *	channels 4-7 are the various differential combinations.
 *********************************************************************************
 */

static int myAnalogRead (struct wiringPiNodeStruct *node, int pin)
{
    int chan = pin - node->pinBase ;
    int16_t  result ;
    uint16_t config = CONFIG_DEFAULT ;

    chan &= 7 ;

    // Setup the configuration register

    //	Set PGA/voltage range

    config &= ~CONFIG_PGA_MASK ;
    config |= node->data0 ;

    //	Set sample speed

    config &= ~CONFIG_DR_MASK ;
    config |= node->data1 ;

    //	Set single-ended channel or differential mode

    config &= ~CONFIG_MUX_MASK ;

    switch (chan)
    {
        case 0: config |= CONFIG_MUX_SINGLE_0 ; break ;
        case 1: config |= CONFIG_MUX_SINGLE_1 ; break ;
        case 2: config |= CONFIG_MUX_SINGLE_2 ; break ;
        case 3: config |= CONFIG_MUX_SINGLE_3 ; break ;

        case 4: config |= CONFIG_MUX_DIFF_0_1 ; break ;
        case 5: config |= CONFIG_MUX_DIFF_2_3 ; break ;
        case 6: config |= CONFIG_MUX_DIFF_0_3 ; break ;
        case 7: config |= CONFIG_MUX_DIFF_1_3 ; break ;
    }

    //	Start a single conversion

    config |= CONFIG_OS_SINGLE ;
    config = __bswap_16 (config) ;
    wiringPiI2CWriteReg16 (node->fd, 1, config) ;

    // Wait for the conversion to complete

    for (;;)
    {
        result =  wiringPiI2CReadReg16 (node->fd, 1) ;
        result = __bswap_16 (result) ;
        if ((result & CONFIG_OS_MASK) != 0)
            break ;
        delayMicroseconds (100) ;
    }

    result =  wiringPiI2CReadReg16 (node->fd, 0) ;
    result = __bswap_16 (result) ;

    // Sometimes with a 0v input on a single-ended channel the internal 0v reference
    //	can be higher than the input, so you get a negative result...

    if ( (chan < 4) && (result < 0) )
        return 0 ;
    else
        return (int)result ;
}

/*
 * ads1115Setup:
 *	Create a new wiringPi device node for an ads1115 on the Pi's
 *	I2C interface.
 *********************************************************************************
 */

int ads1115Setup (const int pinBase, int i2cAddr)
{
    struct wiringPiNodeStruct *node ;
    int fd ;

    if ((fd = wiringPiI2CSetup (i2cAddr)) < 0)
        return FALSE ;

    node = wiringPiNewNode (pinBase, 8) ;

    node->fd           = fd ;
    node->data0        = CONFIG_PGA_4_096V ;	// Gain in data0
    node->data1        = CONFIG_DR_128SPS ;	// Samples/sec in data1
    node->analogRead   = myAnalogRead ;
    node->analogWrite  = myAnalogWrite ;
    node->digitalWrite = myDigitalWrite ;

    return TRUE ;
}