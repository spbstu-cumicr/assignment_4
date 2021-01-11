#include "lab_4_test.h"

static uint32_t checksum = 0;

uint8_t initTest(const char *lastName)
{
    return Lab4_Test_ini((char *)lastName);
}

void getDecision(uint8_t *decision)
{
    checksum = read_flag(decision);
}

uint32_t getChecksum(void)
{
    return checksum;
}
