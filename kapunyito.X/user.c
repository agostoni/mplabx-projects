/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include "user.h"
#include "system.h"
#include "buttons.h"
/******************************************************************************/
/* �llapot                                                           */
/******************************************************************************/
/*
 * Az utols� HB LED �llapotv�lt�s id?b�lyege.
 */
uint32_t hb_last_timestamp = 0;
/**
 * Heart beat LED �llapota
 */
bool hb_state = false;

/*Igaz, ha a motor jelenleg j�r*/
bool motor_aktiv = false;
/*
 * A kapunyit� motor ir�nya.
 * false -> irany le
 */
bool motor_irany = false;
/*
 * Az utols� �rv�nyes riszat�_ki jel id?b�lyege
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
 * Villogtat egy �letjel LED-et a parameterben megadott periodusidovel.
 * 
 * @param period Az �letjel led villog�si periodusa millisekundumban
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
 * Megh�v�dik, ha a riaszto ki jelet detekt�lt a rendszer. Kikapcsolja a 
 * riaszt�t, �s elind�tja az id?zit?t.
 */
void riasztoKi();
/*
 * �jra enged�lyezi a riaszt�t a parameterben megadott ido elteltevel.
 * 
 * @param timeout A kivant ido milliszekundumban
 */
void riasztoIdozito(int32_t timeout);


/******************************************************************************/
/* Interf�sz f�ggv�nyek                                                       */
/******************************************************************************/
void initApp(void) {
    //PORTA �s PORTB konfigur�l�sa dgit�lisk�nt
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
/* "Priv�t" f�ggv�nyek                                                        */
/******************************************************************************/


void alapjel(int32_t period) {
    uint32_t now = timeMillis();
    if (now - hb_last_timestamp > period) {
        hb_last_timestamp = now;
        hb_state = !hb_state; //state �rt�ke 0 �s 1 k�z�tt v�ltozik
        HEART_BEAT = hb_state; // RD-0 be�ll�t�sa  
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