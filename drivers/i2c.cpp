#include "i2c.h"

namespace I2C
{
	Device::Device(const uint8_t _addr) :
	addr(_addr),
	fd(wiringPiI2CSetup(this->addr))
	{	
		// Note: fd == -1 is the only explicit failure mode. 
		// The fd handle returned on init should be nonzero.
		assert(this->fd != _I2C_INIT_FAULT_);

		// This is new, if something weird breaks it's this.
		// wiringPiSetup() is called because some i2c devices
		// need to have a digital input / output pin that's
		// a single, physical pin, and not a serial register.
		assert(wiringPiSetup() != -1);
		
		return;
	}

	//
	void Device::readByte(uint8_t &b)
	{
		b = wiringPiI2CRead(this->fd);

		return;
	}

	//
	int Device::sendByte(const uint8_t &b)
	{
		return wiringPiI2CWrite(this->fd, b);
	}

	// TODO: assess need for this and best implement the corresponding wiringPi function
	// only **after** reading its documentation (wiringPi has a right way and a WRONG way)
	void Device::readWord(uint16_t &w)
	{
		assert(false);

		return;
	}

	// TODO: assess need for this and best implement the corresponding wiringPi function
	// only **after** reading its documentation (wiringPi has a right way and a WRONG way)
	int Device::sendWord(const uint16_t &w)
	{
		assert(false);

		return 0;
	}

	//
	int Device::writeReg8(const uint8_t reg, const uint8_t b)
	{
		return wiringPiI2CWriteReg8(this->fd, reg, b);
	}

	//
	void Device::readReg8(const uint8_t reg, uint8_t &b)
	{
		b = wiringPiI2CReadReg8(this->fd, reg);

		return;
	}


	//
	int Device::writeReg16(const uint8_t reg, const uint16_t w)
	{
		return wiringPiI2CWriteReg16(this->fd, reg, w);
	}

	//
	void Device::readReg16(const uint8_t reg, uint16_t &w)
	{
		w = wiringPiI2CReadReg16(this->fd, reg);

		return;
	}
}
