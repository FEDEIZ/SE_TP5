#include "clock.h"
#include <string.h>

#define SIZE_TIME 6
#define INIT_VALUE 0
#define SEC_UNITS 5

struct clock_s{
    bool valid;
    uint16_t ticksPerSec;
    uint16_t ticksCount;
    uint8_t time[SIZE_TIME];
};

static struct clock_s instances;

//pongo la hora en cero
clock_t createClock(uint16_t ticksPerSec){
    return NULL;
    // instances.valid = false;
    // instances.ticksPerSec = ticksPerSec;
    // instances.ticksCount =0;
    // memset(instances.time, INIT_VALUE, SIZE_TIME);
    // return &instances;
}

bool getTimeClock(clock_t clock, uint8_t * time, uint8_t size){
    memset(time,0,size);
    return false;
    // memcpy(time, clock->time, size);
    // return clock->valid;
}

void configClock(clock_t clock, uint8_t const * const time, uint8_t size){
    memcpy(clock->time, time, size);
    clock->valid = true;
}

void tickClock(clock_t clock){
    clock->ticksCount++;
    if(clock->ticksCount == clock->ticksPerSec){
        clock->ticksCount =0;
        clock->time[SEC_UNITS]++;//los segundos estan en el 5to lugar del arreglo
        if (clock->time[SEC_UNITS] == 10){
            clock->time[SEC_UNITS] = 0;
            clock->time[4]++;
        }
    }
}