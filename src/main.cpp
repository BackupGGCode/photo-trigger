#include "main.h"

#include <stdlib.h>

extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

void * operator new[](size_t size)
{
    return malloc(size);
}

void operator delete[](void * ptr)
{
	if (ptr)
		free(ptr);
}

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);}
void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
void __cxa_guard_abort (__guard *) {}



void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

// Pin mappings
#define PIN_IN_SENSITIVITY A0 //
#define PIN_IN_DELAY A1 //
#define PIN_IN_SENSOR A3 //

#define PIN_IN_TEST_FIRE 4
#define PIN_IN_ARMED 5

#define PIN_OUT_POWER_LED 3  //
#define PIN_OUT_FLASH_LED 2  //

#define PIN_OUT_FLASH 7
#define PIN_OUT_CAMERA 8

// Debug interface
static DebugInterface s_debugInterface;

// State variables
int  m_currentDelay;
int  m_currentSignal;
int  m_currentSensitivity;
bool m_currentIsArmed;
bool m_currentInTestFire;

uint32_t m_cycleCount;

void setup()
{
	// Initialize debug interface
	Serial.begin( DEBUG_BAUD_RATE );
	s_debugInterface.initialize( &Serial, PIN_OUT_POWER_LED );

	// Setup defaults
	m_currentDelay = 0;
	m_currentSignal = 0;
	m_currentSensitivity = 0;
	m_currentIsArmed = false;
	m_currentInTestFire = false;
	m_cycleCount = 0;

	// Setup pins
	setupPins();

}

void setupPins()
{
	pinMode( PIN_OUT_FLASH_LED, OUTPUT );
	pinMode( PIN_OUT_FLASH, OUTPUT );
	pinMode( PIN_OUT_CAMERA, OUTPUT );

	pinMode( PIN_IN_ARMED, INPUT );
	pinMode( PIN_IN_TEST_FIRE, INPUT );
}

void fireFlash()
{
	if ( m_currentDelay == 0 )
	{
		digitalWrite( PIN_OUT_FLASH, HIGH );
		digitalWrite( PIN_OUT_FLASH, LOW );

		digitalWrite( PIN_OUT_FLASH_LED, HIGH );
		digitalWrite( PIN_OUT_CAMERA, HIGH );
	}
	else
	{
		digitalWrite( PIN_OUT_CAMERA, HIGH );
		digitalWrite( PIN_OUT_FLASH_LED, HIGH );

		delay( m_currentDelay );

		digitalWrite( PIN_OUT_FLASH, HIGH );
		digitalWrite( PIN_OUT_FLASH, LOW );
	}

	printDebugInfo();

	delay( 1000 );

	// Turn everything off
	digitalWrite( PIN_OUT_CAMERA, LOW );
	digitalWrite( PIN_OUT_FLASH_LED, LOW );

	delay( 5000 );  // Wait to prevent double-fires

	m_cycleCount = 0; // Refresh values later
}

void printDebugInfo()
{
	if ( SERIAL_DEBUG )
	{
		s_debugInterface.debug( "\n\nFiring flash");

		s_debugInterface.debug( "Signal: ", m_currentSignal );
		s_debugInterface.debug( "Sensitivity: ", m_currentSensitivity );
		s_debugInterface.debug( "Delay: ", m_currentDelay );
		s_debugInterface.debug( "Armed: ", m_currentIsArmed );
	}
}

void loop()
{
	if ( m_cycleCount > VALUE_REFRESH_CYCLE ||
 	     m_cycleCount == 0 )
	{
		m_currentSensitivity = analogRead( PIN_IN_SENSITIVITY );
		m_currentDelay       = 1024 - analogRead( PIN_IN_DELAY );
		m_currentIsArmed     = ( digitalRead( PIN_IN_ARMED ) == LOW );
		m_currentInTestFire  = digitalRead( PIN_IN_TEST_FIRE ) == HIGH;
	}
	m_cycleCount += 1;

	m_currentSignal = analogRead( PIN_IN_SENSOR );
	if ( m_currentSignal >= m_currentSensitivity )
	{
		if ( m_currentIsArmed )
		{
			fireFlash();
		}
		else
		{
			printDebugInfo();

			digitalWrite( PIN_OUT_FLASH_LED, HIGH );
			delay( 500 );
			digitalWrite( PIN_OUT_FLASH_LED, LOW );

			m_cycleCount = 0; // Refresh values later
		}
	}
	else if ( m_currentInTestFire )
	{
		m_currentInTestFire = false;

		fireFlash();
	}
}

int main(void)
{
	init();

	setup();

	while ( true )
	{
		loop();
	}

	return 0;
}
