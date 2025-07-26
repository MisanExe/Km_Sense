#include "../inc/uart.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include "../inc/buffer.h"

#define USER_BAUD_RATE 115200
#define USER_NVIC_USART2 38
static ringBufferHandle_t dataBuffer;

static void _getSelectedUart(uint32_t uart, uart_handle_t *handle);
static uint32_t _getRccGpio(uint32_t uart);
static uint32_t _getRccUart(uint32_t uart);
static uint32_t _getGpioPort(uint32_t uart);
static uint32_t _getInterruptId(uint32_t uart);

static ringBufferHandle_t uart_databuffer[UART_IN_USE_MAX_INDEX];

// Configure for UART 1, 2 OR 3 only
void UART_init(uint32_t uart, uart_handle_t *handle)
{

    _getSelectedUart(uart, handle);
    uint32_t uartGpioRcc = _getRccGpio(uart);
    uint32_t uartRcc = _getRccUart(uart);
    uint32_t uartGpioPort = _getGpioPort(uart);
    uint32_t uartInterruptId = _getInterruptId(uart);


    // gpio settings
    rcc_periph_clock_enable(uartGpioRcc);
    gpio_clear(uartGpioPort, handle->pTx | handle->pRx);
    gpio_set_mode(uartGpioPort, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, handle->pTx);
    gpio_set_mode(uartGpioPort, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, handle->pRx);

    // UART settings
    rcc_periph_clock_enable(uartRcc);
    usart_disable(uart);
    usart_set_flow_control(uart, USART_FLOWCONTROL_NONE);
    usart_set_databits(uart, 8);
    usart_set_baudrate(uart, USER_BAUD_RATE);
    usart_set_parity(uart, 0);
    usart_set_stopbits(uart, 1);
    usart_set_mode(uart, USART_MODE_TX_RX);

    // add  interrupts
    usart_enable_rx_interrupt(uart);
    // usart_enable_tx_interrupt(USART2);
    nvic_enable_irq(uartInterruptId);
    
    usart_enable(uart);

    // init buffer
    RingBuffer_init(&uart_databuffer[handle->uartIndex]);
}

// Write to
void UART_write(uart_handle_t *handle, uint8_t data)
{   
    usart_send_blocking(handle->uart, (uint16_t )data);
}

void UART_writeBytes(uart_handle_t *handle, char *data)
{
    for(int i = 0; i < (int)strlen(data); i++)
    {
        usart_send_blocking(handle->uart, (uint16_t)data[i]);
    }
}
void UART_println(uart_handle_t *handle, char *data)
{
    UART_writeBytes(handle, data);
    UART_writeBytes(handle, "\n\r");
}


void echo(uart_handle_t *handle)
{

    char data = (char)RingBuffer_read(&uart_databuffer[handle->uartIndex]);

    if (data != '\0')
    {
        usart_send_blocking(handle->uart, data);
    }
}

/////////////////////////////////
////// Interrupt handling
/////////////////////////////////

// Uart 2 interrupt handler
void usart2_isr(void)
{
    const bool overrun_occurred = usart_get_flag(USART2, USART_FLAG_ORE) == 1;
    const bool dataAvailable = usart_get_flag(USART2, USART_FLAG_RXNE) == 1;
    if(dataAvailable || overrun_occurred)
    {
        RingBuffer_write(&uart_databuffer[UART2_INDEX], (uint8_t)usart_recv(USART2));
    }
}


//////////////////////////////
////// Helper functions
//////////////////////////////

static void _getSelectedUart(uint32_t uart, uart_handle_t *handle)
{
    if (uart == USART1)
    {
        handle->pTx = GPIO_USART1_TX;
        handle->pRx = GPIO_USART1_RX;
        handle->uartIndex = UART1_INDEX;
    }else if (uart == USART2)
    {
        handle->pTx = GPIO_USART2_TX;
        handle->pRx = GPIO_USART2_RX;
        handle->uartIndex = UART2_INDEX;
    }else if (uart == USART3)
    {
        handle->pTx = GPIO_USART3_TX;
        handle->pRx = GPIO_USART3_RX;
        handle->uartIndex = UART3_INDEX;
    }
    handle->uart = uart;
}
static uint32_t _getRccGpio(uint32_t uart){


    if (uart == USART1)
    {
        return  RCC_GPIOA;
    }else if (uart == USART2)
    {
        return RCC_GPIOB;

    }else if (uart == USART3)
    {   
        return  RCC_GPIOC;
    }
    return 0;
}

static uint32_t _getRccUart(uint32_t uart){

    if (uart == USART1)
    {
        return  RCC_USART1;
    }else if (uart == USART2)
    {
        return RCC_USART2;

    }else if (uart == USART3)
    {   
        return  RCC_USART3;
    }
    return 0;
}

static uint32_t _getGpioPort(uint32_t uart)
{
    if (uart == USART1)
    {
        return GPIOB;
    }else if (uart == USART2)
    {
        return GPIOA;

    }else if (uart == USART3)
    {   
        return  GPIOD;
    }
    return 0;
}

static uint32_t _getInterruptId(uint32_t uart)
{
        if (uart == USART1)
    {
        return NVIC_USART1_IRQ;

    }else if (uart == USART2)
    {
        return NVIC_USART2_IRQ;

    }else if (uart == USART3)
    {   
        return NVIC_USART3_IRQ;;
    }
    return 0;
}