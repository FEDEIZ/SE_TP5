#include "clock.h"
#include <string.h>

#define SIZE_TIME 6
#define INIT_VALUE 0
#define SEC_UNITS 5
#define SEC_TENS 4
#define MIN_UNITS 3
#define MIN_TENS 2
#define HOURS_UNITS 1
#define HOURS_TENS 0
#define MAX_TENS_VALUES 6
#define MAX_UNITS_VALUES 10
#define MAX_HOURS_TENS_VALUE 2  // 24 hrs
#define MAX_HOURS_UNITS_VALUE 4 // 24 hrs
#define ALARM_SIZE 4

struct clock_s{
    bool valid;
    bool alarm_enabled;
    uint16_t ticksPerSec;
    uint16_t ticksCount;
    uint8_t time[SIZE_TIME];
    uint8_t alarm[ALARM_SIZE];
    clock_event_t event_handler;
};

static struct clock_s instances;

static const uint8_t BCD_LIMITS[] = {MAX_TENS_VALUES, MAX_UNITS_VALUES};

void IncrementTime(clock_t clock){
    for(int index = sizeof(clock->time) - 1; index>=0; index--){
        clock->time[index]++;

        if(clock->time[index]> BCD_LIMITS[index%2]){
            clock->time[index] = INIT_VALUE;
        } else {
            break;
        }
    }
    // if(clock->time[HOURS_TENS] == MAX_HOURS_TENS_VALUE){
    //   if(clock->time[HOURS_UNITS] == MAX_HOURS_UNITS_VALUE){
    //     clock->time[HOURS_TENS] = INIT_VALUE;
    //     clock->time[HOURS_UNITS] = INIT_VALUE;
    //   }  
    // }
}

//pongo la hora en cero
clock_t createClock(uint16_t ticksPerSec, clock_event_t event_handler){
    instances.valid = false;
    instances.alarm_enabled = false;
    instances.ticksPerSec = ticksPerSec;
    instances.ticksCount = INIT_VALUE;
    instances.event_handler = event_handler;
    memset(instances.time, INIT_VALUE, SIZE_TIME);
    return &instances;
}

bool getTimeClock(clock_t clock, uint8_t * time, uint8_t size){
    memcpy(time,clock->time,  size);
    return clock->valid;
    
    
}

void configClock(clock_t clock, uint8_t const * const time, uint8_t size){
    
     memcpy( clock->time,time, size);
     clock->valid = true;
}

void tickClock(clock_t clock){
    clock->ticksCount++;
    
     if(clock->ticksCount == clock->ticksPerSec){
        
         clock->ticksCount = INIT_VALUE;
     //    IncrementTime(clock); no me funciona

      clock->time[SEC_UNITS]++;  //los segundos estan en el 5to lugar del arreglo       
        if (clock->time[SEC_UNITS] == 10){
            clock->time[SEC_UNITS] = 0;
            clock->time[SEC_TENS]++;
            
            if(clock->time[SEC_TENS] == 6){
                clock->time[SEC_UNITS] = 0;
                clock->time[SEC_TENS] = 0;
                clock->time[MIN_UNITS]++;

                if(clock->time[MIN_UNITS] == 10){
                    clock->time[SEC_UNITS] = 0;
                    clock->time[SEC_TENS] = 0;
                    clock->time[MIN_UNITS]= 0;
                    clock->time[MIN_TENS]++;

                    if(clock->time[MIN_TENS] == 6){
                        clock->time[SEC_UNITS] = 0;
                        clock->time[SEC_TENS] = 0;
                        clock->time[MIN_UNITS]= 0;
                        clock->time[MIN_TENS] = 0;
                        clock->time[HOURS_UNITS]++;

                        if(clock->time[HOURS_UNITS] == 10){
                            clock->time[SEC_UNITS] = 0;
                            clock->time[SEC_TENS] = 0;
                            clock->time[MIN_UNITS]= 0;
                            clock->time[MIN_TENS] = 0;
                            clock->time[HOURS_UNITS]=0;
                            clock->time[HOURS_TENS]++;
                        }
                    }
                }
                
                bool activate = true;
                // bool activate = (clock->time[SECONDS_TENS] == 0) && (clock->time[SECONDS_UNITS] == 0);
                for(int index=0; index<ALARM_SIZE; index++){
                    if(clock->alarm[index] != clock->time[index]){
                        activate = false;
                        break;
                    }
                    if(activate && clock->alarm_enabled){
                        clock->event_handler(clock, true); // llamo a la funcion de callback
                    }
                }
            
            }
        }

        

     }
}

void clockSetUpAlarm(clock_t clock, uint8_t const * const alarm, uint8_t size){
    memcpy( clock->alarm, alarm, size);
    clock->alarm_enabled = true;
}

bool clockGetAlarm(clock_t clock, uint8_t * alarm, uint8_t size){
    memcpy(  alarm, clock->alarm, size);
    return clock->alarm_enabled;
}

bool clockToggleAlarm(clock_t clock){
    clock->alarm_enabled = !clock->alarm_enabled;
    return clock->alarm_enabled;
}