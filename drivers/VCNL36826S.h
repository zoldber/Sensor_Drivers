#ifndef VCNL36826S_H
#define VCNL36826S_H

#include "i2c.h"

namespace IC_VCNL36826S
{
	//-------------------------------------------------------------------------
	// COMMANDS (See Tables 1-13 for Command Register Summaries)
	//
	// Notes:
	//	This device specifyies "DATA BYTE L/H" when describing register
	//	interface operations. This refers to LSByte/MSByte, respectively. To 
	//	avoid a dummy read before a DATA HIGH, or a dummy read following a DATA
	//	LOW, registers with a common addressing code have been concatenated
	//	into 16 bit words. This works well because exactly zero "command codes"
	//	address a pair of complementary registers that have mixed R and R/W
	//	capablities. 
	//
	//	TLDR: complementary registers A(_8) & B(8_) at 0xAD are now one AB(16)
	//
	//-------------------------------------------------------------------------
	enum Addresses : uint8_t
	{
		PS_CONF1	= 0x00,	// R/W	- Standby and on/off
		PS_CONF2	= 0x03,	// R/W	- PS period, persistence, PS-start/stop
		PS_CONF3	= 0x04,	// R/W	- Lots of flags (see Tables 6:7)
		PS_THDL		= 0x05,	// R/W	- Value: Threshold low
		PS_THDH		= 0x06,	// R/W	- Value: Threshold high
		PS_CANC		= 0x07,	// R/W	- PS cancellation setting
		PS_CONF4	= 0x08,	// R/W	- Low power config and **AUTO-CALIBRATION**
		PS_DATA		= 0xF8,	// R	- PS channel output data (see app. note)
		INT_FLAG	= 0xF9,	// R	- Interrupt flag register (see Table 13)
		PS_ID		= 0xFA,	// R	- Corresponds to device's slave address
		PS_AC_DATA	= 0xFB	// R	- Auto Calibration Data (cfg via PS_CONF4)
	};

	typedef union
	{
		uint16_t word;

		//	MSByte: PS_CONF1H
		//-----------------------------------------------------------------
		struct
		{
			uint16_t			//
			
			PS_CAL		: 1,	// Must be set to 1 when power-on ready.

			/*RESERVED*/: 5,	// Set to (0...)

			PS_ON		: 1,	// Set to 1 to enable bias circuit. Note that
								// the initialization process is as follows:
								// 	step(1)->{PS_ON = 1} step(2)->{PS_CAL = 1}
			/*RESERVED*/: 1,

		//	LSByte: PS_CONF1L
		//-----------------------------------------------------------------
			/*RESERVED*/: 6,	// 
			/*RESERVED*/: 1,	// "Must be set to 1 when power-on ready" (!?)
			/*RESERVED*/: 1;	// Set to (0...)
		};
		
	}
	conf_reg_1_t;

	typedef union
	{
		uint16_t word;

		struct
		{	
		};
		
	}
	conf_reg_2_t;

	typedef union
	{
		uint16_t word;

		struct
		{
		};
		
	}
	conf_reg_3_t;

	typedef union
	{
		uint16_t word;

		struct
		{
		};
		
	}
	conf_reg_4_t;




	

	class Device : public I2C::Device
	{
		public:
			Device(const uint8_t _addr);
			
		private:
	};
}

#endif//VCNL36826S_H
