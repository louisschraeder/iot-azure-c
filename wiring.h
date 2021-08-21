//
// Created by louis on 15.08.2021.
//

#ifndef WIRING_H_
#define WIRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>

#include "./config.h"
#include "./dht11.h"
#include "./soil_moisture_sensor.h"
#include "./Adafruit_ADS1015.h"

#define WIRINGPI_SETUP 1

#define TEMPERATURE_ALERT 30

int readMessage(int messageId, char *payload);

#endif  // WIRING_H_
