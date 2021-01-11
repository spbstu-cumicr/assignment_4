#pragma once

#include <stdint.h>

const uint8_t *getLedColourList(const uint8_t variant);
uint8_t ledColourSize(void);

const uint32_t *getDelayList(const uint8_t variant);
uint8_t delaySize(void);
