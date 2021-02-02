/*
 * bluetooth.c
 *
 *  Created on: 30.01.2021
 *      Author: Adrian Jäger
 *
 *      Bluetooth is Controlled by the USCI_A1 Module (USCI_A0 is for Display)
 */

//Includes
#include "bluetooth.h"

//Mudul-global defines
#define RX_BUFFER_SIZE 20
#define TX_BUFFER_SIZE 20

//Modul-global variables
static uint8_t uart_tx = 0;
static uint8_t tx_buffer[TX_BUFFER_SIZE] = {};

static uint8_t uart_rx = 0;
static uint8_t rx_buffer[RX_BUFFER_SIZE] = {};


/*
 * Fuction to init UART for Bluetooth modul
 * Call in Main
 */
bool Init_UART_BT (void) {

    //Config GPIO Pins
    //P4.4 = UART_TX_Blue, P4.5 = UART_RX_Blue
     GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4 + GPIO_PIN5);

     //Prepare Init Function
     //Baudrate = 38400, SMCLK = 20MHz
     //UCBRx = 109, UCBRFx = 0, UCBRSx = 2, UCOS16 = 0
     USCI_A_UART_initParam uart_init_param = {0};

     uart_init_param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK; //Use SMCLK
     uart_init_param.clockPrescalar = 32;
     uart_init_param.firstModReg = 9;
     uart_init_param.secondModReg = 0;
     uart_init_param.parity = USCI_A_UART_NO_PARITY;
     uart_init_param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
     uart_init_param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
     uart_init_param.uartMode = USCI_A_UART_MODE; //use normal mode, no multiprocessor
     uart_init_param.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; //user oversampling

     //Run Init Function
     if ( STATUS_FAIL == USCI_A_UART_init(USCI_A1_BASE, &uart_init_param)){
         return STATUS_FAIL;
     }


     //Enable UART module for operation
     USCI_A_UART_enable(USCI_A1_BASE);

     //Set USCI UART in sleep mode
     //USCI_A_UART_setDormant(USCI_A1_BASE);

     //Enable Receive Interrupt
     USCI_A_UART_clearInterrupt(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
     USCI_A_UART_enableInterrupt(USCI_A1_BASE,USCI_A_UART_RECEIVE_INTERRUPT);

    return STATUS_SUCCESS;
}




//Define USCI_A1 Interrupt Vector
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A1_VECTOR)))
#endif
void USCI_A1_ISR (void)
{
    switch (__even_in_range(UCA1IV,4)){
        //Vector 2 - RXIFG
        case 2:

            //Receive the data
            uart_rx = USCI_A_UART_receiveData(USCI_A1_BASE);

            //Send data back "echo"
            USCI_A_UART_transmitData(USCI_A0_BASE, uart_rx);

            break;
        default: break;
    }
}
