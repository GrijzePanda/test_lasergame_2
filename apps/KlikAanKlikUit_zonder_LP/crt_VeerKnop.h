// by Marius Versteegen, 2025

#pragma once
#include <crt_CleanRTOS.h>]
#include "crt_KlikAanKlikUitControl.h"

namespace crt
{
	class VeerKnop : public Task
	{
	private:
		KlikAanKlikUitControl& klikAanKlikUitControl;
		int gpioPin;
		
	public:
		VeerKnop(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, 
		         KlikAanKlikUitControl& klikAanKlikUitControl, int gpioPin) :
			Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), 
			klikAanKlikUitControl(klikAanKlikUitControl), gpioPin(gpioPin)
		{
			// set as input pin:
			esp_rom_gpio_pad_select_gpio((gpio_num_t)gpioPin);
			gpio_set_direction((gpio_num_t)gpioPin, (gpio_mode_t) 1 /*input*/);

			start();
		}

	private:
		// private function that enhances readability of the main function
		// and avoids duplicate code
		bool isKnopNeer()
		{
			return gpio_get_level((gpio_num_t)gpioPin) == 1;
		}

		/*override keyword not supported*/
		void main()
		{
			enum State { WachtOpIndrukken, WachtOpLoslaten };
			State state = WachtOpIndrukken;

			vTaskDelay(1000); // good practice: wait for other threads to have started up as well.

			while (true)
			{
				dumpStackHighWaterMarkIfIncreased(); // only call this function while debugging, it takes about 0.25ms.

				switch (state)
				{
				case WachtOpIndrukken:
					vTaskDelay(100); // wait 100ms to prevent bouncing
					if (isKnopNeer())
					{
						klikAanKlikUitControl.knopIngedrukt();
						state = WachtOpLoslaten;
					}
					break;
				
				case WachtOpLoslaten:
					vTaskDelay(100); // wait 100ms to prevent bouncing
					if (!isKnopNeer())
					{
						state = WachtOpIndrukken;
					}
					break;

				default:
					ESP_LOGE("VeerKnop", "Invalid state");
					break;
				}
			}
		}
	}; // end class VeerKnop
};// end namespace crt
