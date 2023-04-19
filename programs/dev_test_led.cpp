#include "../drivers/led_rgb.h"

#include <iostream> //i/o stream
#include <iomanip>	//setw()

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

//
inline bool isMember(const char * str, const char token)
{
	for (size_t i = 0; (i < 8) && (str[i] != '\0'); i++)
		if (str[i] == token) return true;

	return false;
}

// ---
//
// ---
int main(int argc, char * argv[])
{
	int opt;
	
	// Set-active on flag input only. These should be inactive by default.
	int r = 0, g = 0, b = 0;

	while ((opt = getopt(argc, argv, "hp:")) != -1)
	{
		switch(opt)
		{
			case 'h':
				std::cout << "\tusage: ./test:\n";
				std::cout << "\t\t-p\t(p)rimary color comp:  r,g,b,rg,rgb,...\n";
				std::cout << "\t\t-h\toptions\n";
				std::cout << std::endl;	
				
				return 1;

			case 'p':
				if (!optarg) 
					return fault("\topt '-p' expects argument");
	
				//
				r = isMember(optarg, 'r');
				g = isMember(optarg, 'g');
				b = isMember(optarg, 'b');			

				break;

			default: //(?)
				std::cerr << "\trun with -h for options" << std::endl;
				return 1;
		}
	}

	RGB_LED::Device<LED::DriveEnd::Anode> colorLED(0, 2, 3);

	colorLED.set(r, g, b);

	return 0;
}
