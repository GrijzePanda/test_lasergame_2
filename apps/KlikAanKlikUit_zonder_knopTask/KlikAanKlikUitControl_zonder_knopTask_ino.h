// by Marius Versteegen, 2023

// This application uses the CleanRTOS library.
// CleanRTOS is a collection of wrappers around FreeRTOS.
// It lives in the namespace crt (abbreviation of CleanRTOS).
// It's purpose is to make use of FreeRTOS easier and safer.
//
// To help prevent potential headerfile name clashes, it's constituent files are prefixed with crt_
// The classnames themselves don't need the prefix because of membership of the namespace.
// A benefit of that choice is that you can use autocompletion after typing crt:: to find all CleanRTOS 
// parts.
//

// Dit voorbeeld is nog zonder Listener Pattern, etc.

#include <crt_CleanRTOS.h>        // This file includes crt_Config.h  You'll need to change defines there for a release build.

#include "crt_KlikAanKlikUitControl.h"
namespace crt
{
 	MainInits mainInits;            // Initialize CleanRTOS

	const int gpioPinVeerKnop = 34; // GPIO pin number for the VeerKnop
	const int gpioPinLed = 23;      // GPIO pin number for the LED

	KlikAanKlikUitControl klikAanKlikUitControl("KlikAanKlikUitControl", 2 /*priority*/, 4000 /*stackBytes*/, ARDUINO_RUNNING_CORE, gpioPinVeerKnop, gpioPinLed); // Don't forget to call its start() memeber during setup().
}
// ********  end of constant part of this .ino file  **************

void setup()
{
	ESP_LOGI("checkpoint", "start of main");vTaskDelay(1);
}

void loop()
{
	vTaskDelay(1);// Nothing to do in loop - all example code runs in the 4 threads above.
}
