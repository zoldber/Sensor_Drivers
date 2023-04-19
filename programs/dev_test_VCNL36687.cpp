#include "../drivers/VCNL36687.h"
#include <iostream>

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x > y) ? x : y)

int main(int argc, char * argv[])
{
	const uint8_t deviceAddress = 0x60;

	IC_VCNL36687::Device proxSensor(deviceAddress);

	// Initializing registers 1 -> 4 to 0x0000 is safe
	// for testing (all device default cfgs are set by 0).
	IC_VCNL36687::conf_reg_12_t configBitsA { 0x0 };
	IC_VCNL36687::conf_reg_34_t configBitsB { 0x0 };

	configBitsA.flags.PS_INT	= 0b11;	// Enable logic-interrupts (INT pin w/ LED)
	configBitsB.flags.PS_HD		= 0b1;	// Set output bit res. to 16 (0 = 12)

	proxSensor.writeReg16(IC_VCNL36687::Addresses::PS_CONF_12, configBitsA.word);
	proxSensor.writeReg16(IC_VCNL36687::Addresses::PS_CONF_34, configBitsB.word);

	uint16_t buffer { 0x0 };

	while (true)
	{
		proxSensor.readReg16(IC_VCNL36687::Addresses::PS_DATA, buffer);
		std::cout << (buffer & 0x00FF) << "\t| ";

		buffer = MIN((buffer/2), 64);

		// Draw refraction intensity as rolling "bar graph"
		while (buffer --> 0) std::cout << "=";

		std::cout << std::endl;

		delay(128);
	}

	return 0;
}
