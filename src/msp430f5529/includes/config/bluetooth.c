/*
 * bluetooth.c
 *
 *  Created on: 30.01.2021
 *      Author: Adrian J�ger
 *
 *      Bluetooth is Controlled by the USCI_A1 Module (USCI_A0 is for Display)
 */

//Includes
#include "bluetooth.h"
#include "main.h"

//Mudul-global defines
#define RX_BUFFER_SIZE 20
#define TX_BUFFER_SIZE 7

//Modul-global variables
static uint8_t uart_tx = 0;
static uint8_t tx_buffer[TX_BUFFER_SIZE] = {"4321\r\n"};

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
     //Baudrate = 115200, SMCLK = 20MHz
     USCI_A_UART_initParam uart_init_param = {0};

     uart_init_param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK; //Use SMCLK
     uart_init_param.clockPrescalar = 10;
     uart_init_param.firstModReg = 14;
     uart_init_param.secondModReg = 0;
     uart_init_param.parity = USCI_A_UART_NO_PARITY;
     uart_init_param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
     uart_init_param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
     uart_init_param.uartMode = USCI_A_UART_MODE; //use normal mode, no multiprocessor
     uart_init_param.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; //use oversampling

     //Run Init Function
     if ( STATUS_FAIL == USCI_A_UART_init(USCI_A1_BASE, &uart_init_param)){
         return STATUS_FAIL;
     }


     //Enable UART module for operation
     USCI_A_UART_enable(USCI_A1_BASE);

     //Set USCI UART in sleep mode
     //USCI_A_UART_setDormant(USCI_A1_BASE);

     //Enable Receive Interrupt
//     USCI_A_UART_clearInterrupt(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
//     USCI_A_UART_enableInterrupt(USCI_A1_BASE,USCI_A_UART_RECEIVE_INTERRUPT);

     // configure DMA
     //Initialize and Setup DMA Channel 0
      /*
       * Configure DMA channel 0
       * Configure channel for single transfers, so DMAEN must be toggled for retransfer
       * DMA interrupt flag will be set after every string transfer - to be tested
       * Use DMA Trigger Source 21 - UART TX finished
       * Transfer Byte-to-byte
       * Trigger Upon High Source Signal
       */
      DMA_initParam param2 = {0};
      param2.channelSelect = DMA_CHANNEL_0;
      param2.transferModeSelect = DMA_TRANSFER_SINGLE;
      param2.transferSize = (sizeof tx_buffer - 1); //Dont send zero Termination
      param2.triggerSourceSelect = DMA_TRIGGERSOURCE_21;
      param2.transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE;
      param2.triggerTypeSelect = DMA_TRIGGER_HIGH;
      DMA_init(&param2);
      /*
       * Configure DMA channel 0
       * Use String1 as source
       * Increment source address after every transfer
       */
      DMA_setSrcAddress(DMA_CHANNEL_0,
          (uint32_t)(uintptr_t)tx_buffer,
          DMA_DIRECTION_INCREMENT);
      /*
       * Base Address of the DMA Module
       * Configure DMA channel 0
       * Use UART TX Buffer as destination
       * Don't move the destination address after every transfer
       */
      DMA_setDstAddress(DMA_CHANNEL_0,
          USCI_A_UART_getTransmitBufferAddressForDMA(USCI_A1_BASE),
          DMA_DIRECTION_UNCHANGED);

      //Enable transfers on DMA channel 0
      DMA_enableTransfers(DMA_CHANNEL_0);


    return STATUS_SUCCESS;
}

bool send_bt_value(uint16_t value){
    sprintf((char*)tx_buffer, "%04d\r\n", value);
    for (uint8_t i = 0; i < strlen((char*)tx_buffer); i++){
        //Transmit char by char
        USCI_A_UART_transmitData(USCI_A1_BASE, tx_buffer[i]);

        //Wait for Transmit finish
        while (USCI_A_UART_queryStatusFlags(USCI_A1_BASE, USCI_A_UART_BUSY) 
                                            == USCI_A_UART_BUSY);
    }
    return STATUS_SUCCESS;
}

bool send_bt_string(char* str){
    sprintf((char*)tx_buffer, "%s\r\n", str);
    for (uint8_t i = 0; i < strlen((char*)tx_buffer); i++){
        //Transmit char by char
        USCI_A_UART_transmitData(USCI_A1_BASE, tx_buffer[i]);

        //Wait for Transmit finish
        while (USCI_A_UART_queryStatusFlags(USCI_A1_BASE, USCI_A_UART_BUSY)
                                            == USCI_A_UART_BUSY);
    }
    return STATUS_SUCCESS;
}

bool send_value_dma(uint16_t value){
    sprintf((char*)tx_buffer, "%04d\r\n", value);

    //Toggel DMA enable bit to start new transfer
    DMA0CTL^= DMAEN;

    return STATUS_SUCCESS;
}


//
////Define USCI_A1 Interrupt Vector
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=USCI_A1_VECTOR
//__interrupt
//#elif defined(__GNUC__)
//__attribute__((interrupt(USCI_A1_VECTOR)))
//#endif
//void USCI_A1_ISR (void)
//{
//    switch (__even_in_range(UCA1IV,4)){
//        //Vector 2 - RXIFG
//        case 2:
//
//            //Receive the data
//            uart_rx = USCI_A_UART_receiveData(USCI_A1_BASE);
//
//            //Send data back "echo"
//            USCI_A_UART_transmitData(USCI_A1_BASE, uart_rx);
//
//            break;
//        default: break;
//    }
//}

//------------------------------------------------------------------------------
// DMA Interrupt Service Routine
//------------------------------------------------------------------------------
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=DMA_VECTOR
//__interrupt void DMA_ISR(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(DMA_VECTOR))) DMA_ISR (void)
//#else
//#error Compiler not supported!
//#endif
//{
//switch(__even_in_range(DMAIV,16))
//{
//case 0: break;
//case 2: // DMA0IFG = DMA Channel 0
//P1OUT ^= BIT0; // Toggle P1.0 - PLACE BREAKPOINT HERE AND CHECK DMA_DST VARIABLE
//break;
//case 4: break; // DMA1IFG = DMA Channel 1
//case 6: break; // DMA2IFG = DMA Channel 2
//case 8: // DMA3IFG = DMA Channel 3
//DMA0CTL |= DMAREQ;
//P1OUT ^= BIT0; // Toggle P1.0 - PLACE BREAKPOINT HERE AND CHECK DMA_DST VARIABLE
//break;
//case 10: break; // DMA4IFG = DMA Channel 4
//case 12: break; // DMA5IFG = DMA Channel 5
//case 14: break; // DMA6IFG = DMA Channel 6
//case 16: break; // DMA7IFG = DMA Channel 7
//default: break;
//}
//}
