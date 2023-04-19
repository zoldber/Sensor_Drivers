#include "../drivers/VEML6030.h"
#include <iostream>	//i/o streams

#include <iomanip>	//setw(n)
#include <cstring>	//memcpy()

#include <unistd.h> //getopt()

#define MIN(x, y) ( (x < y) ? x : y )
#define MAX(x, y) ( (x > y) ? x : y )

#define BOUND(val, minBound, maxBound) \
	MIN(maxBound, MAX(minBound, val))

// In units of milliseconds
#define MAX_T_STEP 1024
#define MIN_T_STEP 16

//
static_assert(MIN_T_STEP < MAX_T_STEP);

//
inline int fault(const char * str)
{
	std::cerr << str << std::endl;
	return EXIT_FAILURE;
}

// ---
//
// ---
int main(int argc, char * argv[])
{
	// Config register exists at a nonstandard address, specific to the VEML6030
	const uint8_t cfgRegister = 
		static_cast<uint8_t>(IC_VEML6030::Registers::ALS_CONF);

	// Ambient Light Sensor Register
	const uint8_t alsRegister = 
		static_cast<uint8_t>(IC_VEML6030::Registers::ALS_READ);

	// White-Channel register
	const uint8_t wchRegister = 
		static_cast<uint8_t>(IC_VEML6030::Registers::WHITE_READ);

	size_t timeStep = MIN_T_STEP;

	// Note: This program should only support a -t <n> opt. 
	// It's more of a benchmarking tool than anything.
	int opt;

	while ((opt = getopt(argc, argv, "ht:")) != -1)
	{
		switch(opt)
		{
			case 'h':
				std::cout << "\tusage: ./test:\n";
				std::cout << "\t\t-t\tset (t)ime-step (ms): ";
				std::cout << MIN_T_STEP << ":" << MAX_T_STEP << "\n";
				std::cout << "\t\t-h\toptions\n";
				std::cout << std::endl;	
				
				return 1;

			case 't':
				if (!optarg)
					return fault("\topt '-t' expects argument");
				
				// Don't worry about notifying user of arg bounding
				timeStep = BOUND(std::stoi(optarg), MIN_T_STEP, MAX_T_STEP);

				break;

			default: //(?)
				std::cerr << "\trun with -h for options" << std::endl;
				return 1;
		}
	}

	IC_VEML6030::Device sensor(0x10);

	IC_VEML6030::ctrl_reg_t config { 0x0000 };

	config.ALS_GAIN 	= 0b11;		//
	config.ALS_IT		= 0b0000;	//
	config.ALS_PERS 	= 0b00;		//
	config.ALS_INT_EN 	= 0b0;		//
	config.ALS_SD		= 0b0;		//

	uint16_t buffer;

	memcpy(&buffer, &config, sizeof(buffer));

	sensor.writeReg16(cfgRegister, buffer);

	std::cout << "Time [ms],\tALS-Reg. [uc],\tWHT-Chl. [uc]" << std::endl;

	// Loop condition t < SIZE_MAX is never false, by definition of a max unsigned long.
	// Half the maximum size is more than sufficienti given the sampling window defined by
	// MIN_T_STEP and MAX_T_STEP values above.
	constexpr size_t MAX_RUNTIME = SIZE_MAX / 2;

	for (size_t t = 0; t < MAX_RUNTIME; t+= timeStep)
	{
		std::cout << t;

		sensor.readReg16(alsRegister, buffer);
		std::cout << ", " << std::setw(6) << buffer;

		sensor.readReg16(wchRegister, buffer);
		std::cout << ", " << std::setw(6) << buffer << std::endl;
 
		delay(timeStep);		
	}

	return 0;
}
