/* 
 * File:   buttons.h
 * Author: isti
 *
 * Created on January 2, 2016, 4:57 PM
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    /**
     * Regisztr�l egy gomb figyel?t az adott porton. Megh�vja az adott f�ggv�nyt ha esem�ny t�rt�nt.
     * @param port A figyelni k�v�nt port (pl. PORTA)
     * @param portConfig A porthoz tartoz� config regiszter (pl TRISA). A figyelt l�b digit�lis inputk�nt lesz be�ll�tva
     * @param pin 0-7 sz�m, a figyelni k�v�nt l�b a porton bel�l
     * @param activeon 0 vagy 1, 0 ha a gomb 1->0 �tmenetre �lesedik, 1 ha 0->1-re
     * @param callback A f�ggv�ny amit meg kell h�vni, ha esem�ny t�rt�nt
     */
    void registerListener(
        volatile unsigned char* port,
        volatile unsigned char* portConfig,
        int pin,
        int activeon,
        void (* callback)(void));
    /**
     * Ezt meg kell h�vni a f?ciklusb�l periodikusan.
     */
    void monitorButtons();

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONS_H */

