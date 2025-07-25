#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "../inc/uart.h"
#include <libopencm3/stm32/rcc.h>

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
	
	UART_writeBytes(&uart2_handle, text);
	while(1)
	{
		for(int i = 0; i <1000000; i++)
		{
			__asm__("nop");
		}
		
		echo(&uart2_handle);

		

	}
}
