#include "../inc/rtc.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/pwr.h>
#include <stdio.h>
#include <stdbool.h>

static bool isInit = false;

void Rtc_init(uint32_t presetCounterVal)
{
    if(!isInit)
    {
        // enable clock access to rcc pwr 
        // and backup  
        rcc_periph_clock_enable(RCC_PWR);
        rcc_periph_clock_enable(RCC_BKP);

        // disable write protection 
        pwr_disable_backup_domain_write_protect();

        // Rtc clock settings
        // enable Low-Speed-Internal clk
        rcc_osc_on(RCC_LSI);
        while(!rcc_is_osc_ready(RCC_LSI));
        rcc_set_rtc_clock_source(RCC_LSI);

        // Prescaler settings :
        // Enter config mode, set 
        // prescaler and counter value 
        rtc_enter_config_mode();
        rtc_set_prescale_val(40000); // LSI is 40Khz
        rtc_set_counter_val(presetCounterVal);
        rtc_exit_config_mode();
        isInit = true;
    }
    
}

void Rtc_setCounter(uint32_t counterValue)
{
    rtc_enter_config_mode();
	rtc_set_prescale_val(40000); // LSI is 40Khz
	rtc_set_counter_val(counterValue);
	rtc_exit_config_mode();
}

void Rtc_getTimeStr(char *time)
{
    uint32_t epoch = rtc_get_counter_val();
    int days = epoch/86400;
    int hours = epoch/3600;
    int minute = (epoch % 3600)/60;
    int second = (epoch % 60);
    if(isInit)
    {
        sprintf(time, "DD:HH:MM:SS--%d:%d:%d:%d",days, hours, minute, second);
    }else{
        sprintf(time, "DD:HH:MM:SS. Error, Rtc not initalized");
    }
}

uint32_t Rtc_getTime(void)
{
    return rtc_get_counter_val();
}