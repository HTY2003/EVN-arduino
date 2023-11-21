#ifndef EVNColourSensor_h
#define EVColourNSensor_h

#include <Arduino.h>
#include <Wire.h>
#include "EVNAlpha.h"
#include "EVNSensor.h"

#define GAIN_1X     0
#define GAIN_4X     1
#define GAIN_16X    2
#define GAIN_60X    3

class EVNColourSensor : private EVNSensor {
public:

    static const uint8_t I2C_ADDR = 0x29;
    static const uint8_t ID_REG_PART_NUMBER = 0x44;
    static const uint8_t ALT_ID_REG_PART_NUMBER = 0x4D;

    enum class reg : uint8_t
    {
        ENABLE = 0x00,
        ATIME = 0x01,
        WTIME = 0x03,
        AILTL = 0x04,
        AILTH = 0x05,
        AIHTL = 0x06,
        AIHTH = 0x07,
        PERS = 0x0C,
        CONFIG = 0x0D,
        CONTROL = 0x0F,
        ID = 0x12,
        STATUS = 0x13,
        CDATAL = 0x14,
        CDATAH = 0x15,
        RDATAL = 0x16,
        RDATAH = 0x17,
        GDATAL = 0x18,
        GDATAH = 0x19,
        BDATAL = 0x1A,
        BDATAH = 0x1B,
    };

    enum class mask : uint8_t
    {
        ENABLE_AIEN = 0x10,
        ENABLE_WEN = 0x08,
        ENABLE_AEN = 0x02,
        ENABLE_PON = 0x01,
        STATUS_AINT = 0x10,
        STATUS_AVALID = 0x01
    };

    EVNColourSensor(uint8_t port, uint8_t integration_cycles = 1, uint8_t gain = GAIN_4X) : EVNSensor(port)
    {
        _gain = constrain(gain, 0, 3);
        _int_cycles = constrain(integration_cycles, 1, 255);
    };

    void setGain(uint8_t gain)
    {
        _gain = constrain(gain, 0, 3);
        write8(I2C_ADDR, (uint8_t)reg::CONTROL, _gain);
    };

    void setIntegrationCycles(uint8_t integration_cycles)
    {
        _int_cycles = constrain(integration_cycles, 1, 255);
        write8(I2C_ADDR, (uint8_t)reg::ATIME, (uint8_t)(255 - (_int_cycles - 1)));
        _int_time_ms = _int_cycles * 2.4;
    };

    bool begin()
    {
        EVNAlpha::sharedPorts().begin();

        uint8_t id = read8(I2C_ADDR, (uint8_t)reg::ID);
        if (id != ID_REG_PART_NUMBER && id != ALT_ID_REG_PART_NUMBER) return false;

        this->setIntegrationCycles(_int_cycles);
        this->setGain(_gain);

        write8(I2C_ADDR, (uint8_t)reg::ENABLE, (uint8_t)mask::ENABLE_PON);
        delay(3);
        write8(I2C_ADDR, (uint8_t)reg::ENABLE, (uint8_t)mask::ENABLE_PON | (uint8_t)mask::ENABLE_AEN);

        return true;
    };

    void setRedRange(uint16_t low, uint16_t high)
    {
        _r_cal = true;
        _r_low = low;
        _r_high = max(low + 1, high);
    };

    void setGreenRange(uint16_t low, uint16_t high)
    {
        _g_cal = true;
        _g_low = low;
        _g_high = max(low + 1, high);
    };

    void setBlueRange(uint16_t low, uint16_t high)
    {
        _b_cal = true;
        _b_low = low;
        _b_high = max(low + 1, high);
    };

    void setClearRange(uint16_t low, uint16_t high)
    {
        _c_cal = true;
        _c_low = low;
        _c_high = max(low + 1, high);
    };

    uint16_t readRed()
    {
        if ((micros() - _r_time_us) > _int_time_ms * 1000)
        {
            _r = read16(I2C_ADDR, (uint8_t)reg::RDATAL);
            _r_time_us = micros();
        }
        return _r;
    };
    uint16_t readGreen()
    {
        if ((micros() - _g_time_us) > _int_time_ms * 1000)
        {
            _g = read16(I2C_ADDR, (uint8_t)reg::GDATAL);
            _g_time_us = micros();
        }
        return _g;
    };
    uint16_t readBlue()
    {
        if ((micros() - _b_time_us) > _int_time_ms * 1000)
        {
            _b = read16(I2C_ADDR, (uint8_t)reg::BDATAL);
            _b_time_us = micros();
        }
        return _b;
    };
    uint16_t readClear()
    {
        if ((micros() - _c_time_us) > _int_time_ms * 1000)
        {
            _c = read16(I2C_ADDR, (uint8_t)reg::CDATAL);
            _c_time_us = micros();
        }
        return _c;
    };

    double readRedCal()
    {
        if (_r_cal)
            return constrain(((double)this->readRed() - (double)_r_low) / ((double)_r_high - (double)_r_low), 0, 1);
        else
            return -1;
    };
    double readGreenCal()
    {
        if (_g_cal)
            return constrain(((double)this->readGreen() - (double)_g_low) / ((double)_g_high - (double)_g_low), 0, 1);
        else
            return -1;
    };
    double readBlueCal()
    {
        if (_b_cal)
            return constrain(((double)this->readBlue() - (double)_b_low) / ((double)_b_high - (double)_b_low), 0, 1);
        else
            return -1;
    };
    double readClearCal()
    {
        if (_c_cal)
            return constrain(((double)this->readClear() - (double)_c_low) / ((double)_c_high - (double)_c_low), 0, 1);
        else
            return -1;
    };

private:
    bool _r_cal = false, _g_cal = false, _b_cal = false, _c_cal = false;
    uint8_t _gain, _int_cycles;
    double _int_time_ms;
    uint16_t _r, _g, _b, _c;
    uint16_t _r_low, _g_low, _b_low, _c_low;
    uint16_t _r_high, _g_high, _b_high, _c_high;
    uint32_t _r_time_us, _g_time_us, _b_time_us, _c_time_us;
};

#endif