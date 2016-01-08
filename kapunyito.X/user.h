#ifndef USER_H
#define USER_H

#include <pic16f887.h>

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

//Lábkiosztás, bemenetek

//#define NYOMOGOMB PORTAbits.RA0
//#define NYOMOGOMB_TRIS TRISA0
//#define VEGFOK PORTAbits.RA1
//#define VEGFOK_TRIS TRISA1

//kimenetek
#define MOTOR_LE PORTBbits.RB0
#define MOTOR_LE_TRIS TRISB0

#define MOTOR_FEL PORTBbits.RB1
#define MOTOR_FEL_TRIS TRISB1

#define HEART_BEAT PORTDbits.RD0
#define HEART_BEAT_TRIS TRISD0


/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void initApp(void);         /* I/O and Peripheral Initialization */

void foCiklus(void);         /* I/O and Peripheral Initialization */

#endif