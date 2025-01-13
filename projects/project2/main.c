#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

void gpiokesmesi(void) {
    int hangipin = GPIOIntStatus(GPIO_PORTF_BASE, true);
    if (hangipin==16) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
    else if (hangipin==1) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
    }
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_0);
}

void main(void)
{ // Int ile baþlýyosa global ayar
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    IntMasterEnable();
    IntEnable(INT_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); // pin 1, 2, 3 output

    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4|GPIO_INT_PIN_0);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_INT_PIN_4|GPIO_INT_PIN_0, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, gpiokesmesi);

    while(1) {

    }
}
