// by Marius Versteegen, 2025

#pragma once
#include <crt_CleanRTOS.h>

namespace crt
{
	class KlikAanKlikUitControl : public Task
	{
		private:
		    int gpioPinKnop;
			int gpioPinLed;
	public:
		KlikAanKlikUitControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, int gpioPinKnop, int gpioPinLed) :
			Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), gpioPinKnop(gpioPinKnop), gpioPinLed(gpioPinLed)
		{
			// set as input pin:
			esp_rom_gpio_pad_select_gpio((gpio_num_t)gpioPinKnop);
			gpio_set_direction((gpio_num_t)gpioPinKnop, (gpio_mode_t) 1 /*input*/);

			// set as output pin:
			esp_rom_gpio_pad_select_gpio((gpio_num_t)gpioPinLed);
			gpio_set_direction((gpio_num_t)gpioPinLed, (gpio_mode_t) 2/*output*/);

			start();
		}

	private:
		bool isKnopNeer()
		{
			return gpio_get_level((gpio_num_t)gpioPinKnop) == 1;
		}

		void setLedValue(int ledValue)
		{
			gpio_set_level((gpio_num_t)gpioPinLed, ledValue);
		}

		void toggleLed(int& ledValue)
		{
			ledValue = 1-ledValue;   // toggle ledValue tussen 0 en 1
			setLedValue(ledValue);
		}

	private:
		/*override keyword not supported*/
		void main()
		{
			vTaskDelay(1000); // good practice: wait for other threads to have started up as well.

			enum State { WachtOpIndrukken, WachtOpLoslaten };
			State state = WachtOpIndrukken;
			int ledValue = 0; // led uit
			setLedValue(ledValue);

			while (true)
			{
				dumpStackHighWaterMarkIfIncreased(); 		// This function call takes about 0.25ms! It should be called while debugging only.

				switch (state)
				{
					case WachtOpIndrukken:
						vTaskDelay(100); // wait 100ms to prevent bouncing
						if(isKnopNeer())
						{
							toggleLed(ledValue);
							state = WachtOpLoslaten;
						}
						break;

					case WachtOpLoslaten:
						vTaskDelay(100); // wait 100ms to prevent bouncing
						if(!isKnopNeer())
						{
							state = WachtOpIndrukken;
						}
						break;

					default:
						ESP_LOGE("KlikAanKlikUitControl", "Invalid state");
						break;			
				}
			}
		}
	}; // end class KlikAanKlikUitControl_zonder
};// end namespace crt
