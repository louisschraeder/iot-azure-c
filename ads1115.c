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

void readAnalog()
{
    // open device on /dev/i2c-1 the default on Raspberry Pi B
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        printf("Error: Couldn't open device! %d\n", fd);
        exit (1);
    }

    // connect to ADS1115 as i2c slave
    if (ioctl(fd, I2C_SLAVE, asd_address) < 0) {
        printf("Error: Couldn't find device on address!\n");
        exit (1);
    }

    //Configuration
    writeBuf[0] = 1;
    writeBuf[1] = 0b11000010;
    writeBuf[2] = 0b10000101;

    // begin conversion
    if (write(fd, writeBuf, 3) != 3) {
        perror("Write to register 1");
        exit (1);
    }

    sleep(1);


    // set pointer to 0
    readBuf[0] = 0;
    if (write(fd, readBuf, 1) != 1) {
        perror("Write register select");
        exit(-1);
    }

    while(1)
    {
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


    /* Select configuration register(0x01)
    // AINP = AIN0 and AINN = AIN3, +/- 2.048V
    // Continuous conversion mode, 128 SPS(0x84, 0x83)
    char config2[3] = {0};
    config2[0] = 0x01;
    config2[1] = 0x94;
    config2[2] = 0x83;
    write(file, config2, 3);
    sleep(1);



    // Read 2 bytes of data from register(0x00)
    // raw_adc msb, raw_adc lsb
    char reg2[1] = {0x00};
    write(file, reg2, 1);
    char data2[2]={0};
    if(read(file, data2, 2) != 2)
    {
        printf("Error : Input/Output Error \n");
    }
    else
    {
        // Convert the data
        raw_adc = (data2[0] * 256 + data2[1]);



        // Output data to screen
        printf("Digital Value of Analog Input on AIN0 & AIN3: %d \n", raw_adc);
    }*/
}