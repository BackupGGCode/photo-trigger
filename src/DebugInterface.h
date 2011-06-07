/*
 *
 * This class represents debug interface used in EmailCam project
 *
 *  Created on: Feb 12, 2011
 *      Author: Basil Shikin
 */

#ifndef DEBUGINTERFACE_H_
#define DEBUGINTERFACE_H_

#include "WProgram.h"

#define DEBUG_BAUD_RATE 9600
#define SERIAL_DEBUG false
#define PIN_DEBUG true

#define FLASH_INTERVAL 500

class DebugInterface {
public:

	DebugInterface();

	/**
	 * Initialize debug interface
	 */
	void initialize( Stream * stream, uint8_t debugPin);

	/**
	 * Blink debug LED given number of times
	 */
	void blink( uint8_t times );

	/**
	 * Write a debug message into program logs
	 *
	 * @param message Message to log
	 */
	void debug( const char * message );

	/**
	 * Write a debug message into program logs
	 *
	 * @param message Message to log
	 * @param number  Number that is relevant to the message
	 */
	void debug( const char * message, uint32_t number );

private:
	Stream * m_stream;
	uint8_t  m_debugPin;
};

#endif /* DEBUGINTERFACE_H_ */
