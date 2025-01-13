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

    // Port F'yi etkinleþtir ve LED pinini ayarla
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(LED_PORT, LED_PIN);

    // LED'i bir kez yak
    GPIOPinWrite(LED_PORT, LED_PIN, LED_PIN);
    SysCtlDelay(SysCtlClockGet() / 3); // 1 saniye bekle

    // LED'i kapat
    GPIOPinWrite(LED_PORT, LED_PIN, 0);

    // Hibernation modunu yapýlandýr
    ConfigureHibernation();

    // 5 saniye sonra uyandýrmayý ayarla
    HibernateRTCSet(0);
    HibernateRTCEnable();
    HibernateRTCMatchSet(0, 5);

    // Hibernation moduna geç
    HibernateRequest();

    // Hibernation'dan çýkýþ sonrasý LED'i tekrar yak
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
    // Hibernation modunu etkinleþtir
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);

    // Hibernation modunun hazýr olmasýný bekle
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_HIBERNATE))
    {
    }

    // Hibernation modunu yapýlandýr ve etkinleþtir
    HibernateEnableExpClk(SysCtlClockGet());
    HibernateClockConfig(HIBERNATE_CLOCK_SEL_RAW);

    // Wake pinlerini ve RTC'yi kullanmak için yapýlandýr
    HibernateWakeSet(HIBERNATE_WAKE_PIN | HIBERNATE_WAKE_RTC);

    // Hibernation moduna girerken kayýtlý bilgilerin korunmasýný etkinleþtir
    HibernateDataSet(0, 0x1234ABCD);
}
