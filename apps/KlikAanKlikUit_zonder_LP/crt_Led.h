// by Marius Versteegen, 2025

#pragma once
#include <crt_CleanRTOS.h>

// This file contains the code of multiple tasks that run concurrently and notify eachother using flags.

namespace crt
{
	class Led
	{
	private:
        int gpioPin;

	public:
		Led(int gpioPin) : gpioPin(gpioPin)
		{
			// set as output pin:
			esp_rom_gpio_pad_select_gpio((gpio_num_t)gpioPin);
			gpio_set_direction((gpio_num_t)gpioPin, (gpio_mode_t) 2/*output*/);
		}

		void gaAan()
		{
			gpio_set_level((gpio_num_t)gpioPin, 1);
		}

		void gaUit()
		{
			gpio_set_level((gpio_num_t)gpioPin, 0);
		}
	}; // end class Led
};// end namespace crt
