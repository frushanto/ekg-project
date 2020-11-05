#include <msp430.h>				
#include <driverlib.h>

/* Function declarations */
void initWatchdog(void);
void initGPIO(void);

/* Function definitions */
void main(void){
    initWatchdog();
    initGPIO();


	volatile unsigned int i;		// volatile to prevent optimization

	while(1)
	{
		//P1OUT ^= 0x01;				// toggle P1.0
	    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
		for(i=10000; i>0; i--);     // delay
	}
}

// Configure Watchdog Timer
void initWatchdog(void) {
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
}

// Configure GPIO ports/pins
void initGPIO(void) {
	//P1DIR |= 0x01;					// configure P1.0 as output
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
}
