#include "VEML6030.h"

namespace IC_VEML6030
{
	//----------------------------------------------------------------------------
	// VEML6030 Sensor Hardware
	// 
	// Notes:
	//----------------------------------------------------------------------------
	Device::Device(const uint8_t _addr) :
	I2C::Device(_addr)
	{
		return;
	}

	bool Device::threshWHExceeded(void)
	{
		return false;
	}

	bool Device::threshWLExceeded(void)
	{
		return false;
	}
}
