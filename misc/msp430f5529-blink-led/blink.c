#include <msp430.h>				
#include <driverlib.h>

/* User Defines */
#define DELAY_500K 500000

/* Function declarations */
void initWatchdog(void);
void initGPIO(void);
void initClocks(void);

/* Function definitions */
void main(void){
    /* Init MSP430 */
    initWatchdog();
    initGPIO();
    initClocks();

    /* Setup 2 LEDs */
    //volatile unsigned int i = 0;
    //for (i = 0; i < 3; i++) {
        //GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
        //GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        //_delay_cycles(DELAY_500K);
        //GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
        //GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
        //_delay_cycles(DELAY_500K);
    //}

    //GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
    //_delay_cycles(DELAY_500K);

	/* Global Interrupt Enable */
	__bis_SR_register(GIE);     // Enter interrupt
	//__enable_interrupts();		// Compiler enables interrupts
	__no_operation();     	    // For debugger

	/* Toggle LEDs */
	while(1) {
	    //GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        //GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN7);
		//_delay_cycles(DELAY_500K);
	}
}

// Configure Watchdog Timer
void initWatchdog(void) {
	WDT_A_hold(WDT_A_BASE);
}

// Configure GPIO ports/pins
void initGPIO(void) {
    //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    //GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);

    // Set input and enable P1.1 as INT
	// Configure pin as an input
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
	// What edge transition will trigger an interrupt (low-to-high or high-to-low)
	//GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
	// Clears the IFG bit associated with P1.1
	//GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);

	GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
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

/* Interrupt Service Routines */
#pragma vector = PORT1_VECTOR
__interrupt void pushbutton_ISR(void) {
    switch(__even_in_range(PORT1_VECTOR, 0x10)) {
        case 0x00: break;   // None
        case 0x02: break;   // Pin 0
        case 0x04:          // Pin 1
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);    // Toggle LED
            break;
        case 0x06: break;   // Pin 2
        case 0x08: break;   // Pin 3
        case 0x0A: break;   // Pin 4
        case 0x0C: break;   // Pin 5
        case 0x0E: break;   // Pin 6
        case 0x10: break;   // Pin 7
        default: _never_executed();
    }
}



