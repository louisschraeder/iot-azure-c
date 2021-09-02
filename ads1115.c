#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit function
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions

int fd;
int asd_address = 0x48;
int16_t valEarth;
int16_t valRain;
uint8_t writeBuf[3];
uint8_t readBuf[2];

/**
 * Auslesen des Rain- und Bodenfeuchtesensors
 * @param earth
 * @param rain
 * @return
 */
int readAnalog(int * earth, int * rain) {
    // open device on /dev/i2c-1 the default on Raspberry Pi B
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        printf("Error: Couldn't open device! %d\n", fd);
        return -1;
    }

    // connect to ADS1115 as i2c slave
    if (ioctl(fd, I2C_SLAVE, asd_address) < 0) {
        printf("Error: Couldn't find device on address!\n");
        return -1;
    }


    //------------------------------------------------------------------------------------------------

    //Configuration
    writeBuf[0] = 1;
    writeBuf[1] = 0b11000010;
    writeBuf[2] = 0b10000101;

    // send Configuration
    if (write(fd, writeBuf, 3) != 3) {
        perror("Write to register 1");
        return -1;
    }

    sleep(1);


    // set pointer to 0
    readBuf[0] = 0;
    if (write(fd, readBuf, 1) != 1) {
        perror("Write register select");
        return -1;
    }

    // read conversion register
    if (read(fd, readBuf, 2) != 2) {
        perror("Read conversion");
        return -1;
    }

    valEarth = readBuf[0] * 256 + readBuf[1];

    if (valEarth < 0)
        valEarth = 0;


    //------------------------------------------------------------------------------------------------
    //Configuration
    writeBuf[0] = 1;
    writeBuf[1] = 0b11010010;
    writeBuf[2] = 0b10000101;

    // begin conversion
    if (write(fd, writeBuf, 3) != 3) {
        perror("Write to register 1");
        return -1;
    }

    sleep(1);


    // set pointer to 0
    readBuf[0] = 0;
    if (write(fd, readBuf, 1) != 1) {
        perror("Write register select");
        return -1;
    }

    // send Configuration
    if (read(fd, readBuf, 2) != 2) {
        perror("Read conversion");
        return -1;
    }

    valRain = readBuf[0] * 256 + readBuf[1];

    if (valRain < 0)
        valRain = 0;

    //printf("analogEarth: %d\n", valEarth);
    //printf("analogRain: %d\n", valRain);

    /**
     * Earth:
     * min = 32767
     * max = 10365
     *
     * Rain:
     * min = 32767
     * max = 6811
     * */

    int in_min, in_max, out_min, out_max;
    in_min = 32767;
    in_max = 10365;
    out_min = 0;
    out_max = 100;

    //Mappen des Sensorwerts Earth
    int valEarthOut = (valEarth - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    in_min = 32767;
    in_max = 6811;
    out_min = 0;
    out_max = 100;

    //Mappen des Sensorwerts Rain
    int valRainOut = (valRain - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    //printf("analogEarth Prozent: %d\n", valEarthOut);
    //printf("analogRain Prozent: %d\n", valRainOut);
    if(valEarthOut > 100)
        valEarthOut = 100;

    if(valRainOut > 100)
        valRainOut = 100;

    *earth = valEarthOut;
    *rain = valRainOut;

    return 1;
}
