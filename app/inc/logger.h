#ifndef __LOGGER_
#define __LOGGER_
#include <string.h>
#include "../inc/uart.h"
#include "../inc/rtc.h"

typedef enum 
{
    INFO,
    WARNING,
    ERROR,
    DEBUG,
}logLevel_enum;

void Logger_levelToStr(logLevel_enum level, char *levelStr);
void Logger_init(void);
void Logger_log(logLevel_enum level, char *msg);

#endif