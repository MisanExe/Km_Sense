#ifndef _RTC_
#define _RTC_
#include <stdint.h>

void Rtc_init(uint32_t presetCounterVal);
void Rtc_setCounter(uint32_t counterValue);
void Rtc_getTimeStr(char *time);
uint32_t Rtc_getTime(void);
#endif