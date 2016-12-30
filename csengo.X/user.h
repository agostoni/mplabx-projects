#ifndef USER_H
#define USER_H

#include <pic16f887.h>

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

//Lábkiosztás, bemenetek

//kimenetek
#define ZAR PORTBbits.RB0
#define ZAR_TRIS TRISB0

#define CSENGO PORTBbits.RB1
#define CSENGO_TRIS TRISB1


#define HEART_BEAT PORTDbits.RD0
#define HEART_BEAT_TRIS TRISD0

#define ST_1 PORTDbits.RD1
#define ST_1_TRIS TRISD1

#define ST_2 PORTDbits.RD2
#define ST_2_TRIS TRISD2

#define SZAMJEGY_VALTO_TIMEOUT 1000
#define NYIT_TIMEOUT 2000
#define CSENGO_TIMEOUT 5000

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/*
 * Az alkalmazás inicializálása. Egyszer fut le induláskor az els? f?ciklus
 *  el?tt.
 */
void initApp(void);
/*
 * Periodikusan hivodik a végtelenségig. Itt kell a program logikáját 
 * megvalósítani.
 */
void foCiklus(void);
#endif