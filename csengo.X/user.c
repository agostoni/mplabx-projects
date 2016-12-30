/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include "user.h"
#include "system.h"
#include "buttons.h"
#include<stdio.h>

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


/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/**
 * Villogtat egy életjel LED-et a parameterben megadott periodusidovel.
 * 
 * @param period Az életjel led villogási periodusa millisekundumban
 */
void alapjel(uint32_t period);
/*
 * Meghivodik ha a csengo nyomogombot megnyomtak.
 */
void csengo();
void nyit();
void zar();
void novel();
void reset();
void helyi_ertek_valtas();
void status();

int CODE[4] = {5u,4u,3u,2u};

static uint32_t utolso_csengo_ib;
void ciklus (uint32_t eltelt_ido);

typedef enum Allapot_tag {
    ALAP,
    OLVAS,
    NYIT
} Allapot;

Allapot allapot = ALAP;

uint8_t szamjegy;
uint8_t kod[4];
uint8_t he;
uint32_t csengobe_ib;
/******************************************************************************/
/* Interfész függvények                                                       */
/******************************************************************************/
void initApp(void) {
    //PORTA és PORTB konfigurálása dgitálisként
    ANSEL = 0;
    ANSELH = 0;
    HEART_BEAT_TRIS = OUTPUT;
    ZAR_TRIS = OUTPUT;
    ST_1_TRIS = OUTPUT;
    ST_2_TRIS = OUTPUT;
    CSENGO_TRIS = OUTPUT;
    registerListener(&PORTA,&TRISA,7,1,csengo);
    utolso_csengo_ib = csengobe_ib = hb_last_timestamp = timeMillis();
    
    reset();
    
}

void foCiklus(void) {
    status();
    alapjel(1000);
    monitorButtons();
    uint32_t now = timeMillis();
    ciklus (now - utolso_csengo_ib);
    
    if (CSENGO == 1 && (now - csengobe_ib) > CSENGO_TIMEOUT){
        CSENGO = 0;
    }
}

/******************************************************************************/
/* "Privát" függvények                                                        */
/******************************************************************************/


void alapjel(uint32_t period) {
    uint32_t now = timeMillis();
    if (now - hb_last_timestamp > period) {
        hb_last_timestamp = now;
        hb_state = !hb_state; //state értéke 0 és 1 között változik
        HEART_BEAT = hb_state; // RD-0 beállítása  
    }
}

void ciklus(uint32_t eltelt_ido){
  switch (allapot){
        case ALAP : 
            //itt nincs mit csinálni, várunk a cseng?re
            break;
        case OLVAS :
            if (eltelt_ido > (SZAMJEGY_VALTO_TIMEOUT * 2)){
                if (he >= 3){
                    //nyitunk
                    nyit();
                    he = 0;
                    utolso_csengo_ib = timeMillis();
                } else {
                    //vissza alap állapotba
                    reset();
                }
            } else if (eltelt_ido > SZAMJEGY_VALTO_TIMEOUT && he < 3){
                helyi_ertek_valtas();
                utolso_csengo_ib = timeMillis();
            }
            break;
        case NYIT :
          if (eltelt_ido > NYIT_TIMEOUT){
              reset();
          }
          break;
        default :
            //egyéb esetekben a cseng? gomb nem csinál semmit
            break;   
    }
  
}

void helyi_ertek_valtas(){
    if (kod[he] == CODE[he]){
        he++;
        log("he: %d\n",he);
    } else {
        reset();
    }
}

void status(){
    switch(he ){
        case 0:
            ST_1 = 0;
            ST_2 = 0;
            break;
        case 1:
            ST_1 = 0;
            ST_2 = 1;
            break;
        case 2:
            ST_1 = 1;
            ST_2 = 0;
            break;
        case 3:
            ST_1 = 1;
            ST_2 = 1;
            break;
    }
}

void zar (){
    ZAR = 0;
    allapot = ALAP;
}

void nyit (){
    allapot = NYIT;
    ZAR = 1;
}
void reset(){
    kod[0]=kod[1]=kod[2]=kod[3] = 0;
    he = 0;
    zar();
     status();
}

void novel(){
    if (he >=0 && he <4){
        kod[he]++;
        if (kod[he] > 9){
            kod[he] = 0;
        }
        log("Novel %d->%d\n",he,kod[he]);
    }
}

void csengo(){
    log("csengo\n");
    uint32_t now = timeMillis();
    switch (allapot){
        case ALAP : 
            reset();
            novel();
            allapot = OLVAS;
            if (CSENGO == 0){
               CSENGO = 1;
               csengobe_ib = now;
            }
            break;
        case OLVAS :
           
            novel();
            break;
        default :
            //egyéb esetekben a cseng? gomb nem csinál semmit
            break;   
    }
    utolso_csengo_ib = now;
}
