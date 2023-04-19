#include "VD6283TX.h"

namespace IC_VD6283TX
{
    //-------------------------------------------------------------------------
    //  Following power up (tPU = 0.03 ms) of the VD6283, some internal
    //  registers must be initialized before any mode (e.g. ALS or Flicker) can
    //  operate. To ensure proper initialization of the sensor, one dedicated
    //  API function (STALS_Init) is available in the VD6283 software driver
    //  and should be called right after power up.
    //-------------------------------------------------------------------------
    Device::Device(const uint8_t _addr, const uint8_t _GPIO_VCC, const uint8_t _GPIO_INT) :
    I2C::Device(_addr),
    GPIO_VCC(_GPIO_VCC),
    GPIO_INT(_GPIO_INT)
    {
        // Power pin and interrupt pin should NEVER be the same
        assert(this->GPIO_VCC != this->GPIO_INT);

        //
        pinMode(this->GPIO_INT, INPUT);
        pinMode(this->GPIO_VCC, OUTPUT);

        //
        digitalWrite(this->GPIO_VCC, LOW);

        return;
    }

    void Device::activate(void)
    {
        /* Per online thread discussion likely init. sequences:
        1.  Determine which channels to enable and do so in the ALS channel
            enable register and channel 6 enable register
        2.  Set the channel gain in the ALS channel gain x registers.
        3.  Set the measurement period in the ALS period register.
        4.  Set the measurement mode bits, ALS_CONT, in the ALS control register
        5.  Start the measurement by setting the ALS_EN bit in the ALS control
            register
        6.  Next is to determine when data is ready. That might be possible to
            do by polling the INTR_ST bit in the interrupt status register.
        7.  Read the data from the enabled channels and interpret them (exactly
            how I'm not sure).
        8.  Clear the interrupt status bit as described in the datasheet.
        9.  If using continuous mode go back to 6
        */

        // Power on
        digitalWrite(this->GPIO_VCC, HIGH);
        delay(10);  // 0.03 ms (30 us) power up duration is specified by D.S.

        //---------------------------------------------------------------------
        // Enabled first 5 channels with one-hot encoding
        rsv_reg_CH1to5_enable_t reg_CH1to5_enable;
        reg_CH1to5_enable.flags.ALS_CH_EN = 0b11111;

        this->writeReg8(
            static_cast<uint8_t>(Addresses::ALS_CHANNEL_ENABLE), 
            reg_CH1to5_enable.word);

        // Then enable channel 6 with a single hot-bit
        rsv_reg_CH6_enable_t reg_CH6_enable;
        reg_CH6_enable.flags.CH6_EN = 0b1;

        this->writeReg8(
            static_cast<uint8_t>(Addresses::CHANNEL6_ENABLE), 
            reg_CH6_enable.word);


        //---------------------------------------------------------------------
        // Instruct ALS to use ALS Mode 1: Single ALS measurement request
        // ALS_CONT is enabled with a hot-bit on ALS_EN
        rsv_reg_ALS_control_t reg_ALS_control;
        reg_ALS_control.flags.ALS_CONT  = 0b00;
        reg_ALS_control.flags.ALS_EN    = 0b1;

        this->writeReg8(
            static_cast<uint8_t>(Addresses::ALS_CTRL),
            reg_ALS_control.word);


        //---------------------------------------------------------------------
        // Set inter-measurement period
        rsv_reg_ALS_period_t reg_ALS_period;
        reg_ALS_period.flags.ALS_IM_PER = 0x00;

        this->writeReg8(
            static_cast<uint8_t>(Addresses::ALS_PERIOD), 
            reg_ALS_period.word);

        // Set channels 1-6 (0x25-0x2A) to gain code 0b1101 -> gain = 1X
        gen_reg_ALS_gain_t reg_ALS_gain;
        reg_ALS_gain.flags.AGAIN = 0b1101;

        const uint8_t baseAddress = 
            static_cast<uint8_t>(Addresses::ALS_GAIN_CH1);

        // Works because gain registers are contiguous
        for (int i = 0; i < 6; i++)
            this->writeReg8((baseAddress + i), reg_ALS_gain.flags.AGAIN);


        //---------------------------------------------------------------------
        // [!] NOTE [!] : This "register" is split into two reigster addresses 
        // for no reason but to make life hard. To this end, note the use of
        // writeReg8() (see *8*) and bits written to "ALS_EXPOSURE_L" (see *L*)
        //
        // Keep exposure set to default (see Table 4.3.12)
        // Exp. time = (EXTIME[9:0] + 1) x 16384/Fosc | Fosc = 10.24 MHz
        // Default value = 80 ms (min. = 1.6 ms and max. = 1.6 s)
        // Hence, default register assignment is 49
        rsv_reg_ALS_exposure_t reg_ALS_exposure;
        reg_ALS_exposure.flags.EXTIME = 0b110001;
        
        this->writeReg8(
            static_cast<uint8_t>(Addresses::ALS_EXPOSURE_L),
            reg_ALS_exposure.word);


        //---------------------------------------------------------------------
        // Set pedestal value register to 3 (data sheet rec'd.)
        rsv_reg_pedestal_value_t reg_pedestal_value;
        reg_pedestal_value.flags.PDST_VAL = 0b11;   // Rec'd

        this->writeReg8(
            static_cast<uint8_t>(Addresses::PEDESTAL_VALUE),
            reg_pedestal_value.word);

        // By default, no flicker-extraction / subtraction is active

        // Go over Table 4.3.23 once more, implications of:
        //      "output driver 2 value (default)" 
        // are ambiguous and concerning. Notes re "I2C Fast Mode" also present.

        return;
    }

    //-------------------------------------------------------------------------
    //  
    //-------------------------------------------------------------------------
    void Device::deactivate(void)
    {
        digitalWrite(this->GPIO_VCC, 0);
        return;
    }


    //-------------------------------------------------------------------------
    //  Note 1:
    //      This implementation inherently guards against invalid channel 
    //      queries by restricting input ch to a member of a finite,
    //      enumerated channel list.
    //  
    //  Note 2: 
    //      Channels 1-6 are stored as 24 bit contiguous fields of bytes; I.e.
    //      three consecutive addresses store values:
    //      HSByte at [0x...N  ]
    //      MSByte at [0x...N+1]
    //      LSByte at [0x...N+2]
    //  
    //      which are returned as a 32 bit int with an 0x00 leading byte.
    //-------------------------------------------------------------------------
    void Device::readChannel(const Channels ch, uint32_t &w)
	{
        uint8_t buffer;

        const uint8_t baseAddr = static_cast<uint8_t>(ch);

        w = 0x00000000;

        for (int i = 0; i < 3; i++)
        {
            w <<= 8;
            this->readReg8((baseAddr + i), buffer);
            w |= buffer;
        }

        return;
	}
}
