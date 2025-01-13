#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/hibernate.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"

#define LED_PIN GPIO_PIN_1
#define LED_PORT GPIO_PORTF_BASE

void ConfigureHibernation(void);

int main(void)
{
    // Sistem saatini 40 MHz olarak ayarla
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Port F'yi etkinle�tir ve LED pinini ayarla
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(LED_PORT, LED_PIN);

    // LED'i bir kez yak
    GPIOPinWrite(LED_PORT, LED_PIN, LED_PIN);
    SysCtlDelay(SysCtlClockGet() / 3); // 1 saniye bekle

    // LED'i kapat
    GPIOPinWrite(LED_PORT, LED_PIN, 0);

    // Hibernation modunu yap�land�r
    ConfigureHibernation();

    // 5 saniye sonra uyand�rmay� ayarla
    HibernateRTCSet(0);
    HibernateRTCEnable();
    HibernateRTCMatchSet(0, 5);

    // Hibernation moduna ge�
    HibernateRequest();

    // Hibernation'dan ��k�� sonras� LED'i tekrar yak
    while (1)
    {
        GPIOPinWrite(LED_PORT, LED_PIN, LED_PIN);
        SysCtlDelay(SysCtlClockGet() / 3);
        GPIOPinWrite(LED_PORT, LED_PIN, 0);
        SysCtlDelay(SysCtlClockGet() / 3);
    }
}

void ConfigureHibernation(void)
{
    // Hibernation modunu etkinle�tir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);

    // Hibernation modunun haz�r olmas�n� bekle
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_HIBERNATE))
    {
    }

    // Hibernation modunu yap�land�r ve etkinle�tir
    HibernateEnableExpClk(SysCtlClockGet());
    HibernateClockConfig(HIBERNATE_CLOCK_SEL_RAW);

    // Wake pinlerini ve RTC'yi kullanmak i�in yap�land�r
    HibernateWakeSet(HIBERNATE_WAKE_PIN | HIBERNATE_WAKE_RTC);

    // Hibernation moduna girerken kay�tl� bilgilerin korunmas�n� etkinle�tir
    HibernateDataSet(0, 0x1234ABCD);
}
