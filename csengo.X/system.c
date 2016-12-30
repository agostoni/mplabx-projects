/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

//#include <xc.h>         /* XC8 General Include File */
#include "system.h"
#include <pic16f887.h>
#ifdef __SIMULATOR
#include <stdio.h>
#endif  
//a bekapcosl�s �ta eltelt millisecundumok
uint32_t timeInMillis = 0;

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    /**
     * Freki    IRCF2-0
     * 8Mhz     111
     * 4Mhz     110
     * 2Mhz     101
     * 1Mhz     100
     * 500Khz   011
     * 250Khz   010
     * 125Khz   001
     * 31Khz    000
     */
    OSCCONbits.SCS = 0;//�ra v�laszt�s az FOSC configur�ci� alapj�n
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF0 = 0; //1MHz-es �rajel
    // FIGYELJ az _XTAL_FREQ megfelel? be�ll�t�s�ra!
    
    /*
     * Timer inicializ�l�sa
     */
    
    /**
     * El?szorz� be�ll�t�sai
     * 
     * PS<2:0>  Szorz�
     * 000      2
     * 001      4
     * 010      8
     * 011      16
     * 100      32
     * 101      64
     * 110      128
     * 111      256
     */ 
    T0CS =0; //bels? �rajel haszn�lata
    T0SE =0; //felfut� �lre sz�mol
    PSA = 0; //TIMER 0 prescale enged�lyez�s
    PS2 = 0;//prescaler 8-cal oszt 
    PS1 = 1;
    PS0 = 0;
    TMR0 = 0; //timer null�z�sa
    /*
     Interruptok
     */
    T0IE = 1; //TMR0 t�lcsordul�s interrupt enged�lyez�se
    GIE = 1; //glob�lis interruptok enged�lyez�se. Ezt kell utolj�ra be�rni, hogy init k�zben ne t�rt�nhessen interrupt!!!
    
#ifdef __SIMULATOR
    //console logol�s enged�lyez�se szimul�tor m�dban
    TXSTAbits.TXEN = 1;               // enable transmitter
    RCSTAbits.SPEN = 1;               // enable serial port
#endif
}

uint32_t timeMillis(void) {
    uint32_t fclk = (0x01 << (OSCCONbits.IRCF - 1)) * 125;//[KHz]
    uint32_t pre = 0x01 << (OPTION_REGbits.PS +1);
#ifdef __SIMULATOR
    //valami�rt simul�tor m�dban nem �gy viselkedik az id?z�t�s, mint a val�s�gban
    float t0 = (256*pre) / fclk;
#else
    float t0 = (256*4*pre) / fclk;
#endif    
     //felbont�s [ms] ~= PS / flck [MHz]
    //1 MHz, 8-as prescaler -> 8,192 ms
    //~50 nap alatt csordul t�l
    return timeInMillis * t0;//[ms]
}

void sleep_ms(const long ms) {
    uint32_t now = timeMillis();
    while (timeMillis()-now < ms){};
}


void interrupt Timer0_ISR(void) {
    if (T0IE && T0IF) {
        T0IF = 0;
        timeInMillis++;
    }
}
#ifdef __SIMULATOR
//logol�shoz kell
void putch(unsigned char data) {
    while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
        continue;
    TXREG = data;                     // send one character
}
#endif

void log(const char * msg,...) {
//a log �zeneteket eldobjuk, ha nem simulator m�dban vagyunk
#ifdef __SIMULATOR
    va_list(argptr);
    va_start(argptr,msg);
    printf(msg, argptr);
    va_end(argptr);
#endif  
}

