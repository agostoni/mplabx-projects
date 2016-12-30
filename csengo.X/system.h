#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

#define INPUT 1
#define OUTPUT 0

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
/**
 * Várakozás addot ideig
 * @param ms a várakozás id?tartama ms-ban
 */
void sleep_ms(long ms);
/**
 * A bekapcsolás óta eltelt id?
 * @return 
 */
long time();

/**
 * Kiír egy logüzenetet az UART outputra. Chak szimulátorban elérhet?, egyébként nem csinál semmit.
 * @param msg
 * @param ...
 */
void log(const char * msg,...);
//end system.h
#endif