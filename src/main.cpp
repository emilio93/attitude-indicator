#include "msp.h"
#include "driverlib.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
	while (1) {}
}
