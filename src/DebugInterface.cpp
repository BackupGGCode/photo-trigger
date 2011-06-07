/*
 *
 * This class contains 
 *
 *  Created on: Feb 12, 2011
 *      Author: Basil Shikin
 */

#include "DebugInterface.h"

DebugInterface::DebugInterface() {}

/**
 * Initialize debug interface
 */
void DebugInterface::initialize( Stream * stream, uint8_t debugPin)
{
	m_stream = stream;
	m_debugPin = debugPin;

	// Flash debug pin
	if ( PIN_DEBUG )
	{
		pinMode(m_debugPin, OUTPUT);

		blink( 2 );
	}

	if ( SERIAL_DEBUG )
	{
		m_stream->println("");
		m_stream->println("---");
		m_stream->println("");
	}
}

void DebugInterface::blink( uint8_t times )
{
	for (uint8_t i = 0; i < times; i++)
	{
		digitalWrite(m_debugPin, HIGH);
		delay(FLASH_INTERVAL);
		digitalWrite(m_debugPin, LOW);
		delay(FLASH_INTERVAL);
	}

	digitalWrite(m_debugPin, HIGH);
}

void DebugInterface::debug( const char * message )
{
	if ( SERIAL_DEBUG )
	{
		m_stream->println( message );
	}
}

void DebugInterface::debug( const char * message, uint32_t number )
{
	if ( SERIAL_DEBUG )
	{
		m_stream->print( message );
		m_stream->println( number, DEC);
	}
}
