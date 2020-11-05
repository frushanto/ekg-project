#include <msp430.h>				
#include <driverlib.h>

/* Function declarations */
void initWatchdog(void);
void initGPIO(void);

/* Function definitions */
void main(void){
    initWatchdog();
    initGPIO();

    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

	volatile unsigned int i;		// volatile to prevent optimization

	for (i = 50000; i > 0; i--);
	for (i = 50000; i > 0; i--);

	while(1) {
	    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN7);
		for (i = 10000; i > 0; i--);     // delay
	}
}

// Configure Watchdog Timer
void initWatchdog(void) {
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
}

// Configure GPIO ports/pins
void initGPIO(void) {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);
}
