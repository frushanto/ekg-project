#include <msp430.h>				
#include <driverlib.h>

/* Function declarations */
void initWatchdog(void);
void initGPIO(void);
void initClocks(void);

/* Function definitions */
void main(void){
    initWatchdog();
    initGPIO();
    initClocks();

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

/* 
 * Setup Clocking 
 * ACLK = aux clock, slow & low-power;
 * REFO = 32768 Hz, trimmed to 3,5%;
 * */
void initClocks() {
	UCS_initClockSignal (
		UCS_ACLK,				// Configure ACLK
		UCS_REFOCLK_SELECT, 	// Set to REFO source
		UCS_CLOCK_DIVIDER_1		// Set clock divider to 1
	);
}
