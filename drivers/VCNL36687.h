#ifndef VCNL36687_H
#define VCNL36687_H

#include "i2c.h"

namespace IC_VCNL36687
{
	//-------------------------------------------------------------------------
	// COMMANDS (See Tables 1-10 for Command Register Summaries)
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
		PS_CONF_12		= 0x03,	// R/W	- Primary config reg, see Table 2
		PS_CONF_34		= 0x04,	// R/W	- Active force mode, VCSEL current sel.
		PS_CONF_THDL	= 0x05,	// R/W	- PS low interrupt threshold setting
		PS_CONF_THDH	= 0x06,	// R/W	- high ...^
		PS_CONF_CANC	= 0x07,	// R/W	- PS cancellation level setting
		PS_CONF_5		= 0x08,	// R/W	- Power-on-start setting
		PS_DATA			= 0xF2,	// R	- Proximity reading output register
		INT_FLAG		= 0xF3,	// R	- PS interrupt flags (Note: not cfg)
		ID				= 0xF4	// R	- Default value is: 0x0588
	};


	//-------------------------------------------------------------------------
	//	Corresponds to PS_CONF1:2, Tables 2:3
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;

		struct
		{
			uint16_t	// 2-byte bit-field

			//	MSByte: PS_CONF2
			//-----------------------------------------------------------------
			PS_IT		: 2,	// PS Integration Time ('T' units, see Table 3)
								//	00		1 x T
								//	01		2 x T
								//	10		4 x T
								//	11		8 x T

			PS_MPS		: 2,	// PS multi-pulse setting
								//	00		1 [-]
								//	01		2 [-]
								//	10		4 [-]
								//	11		8 [-]

			PS_ITB		: 1, 	// PS Integration Time (?) Bank Setting
								//	0		25 us
								//	1		50 us

			/*RESERVED*/: 3,	// Set to (0...)

			//	LSByte: PS_CONF1
			//-----------------------------------------------------------------
			PS_PERIOD	: 2,	// PS sample period (*not* IT duration)
								// 	00		 8 ms
								// 	01		16 ms
								// 	10		32 ms
								// 	11		64 ms

			PS_PERS		: 2,	// Interrupt persistence setting
								// 	00		1 [-]
								// 	01		2 [-]
								// 	10		3 [-]
								// 	11		4 [-]

			PS_INT		: 2,	// Interrupt type configuration
								//	00		interrupt disabled
								//	01		interrupt disable
								//	10		interrupt enable
								//	11		trigger by logic H/L mode
			
			PS_SMART_PS	: 1,	// PS Smart Persistence (0 = disabled)

			PS_SD		: 1;	// Shutdown (0 = power on)
		}
		flags;
	}
	conf_reg_12_t;


	//-------------------------------------------------------------------------
	//	Corresponds to PS_CONF3:4, Tables 4:5
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;

		struct
		{
			uint16_t			// 2-byte bit-field
			
			PS_SC_EN	: 1,	// PS Sunlight Cancel Setting (0 = turn off)

			PS_SC_CUR	: 2,	// PS Sunlight Cancel Current
								//	00		1 x std SL cancel current
								//	01		2 x ...
								//	10		4 x ...
								//	11		8 x ...
			PS_HD		: 1,	// PS Output Width (0 = 12 bits, 1 = 16)

			/*RESERVED*/: 1,	// Set to (0...)
			
			VC_SEL_I	: 3,	// VCSEL Current Selection (yes, only these 5)
								//	000		 7 mA
								//	001		11 mA
								//	010		14 mA
								//	011		17 mA
								//	100		20 mA
			/*RESERVED*/: 1,	// Set to (0...)

			PS_AF		: 1,	// PS Active Force Mode (0 = auto, 1 = force)

			PS_FOR_Trig	: 1,	// PS ...^ Trigger (0 = none, 1 = trig 1 cycle)

			/*RESERVED*/: 5;	// Set to (0...)
		}
		flags;
	}
	conf_reg_34_t;
	

	//-------------------------------------------------------------------------
	//	Corresponds to PS_THDH_M/L, Table 6
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;
	
		struct
		{
			uint16_t			// 2-byte bit-field

			/*RESERVED*/: 4,	// Set to (0...)

			PS_THDL		: 12;	// Trigger threshold low, 0 -> 0x0FFF
		}
		flags;
	}
	conf_reg_THDL_t;
	

	//-------------------------------------------------------------------------
	//	Corresponds to PS_THDL_M/L, Table 7
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;

		struct
		{
			uint16_t			// 2-byte bit-field

			/*RESERVED*/: 4,	// Set to (0...)

			PS_THDH		: 12;	// Trigger threshold high, 0 -> 0x0FFF
		}
		flags;
	}
	conf_reg_THDH_t;
	

	//-------------------------------------------------------------------------
	//	Corresponds to PS_CANC_M/L, Table 8
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;

		struct
		{
			uint16_t			// 2-byte bit-field

			/*RESERVED*/: 4,	// Set to (0...)

			PS_CANC		: 12;	// Cancellation level setting, 0 -> 0x0FFF
		}
		flags;
	}
	conf_reg_cancel_t;


	//-------------------------------------------------------------------------
	//	Corresponds to PS_CONF5, Table 9
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;

		struct
		{
		uint16_t			// 2-byte bit-field

		/*RESERVED*/:	7,	// Set to (0...)

		PWR_ON_START: 	1,	// 1 = power-on-start 

		/*RESERVED*/: 	8;	// Set to (0...)
		}
		flags;
	}
	conf_reg_power_t;
	

	//-------------------------------------------------------------------------
	//	Corresponds to READ OUT REGISTER, Table 10
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;

		struct
		{
		uint16_t			// 2-byte bit-field

		SL_PR_STAT	: 1,	// Sunlight protect mode status (0 = normal)

		/*RSERVED*/	: 3,	// Set to (0...)

		PS_OUT_CODE	: 4,	// Proximity sensor's output code (wtf?)

		PS_READ_DATA: 8;	// Output Data is LSB	
		}
		flags;
	}
	output_reading_reg_t;


	//-------------------------------------------------------------------------
	//	Corresponds to READ OUT REGISTER, Table 10
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;

		struct
		{
		uint16_t			// 2-byte bit-field
		
		/*RESERVED*/: 3,	// Set to (0...)

		PS_SPFLAG	: 1,	// PS entering protection mode

		/*RESERVED*/: 2,	// Set to (0...)

		PS_IF_CLOSE	: 1,	// PS has risen above PS_THDH Int Thresh

		PS_IF_AWAY	: 1,	// PS has dropped below PS_THDL ...^

		/*RESERVED*/: 8;	// Set to (0...)
		}
		flags;
	}
	interrupt_flag_reg_t;
	

	//-------------------------------------------------------------------------
	//	Corresponds to READ OUT REGISTER, Table 10
	//-------------------------------------------------------------------------
	typedef union
	{
		uint16_t word;

		struct
		{
		uint16_t
		
		DEVICE_ID		: 8,	// Device ID, 0x88 = MP version sample

		/*RESERVED*/	: 2,	// Set to (0...)

		SLAVE_ADDR		: 2,	// Expect 0b11 (0x60) for a slave cfg.

		VERSION_CODE	: 4;	// Device ID version code
		}
		flags;
	}
	output_ID_reg_t;


	//-------------------------------------------------------------------------
	//	
	//-------------------------------------------------------------------------
	class Device : public I2C::Device
	{
		public:
			Device(const uint8_t _addr);

		private:

	};
}

#endif//VCNL36687_H
