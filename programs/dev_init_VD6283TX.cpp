#include "../drivers/VD6283TX.h"
#include <unistd.h> // getopt()
#include <iostream> // cin / cout
#include <iomanip>

#include <string>
#include <vector>
#include <typeinfo>

// For printBin<T>(x)
#include "../drivers/bit_track.h"

int main(int argc, char * argv[])
{
    int opt = -1;

    while ((opt = getopt(argc, argv, "h")) != -1)
    {
        //
    }

    // VD6283TX has a slave address of 0x40, or, according to the datasheet,
    // "an address of 0x20 in 7 bits" (as in 0x40>>1, wtf?)
    IC_VD6283TX::Device opticalSensor(0x20, 7, 0);

    opticalSensor.activate();

    uint8_t id;

    opticalSensor.readReg8(
        static_cast<uint8_t>(IC_VD6283TX::Addresses::DEVICE_ID), id);

    std::cout << "ID Register\t[ 0x" << std::hex << (int)(id) << " ]\n" << std::endl;


    auto testChannels =
    {
        IC_VD6283TX::Channels::Red,
        IC_VD6283TX::Channels::Green,
        IC_VD6283TX::Channels::Blue,
        IC_VD6283TX::Channels::IR,
        IC_VD6283TX::Channels::Clear,
        IC_VD6283TX::Channels::Visible
    };

    const std::string testLabels[6] =
    {
        "[0x06, CH1] Red    \t: ",
        "[0x12, CH4] Green  \t: ",
        "[0x0E, CH3] Blue   \t: ",
        "[0x16, CH5] IR     \t: ",
        "[0x1A, CH6] Clear  \t: ",
        "[0x0A, CH2] Visible\t: "
    };

    uint32_t dataBuffer = 0, i = 0;

    const uint32_t bitMask = 0xFF00000F;

    std::cout << "Mask Key: '.' -> \"don't care\" (assume 0) (see Table 4.3.3)\n" << std::endl;

    for (auto &channel : testChannels)
    {
        opticalSensor.readChannel(channel, dataBuffer);
        std::cout << testLabels[i++];

        printBits<uint32_t>(dataBuffer, bitMask);

        delay(32);
    }

    delay(512);

    opticalSensor.deactivate();

    return 0;
}