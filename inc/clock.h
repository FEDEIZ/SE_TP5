#include <stdint.h>
#include <stdbool.h>

typedef struct clock_s * clock_t;

clock_t createClock(uint16_t ticksPerSec);

bool getTimeClock(clock_t clock, uint8_t * time, uint8_t size);

void configClock(clock_t clock, uint8_t const * const time, uint8_t size);

void tickClock(clock_t clock);