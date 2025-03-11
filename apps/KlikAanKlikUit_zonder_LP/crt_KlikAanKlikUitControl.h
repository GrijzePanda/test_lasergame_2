// by Marius Versteegen, 2025

#pragma once
#include <crt_CleanRTOS.h>
#include "crt_Led.h"

namespace crt
{
	class KlikAanKlikUitControl : public Task
	{
	private:
        Flag flagKnopIngedrukt;
		Led led;

	public:
		KlikAanKlikUitControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, int gpioPinLed) :
			Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), flagKnopIngedrukt(this), led(gpioPinLed)
		{
			start();
		}

    void knopIngedrukt()
    {
        flagKnopIngedrukt.set();
    }

	private:
		/*override keyword not supported*/
		void main()
		{
			vTaskDelay(1000); // good practice: wait for other threads to have started up as well.

			enum State { UIT, AAN };
			State state = UIT;

			while (true)
			{
				dumpStackHighWaterMarkIfIncreased(); 		// This function call takes about 0.25ms! It should be called while debugging only.

				switch (state)
				{
					case UIT:
						led.gaUit();
						wait(flagKnopIngedrukt);
						state = AAN;
						break;

					case AAN:
						led.gaAan();
						wait(flagKnopIngedrukt);
						state = UIT;
						break;

					default:
						ESP_LOGE("KlikAanKlikUitControl_zonder", "Invalid state");
						break;			
				}
			}
		}
	}; // end class KlikAanKlikUitControl_zonder
};// end namespace crt
