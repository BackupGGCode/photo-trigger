#include "WProgram.h"

#include "DebugInterface.h"

#define VALUE_REFRESH_CYCLE 1000

/**
 * Setup pin configurations
 */
void setupPins();


/**
 * Fire the flash
 */
void fireFlash();

void printDebugInfo();

void setup(); // Called once, sets up application variables
void loop();  // Called continuously, does main work


int main(void); // App entry point
