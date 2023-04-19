#include <wiringPi.h>
#include <assert.h>
#include <iostream>
#include <typeinfo>

// Note: for a CC LED, the pin state for active is 1 (aka true, via expresison below)
// #define PIN_STATE_ON 	static_cast<int>(typeid(polarity) == typeid(LED::DriveEnd::Cathode))
// #define PIN_STATE_OFF	!(PIN_STATE_ON)


namespace LED
{
	enum class DriveEnd { Anode, Cathode };

	template <LED::DriveEnd polarity>
	class Device
	{
		public:
			Device(const int _drivePin) :
			drivePin(_drivePin),
			active(typeid(polarity) == typeid(LED::DriveEnd::Cathode))
			{
				assert(wiringPiSetup() != -1);
				
				// Pinmode will be OUTPUT regardless of LED polarity
				pinMode(this->drivePin, OUTPUT);	
				this->set(0);
			}

			void set(bool state)
			{
				digitalWrite(this->drivePin, (state == this->active));
			}

		private:
			const int drivePin;
			const int active;
	};
}
