#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "../inc/uart.h"
#include "../inc/rtc.h"
#include "../inc/logger.h"
#include <libopencm3/stm32/rcc.h>
#include <stdio.h>

static void rcc_setup(void)
{
	// set clock to run at 48Mhz
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}
int main(void)
{
	rcc_setup();
	
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_set_mode(GPIOA,
	GPIO_MODE_OUTPUT_10_MHZ,
	GPIO_CNF_OUTPUT_PUSHPULL,
	GPIO7
	);
	uart_handle_t uart2_handle;
	UART_init(USART2, &uart2_handle);

	char text[] = "I hope this works out.\n\rLost my job, on a mission to retain my dignity\n\n\r";

	Rtc_init(0);
	Logger_init();

	UART_writeBytes(&uart2_handle, text);

	uint32_t previous = Rtc_getTime();
	while(1)
	{
		// for(int i = 0; i <1000000; i++)
		// {
		// 	__asm__("nop");
		// }
		

		
		if( (Rtc_getTime() - previous) % 60 > 1 )
		{
			echo(&uart2_handle);
			char buffer[50];
			Logger_log(INFO, "Hello from stm32F1. we're live now, stay tuned");


			// update 
			previous = Rtc_getTime();
		}

	}
}
