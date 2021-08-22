#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void readAnalog()
{
    // Create I2C bus
    int file;
    char *bus = "/dev/i2c-1";
    if ((file = open(bus, O_RDWR)) < 0)
    {
        printf("Failed to open the bus. \n");
        exit(1);
    }
    // Get I2C device, ADS1115 I2C address is 0x48(72)
    ioctl(file, I2C_SLAVE, 0x48);

    // Select configuration register(0x01)
    // AINP = AIN0 and AINN = AIN1, +/- 2.048V
    // Continuous conversion mode, 128 SPS(0x84, 0x83)
    char config[3] = {0};
    config[0] = 0x01;
    config[1] = 0x84;
    config[2] = 0x83;
    write(file, config, 3);
    sleep(1);

    // Read 2 bytes of data from register(0x00)
    // raw_adc msb, raw_adc lsb
    char reg[1] = {0x00};
    write(file, reg, 1);
    char data[2]={0};
    if(read(file, data, 2) != 2)
    {
        printf("Error : Input/Output Error \n");
    }
    else
    {
        // Convert the data
        int raw_adc = (data[0] * 256);
        int raw_adc2 = (data[1] * 256);

        // Output data to screen
        printf("Digital Value of Analog Input on AIN0 & AIN1: %d, %d  \n", raw_adc, raw_adc2);
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