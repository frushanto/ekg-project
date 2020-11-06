#include <msp430.h>				
#include <driverlib.h>
#include <std_types.h>

/* Function declarations */
void Init_Watchdog(void);
void Init_GPIO(void);
void Init_Clocks(void);
void EnableGlobalInterrupt(void);
void Init_UART(void);
void Test_UART(void);

/* Function definitions */
void main(void){
    /* Init MSP430 */
    Init_Watchdog();
    Init_GPIO();
    Init_Clocks();
    // TODO Timers
    // TODO init peripherals - ADC, UART etc.
    Init_UART();
    EnableGlobalInterrupt();

	/* Toggle LEDs */
	while(1) {
        Test_UART();
	    //GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        //GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN7);
		//DELAY500K;
	}
}

// Configure Watchdog Timer
void Init_Watchdog(void) {
	WDT_A_hold(WDT_A_BASE);
}

// Configure GPIO ports/pins
void Init_GPIO(void) {
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    //GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);

    // Set input and enable P1.1 as INT
	// Configure pin as an input
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

    /*** LEAVE COMMENTED OUT BEGIN ***/
    // What edge transition will trigger an interrupt (low-to-high or high-to-low)
	//GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
	// Clears the IFG bit associated with P1.1
	//GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    /*** LEAVE COMMENTED OUT END ***/

	GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
}

/* 
 * Setup Clocking 
 * ACLK = aux clock, slow & low-power;
 * REFO = 32768 Hz, trimmed to 3,5%;
 * */
void Init_Clocks() {
	UCS_initClockSignal (
		UCS_ACLK,				// Configure ACLK
		UCS_REFOCLK_SELECT, 	// Set to REFO source
		UCS_CLOCK_DIVIDER_1		// Set clock divider to 1
	);
}

/* Global Interrupt Enable */
void EnableGlobalInterrupt() {
    __bis_SR_register(GIE);     // Enter interrupt
    __no_operation();           // For debugger
}

    /*
     * EUSCI_A_UART_enable() enables the EUSI_A_UART and the module
     * is now ready for transmit and receive. It is recommended to
     * initialize the EUSI_A_UART via EUSCI_A_UART_init(), enable
     * the required interrupts and then enable EUSI_A_UART via
     * EUSCI_A_UART_enable().
     * */
void Init_UART() {
    USCI_A_UART_initParam uart_cfg;

    /* Baudrate = 9600, clock freq = ACKL = 32kHz
     * UCBRx = 3, UCBRFx = 0, UCBRSx = 2, UCOS16 = 0
     *
     * From: Table 39-5. Recommended Settings for Typical Crystals and Baud Rates at Page 1039/1189 User guide.
     * For more information about baudrate setting see 39.3.10 Setting a Baud Rate at page 1036 User Guide
     */
    uart_cfg.selectClockSource = USCI_A_UART_CLOCKSOURCE_ACLK;
    uart_cfg.clockPrescalar = 3; // Table 36-4, p.952 User's Guide
    uart_cfg.firstModReg = 0;
    uart_cfg.secondModReg = 3;
    uart_cfg.parity = USCI_A_UART_NO_PARITY;
    uart_cfg.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    uart_cfg.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    uart_cfg.uartMode = USCI_A_UART_MODE;
    uart_cfg.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    /* Configure GPIO pins for UART
     * P3.4/UCA0RXD/UCA0SOMI
     * P3.3/UCA0TXD/UCA0SIMO
     * */

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN4); // Rx Pin
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3,GPIO_PIN3); // Tx Pin

    USCI_A_UART_init(USCI_A0_BASE, &uart_cfg);
    USCI_A_UART_enable(USCI_A0_BASE);
    USCI_A_UART_clearInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
}

void Test_UART() {
    volatile uint8_t receive_data = 0;
    uint8_t transmit_data[] = "Hello World. I'm UART Module of MSP430F5529 Microcontroller"
            " !!!\r\n";
    volatile uint8_t i = 0;
    for (i = 0; i < strlen((char const*)transmit_data); i++) {
        USCI_A_UART_transmitData(USCI_A0_BASE,*(transmit_data+i));
        /* Wait transmission is completed */
        while(USCI_A_UART_queryStatusFlags(USCI_A0_BASE, USCI_A_UART_BUSY)
                    == USCI_A_UART_BUSY);
    }
    DELAY500K;
}

/* Interrupt Service Routines */
#pragma vector = PORT1_VECTOR
__interrupt void pushbutton_ISR(void) {
    switch(__even_in_range(P1IV, 0x10)) {
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

#pragma vector = USCI_A0_VECTOR
__interrupt void UART_ISR(void) {
    volatile uint8_t receive_data = 0;
    if(USCI_A_UART_getInterruptStatus(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG)
            ==  USCI_A_UART_RECEIVE_INTERRUPT_FLAG) {
        receive_data = USCI_A_UART_receiveData(USCI_A0_BASE);
        USCI_A_UART_clearInterrupt(USCI_A0_BASE, USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    }
}



