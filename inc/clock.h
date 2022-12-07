#include <stdint.h>
#include <stdbool.h>

typedef struct clock_s * clock_t;

typedef void (*clock_event_t)(clock_t clock, bool state); // puntero a una funcion

clock_t createClock(uint16_t ticksPerSec, clock_event_t event_handler);

bool getTimeClock(clock_t clock, uint8_t * time, uint8_t size);

void configClock(clock_t clock, uint8_t const * const time, uint8_t size);

void tickClock(clock_t clock);

void clockSetUpAlarm(clock_t clock, uint8_t const * const alarm, uint8_t size);

bool clockGetAlarm(clock_t clock, uint8_t * alarm, uint8_t size);

bool clockToggleAlarm(clock_t clock);