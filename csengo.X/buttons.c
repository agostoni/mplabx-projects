#include "buttons.h"
#include "system.h"

typedef struct _listener{
    volatile unsigned char* port;
    void(*callback)();
    /**
     1 ha felfutó, 0 ha lefutó élre aktív
     */
    unsigned dir : 1;
    /**
     Igaz ha a gomb jelenleg aktív*/
    unsigned active : 1;
    unsigned char mask;
    uint32_t latestActivation;
} Listener;

bool activated(Listener * l);

Listener listeners[5];
unsigned char listenerCount = 0;

void registerListener(volatile unsigned char* port,volatile unsigned char* portConfig,int pin,int activeOn, void(*callback)()){
    if (listenerCount < sizeof(listeners)/sizeof(Listener)){
        pin = pin % 8;
        Listener* l = listeners + listenerCount;
        l->callback = callback;
        l->port = port;
        l->dir = activeOn;
        l->mask = (0x01<<pin);
        l->latestActivation = 0;
        l->active = false;
        //configurálás input portként
        (*portConfig) |= (l->mask);
        listenerCount++;
    }
    //TODO error?
}

void monitorButtons(){
    for (unsigned i=0; i<listenerCount;i++){
        Listener *l = listeners+i;
        if (activated(l)){
            l->callback();
        }
    }
}

bool activated(Listener* l){
    uint32_t now = timeMillis();
    
    bool portValue = (*(l->port) & l->mask) != 0;
    bool active = portValue == l->dir;
    if (active){
        if (!l->active){
            l->active = true;
            l->latestActivation = now;
            return true;
        }
    } else {
        if (l->active && now > l->latestActivation + 100){
            l->active = false;
        }
    }
    return false;
}