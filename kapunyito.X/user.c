/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include "user.h"
#include "system.h"
#include "buttons.h"
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/
uint32_t hb_last_timestamp = 0;

void nyomogombMegnyomva();
void vegfokAktiv();

void initApp(void) {
    //PORTA és PORTB konfigurálása dgitálisként
    ANSEL = 0;
    ANSELH = 0;
    registerListener(&PORTA,&TRISA,7,1,nyomogombMegnyomva);
    registerListener(&PORTA,&TRISA,2,1,vegfokAktiv);
    HEART_BEAT_TRIS = OUTPUT;
    MOTOR_FEL_TRIS = OUTPUT;
    MOTOR_LE_TRIS = OUTPUT;
    hb_last_timestamp = timeMillis();
}

bool hb_state = false;

void alapjel(int period) {
    uint32_t now = timeMillis();
    if (now - hb_last_timestamp > period) {
        hb_last_timestamp = now;
        hb_state = !hb_state; //state értéke 0 és 1 között változik
        HEART_BEAT = hb_state; // RD-0 beállítása  
    }
}

void foCiklus(void) {
    alapjel(1000);
    monitorButtons();
}


bool motor_aktiv = false;
//false -> irany le
bool motor_irany = false;


void frissit(){
    MOTOR_FEL = motor_aktiv && motor_irany;
    MOTOR_LE = motor_aktiv && !motor_irany;
}

void nyomogombMegnyomva(){
    motor_aktiv = !motor_aktiv;
    if (motor_aktiv){
        motor_irany = !motor_irany;
    }
    frissit();
}

void vegfokAktiv(){
    motor_aktiv = false;
    frissit();
}