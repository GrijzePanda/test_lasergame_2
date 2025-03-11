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

		void ledGaAan()
		{
      		gpio_set_level((gpio_num_t)gpioPinLed, 1);
 		}

 		void ledGaUit()
 		{
      		gpio_set_level((gpio_num_t)gpioPinLed, 0);
  		}

	private:
		/*override keyword not supported*/
		void main()
		{
			vTaskDelay(1000); // good practice: wait for other threads to have started up as well.

			enum State { WachtOpIndrukkenMetLedUit, WachtOpLoslatenMetLedAan,
			             WachtOpIndrukkenMetLedAan, WachtOpLoslatenMetLedUit };
			State state = WachtOpIndrukkenMetLedUit;

			ledGaUit();

			while (true)
			{
				dumpStackHighWaterMarkIfIncreased(); 		// This function call takes about 0.25ms! It should be called while debugging only.

				switch (state)
				{
					case WachtOpIndrukkenMetLedUit:
						vTaskDelay(100); // wait 100ms to prevent bouncing
						if(isKnopNeer())
						{
							ledGaAan();
							state = WachtOpLoslatenMetLedAan;
						}
						break;

					case WachtOpLoslatenMetLedAan:
						vTaskDelay(100); // wait 100ms to prevent bouncing
						if(!isKnopNeer())
						{
							state = WachtOpIndrukkenMetLedAan;
						}
						break;

					case WachtOpIndrukkenMetLedAan:
						vTaskDelay(100); // wait 100ms to prevent bouncing
						if(isKnopNeer())
						{
							ledGaUit();
							state = WachtOpLoslatenMetLedUit;
						}
						break;

					case WachtOpLoslatenMetLedUit:
						vTaskDelay(100); // wait 100ms to prevent bouncing
						if(!isKnopNeer())
						{
							state = WachtOpIndrukkenMetLedUit;
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
