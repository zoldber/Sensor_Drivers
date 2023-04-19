#ifndef VEML6030_H
#define VEML6030_H

#include "i2c.h"

namespace IC_VEML6030
{
	//----------------------------------------------------------------------------
	// COMMANDS (See Cmd. Register Format on Page 7 of Datasheet)
	//
	// Notes:
	// 	Six "commands" (fixed-addr registers) are supported by the VEML6030, and
	// 	each instance of "command" in the documentation is practically synonymous
	// 	with addressing an internal regiter corresponding to the subsequent R / W.
	// 	These will *never* be passed as 16 bits (the VEML6030's native word size).
	//----------------------------------------------------------------------------
	enum class Registers : uint8_t
	{
		ALS_CONF	= 0x00,	// Config register (see Table 1)
		ALS_WH		= 0x01,	// HIGH threshold window setting for ALS reading
		ALS_WL		= 0x02,	// LOW  threshold ...^
		RSV_POWER_S	= 0x03,	// Power saving (reserved) reg. [15:3] = 0, [2:0] = 0b???
		ALS_READ	= 0x04,	// ALS measurement (current reading on last active edge)
		WHITE_READ	= 0x05,	// White component ...^
		ALS_TRIGGER = 0x06	// Interrupt (reserved) register (see Table 6)
	};


	//----------------------------------------------------------------------------
	// CONFIG REGISTER (see Table #1 for Register 0)
	//
	// Notes:
	// 	THESE ARE **ALL** WRITE FIELDS.
	//----------------------------------------------------------------------------
	typedef struct
	{
		uint16_t /*RESERVED*/	: 3;	// Set to 0(...)
	
		// Per SparkFun demonstration notes:
		// Both .125 and .25 should be used in most cases except darker rooms.
		// A gain of 2 should only be used if the sensor will be covered by a dark
		// glass.
		uint16_t ALS_GAIN 		: 2;	// ALS gain mult, where x=ALS reading	
										// 		00		1*x
										//		01		2*x 
										//		10		x/8
										//		11		x/4

		uint16_t /*RESERVED*/	: 1;	// Set to 0(...)

		uint16_t ALS_IT			: 4;	// ALS integration time	
										//		1100	25  ms
										//		1000	50  ms
										//		0000	100 ms
										//		0001	200 ms
										//		0010	400 ms
										//		0011	800 ms

		uint16_t ALS_PERS		: 2;	// "ALS Persistence protect num." setting (?)
										// 		00		1
										//		01		2
										//		10		4
										//		11		8

		uint16_t /* RESERVED */	: 2;	// Set to 0(...)
		
		uint16_t ALS_INT_EN		: 1;	// ALS interrupt enabled (bool, 0 -> disable)
		
		uint16_t ALS_SD			: 1;	// ALS shut down setting (bool, 0 -> power on)
	}
	ctrl_reg_t;


	//----------------------------------------------------------------------------
	// SEE TABLE #4
	//----------------------------------------------------------------------------	
	typedef struct
	{
		uint16_t /*RESERVED*/	: 13;	// Set this to 0(...)

		uint16_t PSM			: 2;	// See table "refresh time" for definitions:
										//		00		mode 1
										//		01		mode 2
										//		10		mode 3
										//		11		mode 4

		uint16_t PSM_EN			: 1;	// Power saving mode enable (bool, 0 is disable)
	}
	power_reg_t;

	
	//----------------------------------------------------------------------------
	// Class implementation of VEML6030 hardware (as-is on Vishay board)
	//----------------------------------------------------------------------------	
	class Device : public I2C::Device
	{
		public:
			//
			Device(const uint8_t _addr);

			//
			bool threshWHExceeded(void);
			bool threshWLExceeded(void);

		private:
	};

}

#endif//VEML6030_H
