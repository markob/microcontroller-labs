#ifndef _UTILS_H
#define _UTILS_H

#include "types.h"

void InitUtils(void);
void Delay(uint16_t msDelay);

void StartTimer(uint16_t msDelay);
uint8_t CheckTimer();

#endif
