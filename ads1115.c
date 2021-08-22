#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>    // read/write usleep
#include <stdlib.h>    // exit function
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions

int fd;
// Note PCF8591 defaults to 0x48!
int asd_address = 0x48;
int16_t val;
uint8_t writeBuf[3];
uint8_t readBuf[2];

void readAnalog() {
    // open device on /dev/i2c-1 the default on Raspberry Pi B
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        printf("Error: Couldn't open device! %d\n", fd);
        exit(1);
    }

    // connect to ADS1115 as i2c slave
    if (ioctl(fd, I2C_SLAVE, asd_address) < 0) {
        printf("Error: Couldn't find device on address!\n");
        exit(1);
    }

    //------------------------------------------------------------------------------------------------
    // 0xC2 single shot off
    // bit 15 flag bit for single shot not used here
    // Bits 14-12 input selection:
    // 100 ANC0; 101 ANC1; 110 ANC2; 111 ANC3
    // Bits 11-9 Amp gain. Default to 010 here 001 P19
    // Bit 8 Operational mode of the ADS1115.
    // 0 : Continuous conversion mode
    // 1 : Power-down single-shot mode (default)

    //Configuration
    writeBuf[0] = 1;
    writeBuf[1] = 0b11000010;
    writeBuf[2] = 0b10000101;

    // begin conversion
    if (write(fd, writeBuf, 3) != 3) {
        perror("Write to register 1");
        exit(1);
    }

    sleep(1);


    // set pointer to 0
    readBuf[0] = 0;
    if (write(fd, readBuf, 1) != 1) {
        perror("Write register select");
        exit(-1);
    }

    // read conversion register
    if (read(fd, readBuf, 2) != 2) {
        perror("Read conversion");
        exit(-1);
    }

    val = readBuf[0] * 256 + readBuf[1];

    if (val < 0)
        val = 0;

    printf("analog: %d\n", val);

    //------------------------------------------------------------------------------------------------
    //Configuration
    writeBuf[0] = 1;
    writeBuf[1] = 0b11010010;
    writeBuf[2] = 0b10000101;

    // begin conversion
    if (write(fd, writeBuf, 3) != 3) {
        perror("Write to register 1");
        exit(1);
    }

    sleep(1);


    // set pointer to 0
    readBuf[0] = 0;
    if (write(fd, readBuf, 1) != 1) {
        perror("Write register select");
        exit(-1);
    }

    // read conversion register
    if (read(fd, readBuf, 2) != 2) {
        perror("Read conversion");
        exit(-1);
    }

    val = readBuf[0] * 256 + readBuf[1];

    if (val < 0)
        val = 0;

    printf("analog: %d\n", val);

}