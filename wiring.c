//
// Created by louis on 15.08.2021.
//

#include "./wiring.h"

static unsigned int BMEInitMark = 0;

void printBits(size_t const size, void const * const ptr);

Adafruit_ADS1115 ads;


int mask_check(int check, int mask)
{
    return (check & mask) == mask;
}



int readMessage(int messageId, char *payload)
{
    int16_t adc0;
    double milliVolts;
    double bits2milliVoltsFactor;

    // The ADC input range (or gain) can be changed via the following
    // functions, but be careful never to exceed VDD +0.3V max, or to
    // exceed the upper and lower limits if you adjust the input range!
    // Setting these values incorrectly may destroy your ADC!
    //                                                                ADS1015  ADS1115
    //                                                                -------  -------
    // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
    // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
    // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
    ads.setGain(GAIN_FOUR);
    bits2milliVoltsFactor = 0.03125; // remember to change this according to gain


    if ( wiringPiSetup() == -1 )
    {
        printf( "wire setup fail\n" );
        return -1;
    }

    ads.begin();
    while (1) {
        adc0 = ads.readADC_Differential_0_1();
        milliVolts = adc0 * bits2milliVoltsFactor;
        printBits(sizeof(adc0), &adc0);
        printf(" *** %5d *** %f\n", adc0, milliVolts);
        delay(10000);
    }

    int earth, rain;
    readEarth(&earth);
    readRain(&rain);


    int wert0, wert1, wert2, wert3;
    if (read_dht11_dat(&wert0, &wert1, &wert2, &wert3) != 1)
    {
        printf( "wire read fail\n" );
        return -1;
    }

    snprintf(payload,
             BUFFER_SIZE,
             "{ \"deviceId\": \"myDevice\", \"messageId\": %d, \"temperature\": %d.%d, \"humidity\": %d.%d, \"earth\": %d, \"rain\": %d }",
             messageId,
             wert2, wert3,
             wert0, wert1,
             earth, rain);


    return 0;
}

//assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--) {
        for (j=7;j>=0;j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    //puts("");
}


