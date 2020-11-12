/*
 * uart_cfg.c
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#include <uart_cfg.h>

#define UART_BASE_A0    0
#define UART_BASE_A1    1

#define UART_BASE       UART_BASE_A0
#define UART_MESSAGE_MAX_LENGTH 8

uint8_t uart_received_data[UART_MESSAGE_MAX_LENGTH] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
uint8_t uart_char_number = 0;

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

#if UART_BASE == UART_BASE_A0
    /* Configure GPIO pins for UART A0 BASE
     * P3.4/UCA0RXD/UCA0SOMI
     * P3.3/UCA0TXD/UCA0SIMO
     * */
   GPIO_setAsPeripheralModuleFunctionInputPin(
           GPIO_PORT_P3, GPIO_PIN4); // UART Rx Pin
   GPIO_setAsPeripheralModuleFunctionOutputPin(
           GPIO_PORT_P3,GPIO_PIN3); // UART Tx Pin

#elif UART_BASE == UART_BASE_A1

    /* Configure GPIO pins for UART A1 BASE
     * P4.5/UCA1RXD
     * P4.4/UCA1TXD
     * */
    GPIO_setAsPeripheralModuleFunctionInputPin(
                GPIO_PORT_P4, GPIO_PIN5); // UART Rx Pin
    GPIO_setAsPeripheralModuleFunctionOutputPin(
                GPIO_PORT_P4,GPIO_PIN4); // UART Tx Pin

#else 
    # error "Please specify UART BASE"
#endif


    // Init UART A0
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
        USCI_A_UART_transmitData(USCI_A1_BASE,*(transmit_data+i));
        /* Wait transmission is completed */
        while(USCI_A_UART_queryStatusFlags(
                USCI_A1_BASE, USCI_A_UART_BUSY)
                == USCI_A_UART_BUSY);
    }
    DELAY500K;
}

void Test_UART_With_Display() {
    volatile uint8_t receive_data = 0;
    uint8_t transmit_data = 1;
    uint8_t * p_transmit_data;
    p_transmit_data = &transmit_data;
    volatile uint8_t i = 0;
    for (i = 0; i < 8; i++) {
        USCI_A_UART_transmitData(USCI_A0_BASE, *p_transmit_data);
        /* Wait transmission is completed */
        while(USCI_A_UART_queryStatusFlags(
                USCI_A0_BASE, USCI_A_UART_BUSY)
                == USCI_A_UART_BUSY);
        transmit_data++;
    }
    DELAY500K;
}

/*
 * EUSCI_A_UART_enable() enables the EUSI_A_UART and the module
 * is now ready for transmit and receive. It is recommended to
 * initialize the EUSI_A_UART via EUSCI_A_UART_init(), enable
 * the required interrupts and then enable EUSI_A_UART via
 * EUSCI_A_UART_enable().
 * */
#pragma vector = USCI_A0_VECTOR
__interrupt void UART_Receive_ISR(void) {
    //volatile uint8_t receive_data[8] = {0};
    if(USCI_A_UART_getInterruptStatus(
            USCI_A0_BASE,
            USCI_A_UART_RECEIVE_INTERRUPT_FLAG)
            ==  USCI_A_UART_RECEIVE_INTERRUPT_FLAG) {
        if(!(uart_char_number <= UART_MESSAGE_MAX_LENGTH-1)) {
            uart_char_number == 0;
        }
        uart_received_data[uart_char_number] = USCI_A_UART_receiveData(USCI_A0_BASE);
        USCI_A_UART_clearInterrupt(USCI_A0_BASE,
                                   USCI_A_UART_RECEIVE_INTERRUPT_FLAG);
        uart_char_number++;
    }
}

// Test UART Echo Interrupt
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=USCI_A0_VECTOR
//__interrupt void USCI_A0_ISR(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//    switch (__even_in_range(UCA1IV, 4))
//    {
//    case 0:
//        break; // Vector 0 - no interrupt
//    case 2: // Vector 2 - RXIFG
//        while (!(UCA1IFG & UCTXIFG))
//            ; // USCI_A0 TX buffer ready?
//        UCA1TXBUF = UCA1RXBUF; // TX -> RXed character
//        break;
//    case 4:
//        break; // Vector 4 - TXIFG
//    default:
//        break;
//    }
//}



