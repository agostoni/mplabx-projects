/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include "user.h"
#include "system.h"
#include "buttons.h"
/******************************************************************************/
/* Állapot                                                           */
/******************************************************************************/
/*
 * Az utolsó HB LED állapotváltás id?bélyege.
 */
uint32_t hb_last_timestamp = 0;
/**
 * Heart beat LED állapota
 */
bool hb_state = false;

/*Igaz, ha a motor jelenleg jár*/
bool motor_aktiv = false;
/*
 * A kapunyitó motor iránya.
 * false -> irany le
 */
bool motor_irany = false;
/*
 * Az utolsó érvényes riszató_ki jel id?bélyege
 */
uint32_t riasztoki_idobelyeg;
/*
 * Ha igaz, akkor a riaszto ki van kapcsolva.
 */
bool riaszto_ki;
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/**
 * Villogtat egy életjel LED-et a parameterben megadott periodusidovel.
 * 
 * @param period Az életjel led villogási periodusa millisekundumban
 */
void alapjel(int32_t period);
/*
 * Meghivodik ha a kapunyito nyomogombot megnyomtak.
 */
void nyomogombMegnyomva();
/*
 * Meghivodik, ha a vegallaskapcsolo megallitotta a motrot. A belso allapotot 
 * ennek megfeleloen frissiteni kell.
 */
void vegallasAktiv();
/*
 * Meghívódik, ha a riaszto ki jelet detektált a rendszer. Kikapcsolja a 
 * riasztót, és elindítja az id?zit?t.
 */
void riasztoKi();
/*
 * Újra engedélyezi a riasztót a parameterben megadott ido elteltevel.
 * 
 * @param timeout A kivant ido milliszekundumban
 */
void riasztoIdozito(int32_t timeout);


/******************************************************************************/
/* Interfész függvények                                                       */
/******************************************************************************/
void initApp(void) {
    //PORTA és PORTB konfigurálása dgitálisként
    ANSEL = 0;
    ANSELH = 0;
    registerListener(&PORTA,&TRISA,7,1,nyomogombMegnyomva);
    registerListener(&PORTA,&TRISA,2,1,vegallasAktiv);
    registerListener(&PORTA,&TRISA,4,1,riasztoKi);
    HEART_BEAT_TRIS = OUTPUT;
    MOTOR_FEL_TRIS = OUTPUT;
    MOTOR_LE_TRIS = OUTPUT;
    RIASZTO_KI_TRIS = OUTPUT;
    hb_last_timestamp = timeMillis();
}

void foCiklus(void) {
    alapjel(1000);
    monitorButtons();
    riasztoIdozito(60000);
}

/******************************************************************************/
/* "Privát" függvények                                                        */
/******************************************************************************/


void alapjel(int32_t period) {
    uint32_t now = timeMillis();
    if (now - hb_last_timestamp > period) {
        hb_last_timestamp = now;
        hb_state = !hb_state; //state értéke 0 és 1 között változik
        HEART_BEAT = hb_state; // RD-0 beállítása  
    }
}

void frissit(){
    MOTOR_FEL = motor_aktiv && motor_irany;
    MOTOR_LE = motor_aktiv && !motor_irany;
    RIASZTO_KI = riaszto_ki;
}

void nyomogombMegnyomva(){
    motor_aktiv = !motor_aktiv;
    if (motor_aktiv){
        motor_irany = !motor_irany;
    }
    frissit();
}

void vegallasAktiv(){
    motor_aktiv = false;
    frissit();
}

void riasztoKi(){
    if (!riaszto_ki){
        //az idozites idotartama alatt nem fogad ujabb jelet!
        riaszto_ki = true;
        riasztoki_idobelyeg = timeMillis();
        frissit();
    }
}

void riasztoIdozito(int32_t timeout) {
    if (riaszto_ki){
        uint32_t now = timeMillis();
        if (now - riasztoki_idobelyeg > timeout) {
            //riasztot ujra aktivalja
            riaszto_ki = false;
            frissit();
        }
    }
}