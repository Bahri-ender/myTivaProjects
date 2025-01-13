#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"

void UART_Init(void);

int main(void)
{
    // Sistem saatini 40 MHz olarak ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // UART mod�l�n� ba�lat
    UART_Init();

    // Floating-point de�i�kenler
    float num1 = 5.25;
    float num2 = 3.75;
    float sum, difference, product, quotient;

    // Floating-point i�lemleri
    sum = num1 + num2;
    difference = num1 - num2;
    product = num1 * num2;
    quotient = num1 / num2;

    // Sonu�lar� UART �zerinden g�nder
    char buffer[50];

    sprintf(buffer, "Sum: %.2f\r\n", sum);
    UARTCharPut(UART0_BASE, buffer);

    sprintf(buffer, "Difference: %.2f\r\n", difference);
    UARTCharPut(UART0_BASE, buffer);

    sprintf(buffer, "Product: %.2f\r\n", product);
    UARTCharPut(UART0_BASE, buffer);

    sprintf(buffer, "Quotient: %.2f\r\n", quotient);
    UARTCharPut(UART0_BASE, buffer);

    while (1)
    {
        // Ana d�ng�
    }
}

void UART_Init(void)
{
    // UART0 mod�l�n� etkinle�tir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // UART pinlerini yap�land�r
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // UART ayarlar�n� yap�land�r
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
