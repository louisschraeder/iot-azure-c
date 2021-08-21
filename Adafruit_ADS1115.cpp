//
// Created by louis on 21.08.2021.
//

#include "./Adafruit_ADS1115.h"
#include <unistd.h>
#include <wiringPiI2C.h>

/**************************************************************************/
/*!
    @brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static uint8_t i2cread(uint8_t i2cFd) {
    return wiringPiI2CRead(i2cFd);
    //  #if ARDUINO >= 100
    //  return Wire.read();
    //  #else
    //  return Wire.receive();
    //  #endif
}

/**************************************************************************/
/*!
    @brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static void i2cwrite(uint8_t i2cFd, uint8_t x) {
    wiringPiI2CWrite(i2cFd, x);
    //  #if ARDUINO >= 100
    //  Wire.write((uint8_t)x);
    //  #else
    //  Wire.send(x);
    //  #endif
}

/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cFd, uint8_t i2cAddress, uint8_t reg, uint16_t value) {
    wiringPiI2CWriteReg16(i2cFd, reg, (value>>8) | (value<<8));

    //wiringPiI2CWriteReg8(m_i2cFd, reg, (uint8_t)(value>>8));
    //wiringPiI2CWriteReg8(m_i2cFd, reg, (uint8_t)(value & 0xFF));

    //wiringPiI2CWrite(m_i2cFd, reg);
    //wiringPiI2CWrite(m_i2cFd, (uint8_t)(value>>8));
    //wiringPiI2CWrite(m_i2cFd, (uint8_t)(value & 0xFF));

    // *** ORIGINAL ***
    //Wire.beginTransmission(i2cAddress);
    //i2cwrite(m_i2cFd, (uint8_t)reg);
    //i2cwrite(m_i2cFd, (uint8_t)(value>>8));
    //i2cwrite(m_i2cFd, (uint8_t)(value & 0xFF));
    //Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cFd, uint8_t i2cAddress, uint8_t reg) {
    wiringPiI2CWrite(i2cFd, ADS1015_REG_POINTER_CONVERT);
    uint16_t reading = wiringPiI2CReadReg16(i2cFd, reg);
    reading = (reading>>8) | (reading<<8); // yes, wiringPi did not assemble the bytes as we want
    return reading;

    //return wiringPiI2CReadReg16(m_i2cFd, reg);

    //return wiringPiI2CReadReg16(m_i2cFd, reg);

    //return ((i2cread(m_i2cFd) << 8) | i2cread(m_i2cFd));

    // *** ORIGINAL ***
    //Wire.beginTransmission(i2cAddress);
    //i2cwrite(m_i2cFd, ADS1015_REG_POINTER_CONVERT);
    //Wire.endTransmission();
    //Wire.requestFrom(i2cAddress, (uint8_t)2);
    //return ((i2cread(m_i2cFd) << 8) | i2cread(m_i2cFd));
}



/**************************************************************************/
/*!
   @brief  Instantiates a new ADS1015 class w/appropriate properties
*/
/**************************************************************************/
Adafruit_ADS1015::Adafruit_ADS1015(uint8_t i2cAddress)
{
    m_i2cAddress = i2cAddress;
    m_i2cFd = -1;
    m_conversionDelay = ADS1015_CONVERSIONDELAY;
    m_bitShift = 4;
    m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
}

/**************************************************************************/
/*!
    @brief  Instantiates a new ADS1115 class w/appropriate properties
*/
/**************************************************************************/
Adafruit_ADS1115::Adafruit_ADS1115(uint8_t i2cAddress)
{
    m_i2cAddress = i2cAddress; // 48
    m_i2cFd = -1;
    m_conversionDelay = ADS1115_CONVERSIONDELAY; // 8
    m_bitShift = 0;
    m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
}

/**************************************************************************/
/*!
    @brief  Sets up the HW (reads coefficients values, etc.)
*/
/**************************************************************************/
void Adafruit_ADS1015::begin() {
    printf("setting up ADS with m_i2cAddress = %d\n", m_i2cAddress);
    m_i2cFd = wiringPiI2CSetup(m_i2cAddress);
    printf("m_i2cFd = %d\n", m_i2cFd);
}

/**************************************************************************/
/*!
    @brief  Sets the i2c address specifying which ads circuit to talk to
*/
/**************************************************************************/
void Adafruit_ADS1015::setI2cAddress(uint8_t i2cAddress) {
    m_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*!
    @brief  Sets the gain and input voltage range
*/
/**************************************************************************/
void Adafruit_ADS1015::setGain(adsGain_t gain)
{
    m_gain = gain;
}

/**************************************************************************/
/*!
    @brief  Gets a gain and input voltage range
*/
/**************************************************************************/
adsGain_t Adafruit_ADS1015::getGain()
{
    return m_gain;
}

/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel
*/
/**************************************************************************/
uint16_t Adafruit_ADS1015::readADC_SingleEnded(uint8_t channel) {
    if (channel > 3)
    {
        return 0;
    }

    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
            ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
            ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
            ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
            ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

            // Set PGA/voltage range
            config |= m_gain;

            // Set single-ended input channel
            switch (channel)
            {
                case (0):
                    config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
                    break;
                    case (1):
                        config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
                        break;
                        case (2):
                            config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
                            break;
                            case (3):
                                config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
                                break;
            }

            // Set 'start single-conversion' bit
            config |= ADS1015_REG_CONFIG_OS_SINGLE;

            // Write config register to the ADC
            writeRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

            // Wait for the conversion to complete
            usleep(1000*m_conversionDelay);

            // Read the conversion results
            // Shift 12-bit results right 4 bits for the ADS1015
            return readRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
}

/**************************************************************************/
/*!
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN0) and N (AIN1) input.  Generates
            a signed value since the difference can be either
            positive or negative.
*/
/**************************************************************************/
int16_t Adafruit_ADS1015::readADC_Differential_0_1() {
    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
            ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
            ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
            ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
            ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

            // Set PGA/voltage range
            config |= m_gain;

            // Set channels
            config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1;          // AIN0 = P, AIN1 = N

            // Set 'start single-conversion' bit
            config |= ADS1015_REG_CONFIG_OS_SINGLE;

            // Write config register to the ADC
            writeRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

            // Wait for the conversion to complete
            usleep(1000*m_conversionDelay);

            // Read the conversion results
            uint16_t res = readRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
            if (m_bitShift == 0)
            {
                return (int16_t)res;
            }
            else
            {
                // Shift 12-bit results right 4 bits for the ADS1015,
                // making sure we keep the sign bit intact
                if (res > 0x07FF)
                {
                    // negative number - extend the sign to 16th bit
                    res |= 0xF000;
                }
                return (int16_t)res;
            }
}

/**************************************************************************/
/*!
    @brief  Reads the conversion results, measuring the voltage
            difference between the P (AIN2) and N (AIN3) input.  Generates
            a signed value since the difference can be either
            positive or negative.
*/
/**************************************************************************/
int16_t Adafruit_ADS1015::readADC_Differential_2_3() {
    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
            ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
            ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
            ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
            ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

            // Set PGA/voltage range
            config |= m_gain;

            // Set channels
            config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3;          // AIN2 = P, AIN3 = N

            // Set 'start single-conversion' bit
            config |= ADS1015_REG_CONFIG_OS_SINGLE;

            // Write config register to the ADC
            writeRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

            // Wait for the conversion to complete
            usleep(1000*m_conversionDelay);

            // Read the conversion results
            uint16_t res = readRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
            if (m_bitShift == 0)
            {
                return (int16_t)res;
            }
            else
            {
                // Shift 12-bit results right 4 bits for the ADS1015,
                // making sure we keep the sign bit intact
                if (res > 0x07FF)
                {
                    // negative number - extend the sign to 16th bit
                    res |= 0xF000;
                }
                return (int16_t)res;
            }
}

/**************************************************************************/
/*!
    @brief  Sets up the comparator to operate in basic mode, causing the
            ALERT/RDY pin to assert (go from high to low) when the ADC
            value exceeds the specified threshold.
            This will also set the ADC in continuous conversion mode.
*/
/**************************************************************************/
void Adafruit_ADS1015::startComparator_SingleEnded(uint8_t channel, int16_t threshold)
{
    // Start with default values
    uint16_t config = ADS1015_REG_CONFIG_CQUE_1CONV   | // Comparator enabled and asserts on 1 match
            ADS1015_REG_CONFIG_CLAT_LATCH   | // Latching mode
            ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
            ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
            ADS1015_REG_CONFIG_MODE_CONTIN  | // Continuous conversion mode
            ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

            // Set PGA/voltage range
            config |= m_gain;

            // Set single-ended input channel
            switch (channel)
            {
                case (0):
                    config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
                    break;
                    case (1):
                        config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
                        break;
                        case (2):
                            config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
                            break;
                            case (3):
                                config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
                                break;
            }

            // Set the high threshold register
            // Shift 12-bit results left 4 bits for the ADS1015
            writeRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_HITHRESH, threshold << m_bitShift);

            // Write config register to the ADC
            writeRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
}

void Adafruit_ADS1015::updateWiringPiSetup()
{
    m_i2cFd = wiringPiI2CSetup(m_i2cAddress);
    printf("new fd: %d\n", m_i2cFd);
}

/**************************************************************************/
/*!
    @brief  In order to clear the comparator, we need to read the
            conversion results.  This function reads the last conversion
            results without changing the config value.
*/
/**************************************************************************/
int16_t Adafruit_ADS1015::getLastConversionResults()
{
    // Wait for the conversion to complete
    usleep(1000*m_conversionDelay);

    // Read the conversion results
    uint16_t res = readRegister(m_i2cFd, m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
    if (m_bitShift == 0)
    {
        return (int16_t)res;
    }
    else
    {
        // Shift 12-bit results right 4 bits for the ADS1015,
        // making sure we keep the sign bit intact
        if (res > 0x07FF)
        {
            // negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
        return (int16_t)res;
    }
}
