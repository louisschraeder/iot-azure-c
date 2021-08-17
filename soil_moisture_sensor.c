//
// Created by louis on 17.08.2021.
//

#include "./soil_moisture_sensor.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define gpioEarth 11

int readEarth()
{
    uint8_t value = 0;

    value = digitalRead(gpioEarth);

    printf( "Earth: %d\n", value );

    return 0;
}