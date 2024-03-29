#ifndef EVNPortSelector_h
#define EVNPortSelector_h

#include <Arduino.h>
#include <Wire.h>

#define DEFAULT_I2C_FREQ (uint32_t)100000

class EVNPortSelector
{
public:
	static const uint8_t I2C_ADDR = 0x70;
	EVNPortSelector(uint32_t i2c_freq = DEFAULT_I2C_FREQ);
	void begin();
	void setPort(uint8_t port);
	uint8_t getPort();
	uint8_t getWire0Port();
	uint8_t getWire1Port();
	void printPorts();

private:
	uint8_t _wire0_port = 1, _wire1_port = 9, _port = 1;
	uint32_t _i2c_freq;
};

#endif