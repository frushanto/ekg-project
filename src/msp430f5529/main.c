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
void Init_ADC(void);
void Test_ADC(void);

/* Function definitions */
void main(void){
    /* Init MSP430 */
    Init_Watchdog();
    Init_GPIO();
    Init_Clocks();
    // TODO Timers
    Init_UART();
    Init_ADC();
    EnableGlobalInterrupt();

	/* Toggle LEDs */
	while(1) {
        //Test_UART();
        Test_ADC();
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
    // What edge transition will trigger an interrupt
    //(low-to-high or high-to-low)
	//GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1,
        //GPIO_LOW_TO_HIGH_TRANSITION);
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
     * From: Table 39-5. Recommended Settings for
     * Typical Crystals and Baud Rates at Page 1039/1189 User guide.
     * For more information about baudrate setting see 39.3.10
     * Setting a Baud Rate at page 1036 User Guide
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

    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P3, GPIO_PIN4); // UART Rx Pin
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P3,GPIO_PIN3); // UART Tx Pin

    USCI_A_UART_init(USCI_A0_BASE, &uart_cfg);
    USCI_A_UART_enable(USCI_A0_BASE);
    USCI_A_UART_clearInterrupt(USCI_A0_BASE,
            USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE,
            USCI_A_UART_RECEIVE_INTERRUPT);
}

void Test_UART() {
    volatile uint8_t receive_data = 0;
    uint8_t transmit_data[] = "Hello World. I'm UART Module "
            "of MSP430F5529 Microcontroller"
            " !!!\r\n";
    volatile uint8_t i = 0;
    for (i = 0; i < strlen((char const*)transmit_data); i++) {
        USCI_A_UART_transmitData(USCI_A0_BASE,*(transmit_data+i));
        /* Wait transmission is completed */
        while(USCI_A_UART_queryStatusFlags(
                USCI_A0_BASE, USCI_A_UART_BUSY)
                == USCI_A_UART_BUSY);
    }
    DELAY500K;
}

void Init_ADC() {
    //Enable A/D channel A0
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P6, GPIO_PIN0);

    //Initialize the ADC12_A Module
    /*
     * Base address of ADC12_A Module
     * Use internal ADC12_A bit as sample/hold signal
     * to start conversion
     * USE ACLK 32kHz as clock source
     * Use default clock divider of 1
     */
    ADC12_A_init(ADC12_A_BASE,
                 ADC12_A_SAMPLEHOLDSOURCE_SC,
                 ADC12_A_CLOCKSOURCE_ACLK,
                 ADC12_A_CLOCKDIVIDER_1);

    ADC12_A_enable(ADC12_A_BASE);

    /*
     * Base address of ADC12_A Module
     * For memory buffers 0-7 sample/hold for 64
     * clock cycles
     * For memory buffers 8-15 sample/hold for 4
     * clock cycles (default)
     * Disable Multiple Sampling
     */
    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
        ADC12_A_CYCLEHOLD_64_CYCLES,
        ADC12_A_CYCLEHOLD_4_CYCLES,
        ADC12_A_MULTIPLESAMPLESDISABLE);

    //Configure Memory Buffer
    /*
     * Base address of the ADC12_A Module
     * Configure memory buffer 0
     * Map input A0 to memory buffer 0
     * Vr+ = Vref+ (int)
     * Vr- = AVss
     * Memory buffer 0 is not the end of a sequence
     */
    ADC12_A_configureMemoryParam adc_cfg = {0};
    adc_cfg.memoryBufferControlIndex = ADC12_A_MEMORY_0;
    adc_cfg.inputSourceSelect = ADC12_A_INPUT_A0;
    // TODO Ask about voltage references
    adc_cfg.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
    adc_cfg.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
    adc_cfg.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
    ADC12_A_configureMemory(ADC12_A_BASE ,&adc_cfg);

    //Configure internal reference
    //If ref generator busy, WAIT
    while (REF_ACTIVE == Ref_isRefGenBusy(REF_BASE));
    //Select internal ref = 1.5V
    Ref_setReferenceVoltage(REF_BASE,
        REF_VREF1_5V);
    //Internal Reference ON
    Ref_enableReferenceVoltage(REF_BASE);

    //Delay (~75us) for Ref to settle
    __delay_cycles(75);

    //Enable memory buffer 0 interrupt
    ADC12_A_clearInterrupt(ADC12_A_BASE,
            ADC12IFG0);
    ADC12_A_enableInterrupt(ADC12_A_BASE,
            ADC12IE0);
}

void Test_ADC() {
    //Enable/Start sampling and conversion
    /*
     * Base address of ADC12_A Module
     * Start the conversion into memory buffer 0
     * Use the single-channel, single-conversion mode
     */
    ADC12_A_startConversion(
            ADC12_A_BASE,
            ADC12_A_MEMORY_0,
            ADC12_A_SINGLECHANNEL);

    /*** POLLING METHOD TO TEST ADC ***/
    //Poll for interrupt on memory buffer 0
//    while (!ADC12_A_getInterruptStatus(ADC12_A_BASE,
//               ADC12IFG0));

    //LPM0, ADC12_A_ISR will force exit
    __bis_SR_register(LPM0_bits + GIE);
    //for Debugger
    __no_operation();
}

/* Interrupt Service Routines */
#pragma vector = PORT1_VECTOR
__interrupt void pushbutton_ISR(void) {
    switch(__even_in_range(P1IV, 0x10)) {
        case 0x00: break;   // None
        case 0x02: break;   // Pin 0
        case 0x04:          // Pin 1
            // Toggle LED
            GPIO_toggleOutputOnPin(GPIO_PORT_P1,
                                   GPIO_PIN0);
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
    if(USCI_A_UART_getInterruptStatus(
            USCI_A0_BASE,
            USCI_A_UART_RECEIVE_INTERRUPT_FLAG)
            ==  USCI_A_UART_RECEIVE_INTERRUPT_FLAG) {
        receive_data = USCI_A_UART_receiveData(USCI_A0_BASE);
        USCI_A_UART_clearInterrupt(USCI_A0_BASE,
            USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_A_ISR(void) {
    switch (__even_in_range(ADC12IV,34)) {
        case  0: break;   //Vector  0:  No interrupt
        case  2: break;   //Vector  2:  ADC overflow
        case  4: break;   //Vector  4:  ADC timing overflow
        case  6:          //Vector  6:  ADC12IFG0
                 //Is Memory Buffer 0 = A0 > 0.5AVcc?
                 if (ADC12_A_getResults(ADC12_A_BASE,
                             ADC12_A_MEMORY_0)
                         >= 0x7ff) {
                     //set P1.0
                     GPIO_setOutputHighOnPin(
                             GPIO_PORT_P1,
                             GPIO_PIN0
                             );
                 } else {
                     //Clear P1.0 LED off
                     GPIO_setOutputLowOnPin(
                             GPIO_PORT_P1,
                             GPIO_PIN0
                             );
                 }

                 //Exit active CPU
                 __bic_SR_register_on_exit(LPM0_bits);
        case  8: break;   //Vector  8:  ADC12IFG1
        case 10: break;   //Vector 10:  ADC12IFG2
        case 12: break;   //Vector 12:  ADC12IFG3
        case 14: break;   //Vector 14:  ADC12IFG4
        case 16: break;   //Vector 16:  ADC12IFG5
        case 18: break;   //Vector 18:  ADC12IFG6
        case 20: break;   //Vector 20:  ADC12IFG7
        case 22: break;   //Vector 22:  ADC12IFG8
        case 24: break;   //Vector 24:  ADC12IFG9
        case 26: break;   //Vector 26:  ADC12IFG10
        case 28: break;   //Vector 28:  ADC12IFG11
        case 30: break;   //Vector 30:  ADC12IFG12
        case 32: break;   //Vector 32:  ADC12IFG13
        case 34: break;   //Vector 34:  ADC12IFG14
        default: break;
    }
}

