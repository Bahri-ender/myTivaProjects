#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "inc/hw_memmap.h"



void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    int saat = SysCtlClockGet();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); // pin 1, 2, 3 output

    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    uint32_t m = 1000000;

    while (1) {
       if(m!=0|m!=4294967296){
         GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
         while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0) {//pf0
            m = m+100000;
            if (m != 4294967296) {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
                SysCtlDelay(m);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
                SysCtlDelay(m);
            }
            else if (m==4294967296) {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
                m=m-100000;
                SysCtlDelay(m);
            }
        }
         while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0) {
            m = m-100000;
            if (m != 0) {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
                SysCtlDelay(m);
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
                SysCtlDelay(m);
            }
            else if (m==0) {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
                m=m+100000;
                SysCtlDelay(m);
            }
        }
         SysCtlDelay(m);
         GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
         SysCtlDelay(m);
     // HWREG(0x400253fc)=2;
    }
}
}
