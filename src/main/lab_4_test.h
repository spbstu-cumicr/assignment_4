#pragma once

#include "stm32f4xx.h"
#include "stdio.h"

uint8_t Lab4_Test_ini(char *name);
uint32_t read_flag(uint8_t *data);

uint8_t initTest(const char *lastName);
void getDecision(uint8_t *decision);
uint32_t getChecksum(void);
