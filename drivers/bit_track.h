#include <iostream>
#include <unistd.h>
#include <stdlib.h>

// Supports a mask for "don't care conditions"
template <typename T>
void printBits(int x, int m)
{
	const T val = static_cast<T>(x);

	const T mask = static_cast<T>(m);

	int i = 0;

	//val will have size (in bytes): 1, 2, 4, 8
	for (T cursor = 0x80 << ( 8 * ( sizeof(T) - 1 ) ); cursor > 0; cursor /= 2)
	{
		if (i && ((i % 4) == 0)) std::cout << "-";

		i += 1;

		if (mask & cursor)
		{
			std::cout << ".";
		}
		else
		{
			std::cout << ((val & cursor) ? "1" : "0");
		}
	}

	// Flush buffer
	std::cout << std::endl;
	
	return;
}

// Take arg as an int, which is inherently polymorphic,
// then use static_cast via template arg (uint8_t etc).
template <typename T>
void printBits(int x)
{
	// Call masked printBits() with a DC mask of 0x0 (i.e. none)
	printBits<T>(0, 0x0);
	
	return;
}
