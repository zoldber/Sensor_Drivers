#ifndef VD6283TX_H
#define VD6283TX_H

#include "i2c.h"

namespace IC_VD6283TX
{
    const uint8_t default_id = 0x40;

	//-------------------------------------------------------------------------
	// COMMANDS (See Tables 1-10 for Command Register Summaries)
	//	Given complementary registers:
    //      H(8__) & M(_8_) & L(__8) at consecutive addresses
    //      0xA0, 
    //      0xA1, 
    //      0xA2,
    //      Implement: ABC(24) as _ABC(32) where MSByte is either zero'd or 
    //      reserved for something special
	// Notes:
    //  Color Filters <-> Channels
    //      CH1 Red
    //      CH2 Visible
    //      CH3 Blue
    //      CH4 Green
    //      CH5 IR
    //      CH6 Clear
	//-------------------------------------------------------------------------
	enum class Addresses : uint8_t
	{
        DEVICE_ID	        =	0x00,   //  R   Default is 0x40
        REVISION_ID	        =	0x01,   //  R   Silicon revision ID

        INTERRUPT_CTRL	    =	0x02,   //
        ALS_CTRL	        =	0x03,   //
        ALS_PERIOD	        =	0x04,   //
        
        ALS_CH1_DATA_H	    =	0x06,   //  Red Channel
        ALS_CH1_DATA_M	    =	0x07,   //  ^
        ALS_CH1_DATA_L	    =	0x08,   //  ^

        ALS_CH2_DATA_H	    =	0x0A,   //  Visible Channel
        ALS_CH2_DATA_M	    =	0x0B,   //  ^
        ALS_CH2_DATA_L	    =	0x0C,   //  ^
        
        ALS_CH3_DATA_H	    =	0x0E,   //  Blue Channel
        ALS_CH3_DATA_M	    =	0x0F,   //  ^
        ALS_CH3_DATA_L	    =	0x10,   //  ^
        
        ALS_CH4_DATA_H	    =	0x12,   //  Green Channel
        ALS_CH4_DATA_M	    =	0x13,   //  ^
        ALS_CH4_DATA_L	    =	0x14,   //  ^
        
        ALS_CH5_DATA_H	    =	0x16,   //  IR Channel
        ALS_CH5_DATA_M	    =	0x17,   //  ^
        ALS_CH5_DATA_L	    =	0x18,   //  ^
        
        ALS_CH6_DATA_H	    =	0x1A,   //  Clear Channel
        ALS_CH6_DATA_M	    =	0x1B,   //  ^
        ALS_CH6_DATA_L	    =	0x1C,   //  ^
        
        //Low and Middle? Where is High? Not in the datasheet, that's for sure.
        ALS_EXPOSURE_M	    =	0x1D,
        ALS_EXPOSURE_L	    =	0x1E,

        // Gain modes
        ALS_GAIN_CH1	    =	0x25,
        ALS_GAIN_CH2	    =	0x26,
        ALS_GAIN_CH3	    =	0x27,
        ALS_GAIN_CH4    	=	0x28,
        ALS_GAIN_CH5	    =	0x29,
        ALS_GAIN_CH6	    =	0x2A,

        CHANNEL6_ENABLE	    =	0x2D,
        ALS_CHANNEL_ENABLE	=	0x2E,
        AC_MODE_CTRL	    =	0x31,
        PEDESTAL_VALUE	    =	0x32,
        SDA_DRV_CFG	        =	0x3C,
        GPIO1_DRV_CFG	    =	0x41
	};

    // Note: yes, these are the same as their respective base addresses above
    enum class Channels : uint8_t
    {
        Red     = 0x06,
        Visible = 0x0A,
        Blue    = 0x0E,
        Green   = 0x12,
        IR      = 0x16,
        Clear   = 0x1A
    };


	//-------------------------------------------------------------------------
    //  Table 4.3.1 -   Device identification register
	//-------------------------------------------------------------------------
	typedef union
	{
		uint8_t word;

		struct
		{
			uint8_t
            DEV_ID: 8;
        }
        flags;
    }
    rsv_reg_device_id_t;


	//-------------------------------------------------------------------------
    //  Table 4.3.2 -   Silicon revision identification register
	//-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            REV_ID: 8;
        }
        flags;
    }
    rsv_reg_silicon_revision_id_t;


	//-------------------------------------------------------------------------
    //  Table 4.3.3 -   Interrupt control register
    //
    //  INTR_ST: interrupt status
    //      1: no interrupt has triggered, or last interrupt hasn't cleared
    //  
    //  CLR_INTR: clear interrupt flag (handshake mechanism)
    //      INTR_ST interrupt flag is cleared by:
    //          CLR_INTR = '1' followed by CLR_INTR = '0'
	//-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            /*RESERVED*/: 6,
            INTR_ST     : 1,
            CLR_INTR    : 1;
        }
        flags;
    }
    rsv_reg_interrupt_control_t;


	//-------------------------------------------------------------------------
    //  Table 4.3.4 -   ALS control register
    //
    //  ALS_CONT: ALS continuous mode selection
    //      00: ALS mode 1: single ALS measurement request
    //      01: reserved
    //      11: ALS mode 3: continuous ALS measurement request defined by
    //          ALS_PERIOD with handshake mechanism from host.
    //  
    //  ALS_EN: enable ALS operation
    //      0: ALS operation is stopped (i.e. idle)
    //      1: ALS operation started with ALS_CONT mode
	//-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            /*RESERVED*/: 5,
            ALS_CONT    : 2,
            ALS_EN      : 1;
        }
        flags;
    }
    rsv_reg_ALS_control_t;


	//-------------------------------------------------------------------------
    //  Table 4.3.5 -   ALS period register
    //
    //  ALS_IM_PER: inter-measurement period
    //      LSB = 20.5 ms
    //      Min = 0 ms and Max = 5.22 s
	//-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            ALS_IM_PER  : 8;
        }
        flags;
    }
    rsv_reg_ALS_period_t;


    //-------------------------------------------------------------------------
    //  Tables 4.3.6-11 ALS data registers for channels 1-6
    //-------------------------------------------------------------------------
    typedef union
    {
        uint32_t word;

        struct
        {
            uint32_t
            /*PADDING*/     : 8,
            ALS_CH_DATA_H   : 8,
            ALS_CH_DATA_M   : 8,
            ALS_CH_DATA_L   : 8;
        }
        flags;
    }
    gen_reg_ALS_data_t;


    //-------------------------------------------------------------------------
    //  Table 4.3.12-   ALS exposure register
    //
    //  EXTIME: exposure time = (EXTIME[9:0] + 1) x 16384/Fosc
    //      Fosc = 10.24 MHz
    //      Default value = 80 ms (min. = 1.6 ms and max. = 1.6 s)
    //-------------------------------------------------------------------------
    typedef union
    {
        uint16_t word;

        struct
        {
            uint16_t
            /*RESERVED*/    : 6,
            EXTIME          : 10;
        }
        flags;
    }
    rsv_reg_ALS_exposure_t;


    //-------------------------------------------------------------------------
    //  Table 4.3.13-18 ALS gain registers for channels 1-6
    //
    //  AGAIN: channel x analog gain
    //      0000: reserved
    //      0001: AGAIN = 66.6x
    //      0010: AGAIN = 50x
    //      0011: AGAIN = 33x
    //      0100: AGAIN = 25x
    //      0101: AGAIN = 16x
    //      0110: AGAIN = 10x
    //      0111: AGAIN = 7.1x
    //      1000: AGAIN = 5x
    //      1001: AGAIN = 3.33x
    //      1010: AGAIN = 2.5x
    //      1011: AGAIN = 1.67x
    //      1100: AGAIN = 1.25x
    //      1101: AGAIN = 1x
    //      1110: AGAIN = 0.83x
    //      1111: AGAIN = 0.71x
    //-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            /*RESERVED*/: 3,
            AGAIN       : 5;
        }
        flags;
    }
    gen_reg_ALS_gain_t;


    //-------------------------------------------------------------------------
    //  Table 4.3.19-   Channel 6 enable register
    //
    //  CH6_EN: channel6 operation activation
    //      0: Channel 6 is disabled
    //      1: Channel 6 is enabled
    //-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            /*RESERVED*/    : 7,
            CH6_EN          : 1;
        }
        flags;
    }
    rsv_reg_CH6_enable_t;


    //-------------------------------------------------------------------------
    //  Table 4.3.20-   ALS channel enable register
    //
    //  ALS_CH_EN: ALS channel [1..5] operation
    //      00000: no channel enabled
    //      00001: enables channel 1
    //      00010: enables channel 2
    //      00100: enables channel 3
    //      01000: enables channel 4
    //      10000: enables channel 5
    //      11111: all channels [1...5] enabled
    //-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            /*RESERVED*/    : 3,
            ALS_CH_EN       : 5;
        }
        flags;
    }
    rsv_reg_CH1to5_enable_t;


    //-------------------------------------------------------------------------
    //  Table 4.3.21-   AC mode control register
    //
    //  Pedestal feature enable/disable:
    //      0: pedestal feature enabled (default)
    //      1: pedestal feature disabled
    //
    //  AC_OUT_SEL: AC flicker signal output selection
    //      0: PDM data output on GPIO1 MUX (default)
    //      1: PDM data output on GPIO2
    //
    //  Flicker operation channel selection:
    //      000: no channel selected (default)
    //      001: ALS channel 6 selected
    //      010: ALS channel 1 selected
    //      011: ALS channel 2 selected
    //      100: ALS channel 3 selected
    //      101: ALS channel 4 selected
    //      110: ALS channel 5 selected
    //
    //  Flicker frequency extractor:
    //      0: flicker frequency extractor feature disabled (default)
    //      1: flicker frequency extractor feature enabled
    //-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            /*RESERVED*/    : 1,
            PEDESTAL_DIS    : 1,
            /*RESERVED*/    : 1,
            AC_OUT_SEL      : 1,
            AC_CH_SEL       : 3,
            AC_FREQEXT_EN   : 1;
        }
        flags;
    }
    rsv_reg_AC_mode_control_t;

    //-------------------------------------------------------------------------
    //  Table 4.3.22-   Pedestal value register
    //
    //  Read/write of the pedestal value (applies for flicker operation only)
    //      Recommended pedestal value = 3
    //-------------------------------------------------------------------------
    typedef union
    {
        uint8_t word;

        struct
        {
            uint8_t
            /*RESERVED*/: 5,
            PDST_VAL    : 3;
        }
        flags;
    }
    rsv_reg_pedestal_value_t;


    //-------------------------------------------------------------------------
    //  
    //-------------------------------------------------------------------------
    class Device : public I2C::Device
    {
        public:
            Device(const uint8_t _addr, const uint8_t _GPIO_VCC, const uint8_t _GPIO_INT);

            void activate(void);
            void deactivate(void);

            void readChannel(const Channels ch, uint32_t &w);

            const uint8_t GPIO_VCC;
            const uint8_t GPIO_INT;

        private:
        
    };
}

#endif//VD6283TX_H
