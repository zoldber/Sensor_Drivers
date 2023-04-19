#include "../drivers/bit_track.h"


int main(void)
{
	const uint32_t a = 0x01ABCDEF;

	printBits<uint32_t>(a);

	return 0;
}
