#include "../inc/logger.h"
#include <libopencm3/stm32/usart.h>
#include <stdio.h>

static uart_handle_t uart_handle;
void Logger_levelToStr(logLevel_enum level, char *levelStr)
{
    switch (level)
    {
    case INFO:
        strcpy(levelStr, "INFO");
        break;
    case WARNING:
        strcpy(levelStr, "WARNING");
        break;
    case ERROR:
        strcpy(levelStr, "ERROR");
        break;
    case DEBUG:
        strcpy(levelStr, "DEBUG");
        break;
    default:
        strcpy(levelStr, "DEBUG");
        break;
    }
}
void Logger_init(void)
{
    // initalize rtc;
    Rtc_init(0);
    // initalize uart 
    UART_init(USART2, &uart_handle);
}
void Logger_log(logLevel_enum level, char *msg)
{
    char buffer[LOGGER_BUFFER_SIZE];
    char time[20];
    char levelStr[15];

    Rtc_getTimeStr(time);
    Logger_levelToStr(level, levelStr);
    sprintf(buffer, "%s:\t %s : %s", time, levelStr, msg);   
    UART_println(&uart_handle, buffer);
}
