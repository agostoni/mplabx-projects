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
     * Regisztrál egy gomb figyel?t az adott porton. Meghívja az adott függvényt ha esemény történt.
     * @param port A figyelni kívánt port (pl. PORTA)
     * @param portConfig A porthoz tartozó config regiszter (pl TRISA). A figyelt láb digitális inputként lesz beállítva
     * @param pin 0-7 szám, a figyelni kívánt láb a porton belül
     * @param activeon 0 vagy 1, 0 ha a gomb 1->0 átmenetre élesedik, 1 ha 0->1-re
     * @param callback A függvény amit meg kell hívni, ha esemény történt
     */
    void registerListener(
        volatile unsigned char* port,
        volatile unsigned char* portConfig,
        int pin,
        int activeon,
        void (* callback)(void));
    /**
     * Ezt meg kell hívni a f?ciklusból periodikusan.
     */
    void monitorButtons();

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONS_H */

