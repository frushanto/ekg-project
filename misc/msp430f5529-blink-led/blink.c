#include <msp430.h>				
#include <driverlib.h>

/* Function declarations */
void initWatchdog(void);
void initGPIO(void);

/* Function definitions */
void main(void){
    initWatchdog();
    GPIO_
	P1DIR |= 0x01;					// configure P1.0 as output

	volatile unsigned int i;		// volatile to prevent optimization

	while(1)
	{
		P1OUT ^= 0x01;				// toggle P1.0
		for(i=50000; i>0; i--);     // delay
	}
}

void initWatchdog(void) {
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
}

void initGPIO(void) {

}
