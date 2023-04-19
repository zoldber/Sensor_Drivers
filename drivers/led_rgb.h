#include "led.h"

namespace RGB_LED
{
	template <LED::DriveEnd polarity>
	class Device
	{
		public:
			Device(const int pinR, const int pinG, const int pinB) :
			ledR(pinR),
			ledG(pinG),
			ledB(pinB)
			{	
				this->set(0, 0, 0);

				return;
			}
			
			void set(const int R, const int G, const int B)
			{
				ledR.set(R);
				ledG.set(G);
				ledB.set(B);

				return;
			}
	
		private:
			LED::Device<polarity> ledR, ledG, ledB;
	};
}
