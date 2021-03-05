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
#define RX_BUFFER_SIZE 10
#define TX_BUFFER_SIZE 40

//Modul-global variables
static uint8_t uart_tx = 0;
static uint8_t tx_buffer[TX_BUFFER_SIZE] = { };

static uint8_t uart_rx = 0;
static uint8_t rx_buffer[RX_BUFFER_SIZE] = { };

//Busy bit, if true DMA is sending a message atm.
static bool bt_busy = 0;
/*
 * Fuction to init UART for Bluetooth modul
 * Call in Main
 */
bool Init_UART_BT(void)
{

    //Config GPIO Pins
    //P4.4 = UART_TX_Blue, P4.5 = UART_RX_Blue
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
    GPIO_PIN4 + GPIO_PIN5);

    //Prepare Init Function
    //Baudrate = 38400/115200, SMCLK = 20MHz
    USCI_A_UART_initParam uart_init_param = { };

    uart_init_param.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK; //Use SMCLK
    uart_init_param.clockPrescalar = 32;   // -> 32 = 38400, 10 = 115200
    uart_init_param.firstModReg = 9;       // ->  9 = 38400, 14 = 115200
    uart_init_param.secondModReg = 0;
    uart_init_param.parity = USCI_A_UART_NO_PARITY;
    uart_init_param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    uart_init_param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    uart_init_param.uartMode = USCI_A_UART_MODE; //use normal mode, no multiprocessor
    uart_init_param.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION; //use oversampling

    //Run Init Function
    if ( STATUS_FAIL == USCI_A_UART_init(USCI_A1_BASE, &uart_init_param))
    {
        return STATUS_FAIL;
    }

    //Enable UART module for operation
    USCI_A_UART_enable(USCI_A1_BASE);

    //Set USCI UART in sleep mode
    //USCI_A_UART_setDormant(USCI_A1_BASE);

    //Enable Receive Interrupt
    USCI_A_UART_clearInterrupt(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
    //USCI_A_UART_enableInterrupt(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT);

    /********* configure DMA **************/

    /*
     * Notes:
     * Single Transfer: DMAEN is cleared after each Transfer (Transfer finished if DMAxSZ
     *      Transfers have been made (Transfersize)
     * DMA_enableTransfers: Sets DAMEN Bit -> Use to Trigger new Transfers in Single Mode
     * DMA_startTransfer: sets only DMAREQ -> only to use with TRIGGERSOURCE_0
     */

    //Configure DMA channel 0 for Transfer Data from TX_Buffer to UART_A1 Module (BT)
    DMA_initParam dma0_param = { };
    dma0_param.channelSelect = DMA_CHANNEL_0;                 //Use Channel 0
    dma0_param.transferModeSelect = DMA_TRANSFER_SINGLE;     //Transfer one time
    dma0_param.transferSize = sizeof(tx_buffer) - 1; //Dont send zero Termination
    dma0_param.triggerSourceSelect = DMA_TRIGGERSOURCE_21;    //UART_A1-TxD
    dma0_param.transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE; //Byte-to-Byte Transfer
    dma0_param.triggerTypeSelect = DMA_TRIGGER_HIGH; //Send nex bit if UART_A1_TX is ready
    DMA_init(&dma0_param);

    //Configure DMA channel 0 Data Source
    DMA_setSrcAddress(DMA_CHANNEL_0,   //Config Channel 0
            (uint32_t) (uintptr_t) tx_buffer,   //Source Adress is tx_buffer
            DMA_DIRECTION_INCREMENT);      //Increase Adress after each Transfer

    //Configure DMA Channel 0 Destination
    DMA_setDstAddress(DMA_CHANNEL_0,          //Config Channel 0
            USCI_A_UART_getTransmitBufferAddressForDMA(USCI_A1_BASE), //Destination is UART_A1 Tx Register
            DMA_DIRECTION_UNCHANGED);         //Dont Touch Adress after Transfer

    //Enable transfers on DMA channel 0 -> In our config this would start a transmit so dont use here
    //DMA_enableTransfers(DMA_CHANNEL_0);

    //Enable DMA0 Interupts
    DMA_clearInterrupt(DMA_CHANNEL_0);
    DMA_enableInterrupt(DMA_CHANNEL_0);

    return STATUS_SUCCESS;
}

//Standard send function with wait - dont use
bool send_bt_value(uint16_t value)
{
    sprintf((char*) tx_buffer, "s%04d\n", value);
    for (uint8_t i = 0; i < strlen((char*) tx_buffer); i++)
    {
        //Transmit char by char
        USCI_A_UART_transmitData(USCI_A1_BASE, tx_buffer[i]);

        //Wait for Transmit finish
        while (USCI_A_UART_queryStatusFlags(USCI_A1_BASE, USCI_A_UART_BUSY)
                == USCI_A_UART_BUSY)
            ;
    }
    return STATUS_SUCCESS;
}

//Standard send function with wait - dont use
bool send_bt_string(char *str)
{
    sprintf((char*) tx_buffer, "s%04d\n", str);
    for (uint8_t i = 0; i < strlen((char*) tx_buffer); i++)
    {
        //Transmit char by char
        USCI_A_UART_transmitData(USCI_A1_BASE, tx_buffer[i]);

        //Wait for Transmit finish
        while (USCI_A_UART_queryStatusFlags(USCI_A1_BASE, USCI_A_UART_BUSY)
                == USCI_A_UART_BUSY)
            ;
    }
    return STATUS_SUCCESS;
}

//Send a Value / String with UART A_1 using DMA - use this
bool send_bt_value_dma(uint16_t value)
{
    //Check if there is already a message in Pipeline
    if (bt_busy)
        return STATUS_FAIL;
    value = value / 20;
    //Build string to be send
    sprintf((char*) tx_buffer, "s%03d\n", value);

    //Set dynamicly Data Size to Transfer
    DMA_setTransferSize(DMA_CHANNEL_0, (uint16_t) strlen((char*) tx_buffer));

    //Toggel DMA enable (DMAEN) bit to start new transfer
    //DMA_startTransfer() - This only works with Trigger Source 0
    DMA_enableTransfers(DMA_CHANNEL_0);

    //Set Busy bit so no other transfer can be started
    bt_busy = true;

    return STATUS_SUCCESS;
}

//Interupt not needed since BT isnt recieving anything
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
//DMA Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
    switch(__even_in_range(DMAIV,16))
    {
        case 0: break;
        case 2: // DMA0IFG = DMA Channel 0
            //DMA finished, reset busy bit
            bt_busy = 0;
            break;
        case 4: break;// DMA1IFG = DMA Channel 1
        case 6: break;// DMA2IFG = DMA Channel 2
        case 8: break;// DMA3IFG = DMA Channel 3
        case 10: break;// DMA4IFG = DMA Channel 4
        case 12: break;// DMA5IFG = DMA Channel 5
        case 14: break;// DMA6IFG = DMA Channel 6
        case 16: break;// DMA7IFG = DMA Channel 7
        default: break;
    }
}
