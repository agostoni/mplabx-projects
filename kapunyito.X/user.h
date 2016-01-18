#ifndef USER_H
#define USER_H

#include <pic16f887.h>

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

//L�bkioszt�s, bemenetek

//kimenetek
#define MOTOR_LE PORTBbits.RB0
#define MOTOR_LE_TRIS TRISB0

#define MOTOR_FEL PORTBbits.RB1
#define MOTOR_FEL_TRIS TRISB1

#define RIASZTO_KI PORTBbits.RB2
#define RIASZTO_KI_TRIS TRISB2

#define HEART_BEAT PORTDbits.RD0
#define HEART_BEAT_TRIS TRISD0


/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/*
 * Az alkalmaz�s inicializ�l�sa. Egyszer fut le indul�skor az els? f?ciklus
 *  el?tt.
 */
void initApp(void);
/*
 * Periodikusan hivodik a v�gtelens�gig. Itt kell a program logik�j�t 
 * megval�s�tani.
 */
void foCiklus(void);
#endif