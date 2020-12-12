/*
 * uart_cfg.c
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#include <includes/config/uart_cfg.h>

#define UART_BASE_A0    0
#define UART_BASE_A1    1

#define UART_BASE       UART_BASE_A0
#define UART_MESSAGE_MAX_LENGTH 12

#define RECEIVE_DATA_COUNT                      0x02

uint8_t uart_received_data[UART_MESSAGE_MAX_LENGTH] = {0x00};
uint8_t uart_received_data_counter = 0;

void Init_UART() {

    #if UART_BASE == UART_BASE_A0
    /* Configure GPIO pins for UART A0 BASE
     * P3.4/UCA0RXD/UCA0SOMI
     * P3.3/UCA0TXD/UCA0SIMO
     * */
   GPIO_setAsPeripheralModuleFunctionInputPin(
           GPIO_PORT_P3, GPIO_PIN4); // UART Rx Pin - P3.4
   GPIO_setAsPeripheralModuleFunctionOutputPin(
           GPIO_PORT_P3,GPIO_PIN3); // UART Tx Pin - P3.3

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

    USCI_A_UART_initParam uart_cfg = {0};

    /* Baudrate = 9600, clock freq = ACKL = 32kHz
     * UCBRx = 3, UCBRFx = 0, UCBRSx = 3, UCOS16 = 0
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


    // Init UART A0
    //USCI_A_UART_init(USCI_A0_BASE, &uart_cfg);
    if (STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &uart_cfg)){
        return;
    }
    USCI_A_UART_enable(USCI_A0_BASE);
    USCI_A_UART_clearInterrupt(USCI_A0_BASE,
            USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE,
            USCI_A_UART_RECEIVE_INTERRUPT);
}

void Test_UART_Erik(uint8_t test_value)
{
    USCI_A_UART_transmitData(USCI_A0_BASE, test_value);
    /* Wait transmission is completed */
    while(USCI_A_UART_queryStatusFlags(USCI_A0_BASE, USCI_A_UART_BUSY)== USCI_A_UART_BUSY);
}


void Test_UART(uint16_t adc_value) {
    uint8_t uart_transmit_set_val[] = "add 5,0,";//page8.n0.val=
    //uint8_t uart_transmit_get_val[] = "get n0.val";
    uint8_t uart_transmit_full_message[12] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    volatile uint8_t fm_counter = 0;
    volatile uint8_t i = 0;
    uint8_t uart_transmit_length = strlen((char const*)uart_transmit_set_val);
    // Transmit array with data
    for (i = 0; i < strlen((char const*)uart_transmit_set_val); i++) {
    //for (i = 0; i < 10; i++) {
        USCI_A_UART_transmitData(USCI_A0_BASE, uart_transmit_set_val[i]);
        uart_transmit_full_message[fm_counter] = uart_transmit_set_val[i];
        fm_counter++;
        /* Wait transmission is completed */
        while(USCI_A_UART_queryStatusFlags(
                USCI_A0_BASE, USCI_A_UART_BUSY)
                == USCI_A_UART_BUSY);
    }

    int test_val = (adc_value / 16) - 30;
    uint8_t buffer[50];
    sprintf( buffer, "%d", test_val );
    for (i = 0; i < strlen((char const*)buffer); i++) {
        USCI_A_UART_transmitData(USCI_A0_BASE, buffer[i]);
        uart_transmit_full_message[fm_counter] = buffer[i];
        fm_counter++;
        /* Wait transmission is completed */
        while(USCI_A_UART_queryStatusFlags(
                USCI_A0_BASE, USCI_A_UART_BUSY)
                == USCI_A_UART_BUSY);
    }


    // Transmit 0xFF 3 times. Required to define end of the message
    uint8_t uart_transmit_cmd_ff = 0xFF;
    for (i = 0; i < 3; i++) {
        USCI_A_UART_transmitData(USCI_A0_BASE, uart_transmit_cmd_ff);
        uart_transmit_full_message[fm_counter] = uart_transmit_cmd_ff;
        fm_counter++;
        /* Wait transmission is completed */
        while(USCI_A_UART_queryStatusFlags(
                USCI_A0_BASE, USCI_A_UART_BUSY)
                == USCI_A_UART_BUSY);
    }
    _delay_cycles(10);
}

/*
 * EUSCI_A_UART_enable() enables the EUSI_A_UART and the module
 * is now ready for transmit and receive. It is recommended to
 * initialize the EUSI_A_UART via EUSCI_A_UART_init(), enable
 * the required interrupts and then enable EUSI_A_UART via
 * EUSCI_A_UART_enable().
 * */
#pragma vector = USCI_A0_VECTOR
__interrupt void UART_A0_ISR(void) {
    switch (__even_in_range(UCA0IV, 4))
        {
        case 0:
            break; // Vector 0 - no interrupt
        case 2: // Vector 2 - RXIFG
            if (uart_received_data_counter < UART_MESSAGE_MAX_LENGTH) {
                uart_received_data[uart_received_data_counter] =
                    USCI_A_UART_receiveData(USCI_A0_BASE);
                uart_received_data_counter++;
            } else {
                uart_received_data_counter = 0;
            }
            USCI_A_UART_clearInterrupt(USCI_A0_BASE,
                    USCI_A_UART_RECEIVE_INTERRUPT);
            break;
        case 4:
            break; // Vector 4 - TXIFG
        default:
            break;
        }
}



