#ifndef I2C_H
#define I2C_H

#include <wiringPiI2C.h>	// Supports R/W of 8-, 16-bit words via SCL and SDA
#include <wiringPi.h>		// Defines delay(_ms), might see further use
#include <stdint.h>			// Defines fixed-width ints and supports bit-casting
#include <assert.h>			// Handles fatal init error in constructor
#include <cstring>			// Supports memcpy() with void * __restrict__ args

#define _I2C_INIT_FAULT_ -1

namespace I2C
{
	class Device
	{
		public:
			Device(const uint8_t _addr);
			
			//
			const uint8_t addr;
			const uint8_t fd;

			// Note: pass by reference whenever possible. I/O methods (see below) that
			//	return an int are **assigning the read value to arg "&w" (word) and only
			//	returning the exit code of the corresponding GPIO call**.
			
			//
			void readByte(uint8_t &b);
			int sendByte(const uint8_t &b);

			//
			void readWord(uint16_t &w);
			int sendWord(const uint16_t &w);

			//
			void readReg8(const uint8_t reg, uint8_t &b);
			int writeReg8(const uint8_t reg, const uint8_t b);

			//
			void readReg16(const uint8_t reg, uint16_t &w);
			int writeReg16(const uint8_t reg, const uint16_t w);

		private:
			//
	};
}

#endif//I2C_H
