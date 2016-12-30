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
 * V�rakoz�s addot ideig
 * @param ms a v�rakoz�s id?tartama ms-ban
 */
void sleep_ms(long ms);
/**
 * A bekapcsol�s �ta eltelt id?
 * @return 
 */
long time();

/**
 * Ki�r egy log�zenetet az UART outputra. Chak szimul�torban el�rhet?, egy�bk�nt nem csin�l semmit.
 * @param msg
 * @param ...
 */
void log(const char * msg,...);
//end system.h
#endif