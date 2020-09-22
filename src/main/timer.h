#ifndef TIMER_H
#define TIMER_H

#include "mcu_support_package/inc/stm32f4xx.h"

#define REFERENCE_FREQUENCY_HZ 1000000

typedef struct Clock
{
/*
private:
*/
	uint32_t startTime;
	uint32_t counter;
/*
public:
*/	
	uint32_t (*getTime)(struct Clock *clock);
	void (*reset)(struct Clock *clock);
} Clock;

void initTimer(const uint32_t timeout_ms);
Clock *initClock(const uint32_t timeout_ms);

uint32_t getClockTime(Clock *clock);
void resetClock(Clock *clock);

#endif //TIMER_H
