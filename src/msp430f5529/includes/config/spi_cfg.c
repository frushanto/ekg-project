/*
 * spi_cfg.h
 *
 *  Created on: 9 Dec 2020
 *      Author: ivan
 */


#include <includes/config/spi_cfg.h>

//*****************************************************************************
//
//Specify desired frequency of SPI communication
//
//*****************************************************************************
#define SPICLK      10000000         // 10MHz

uint8_t transmitData = 0x00, receiveData = 0x00;
uint8_t returnValue = 0x00;

/* Function declarations */
void Init_SPI() {
    //Initialize Master
    USCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = USCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = UCS_getSMCLK();
    param.desiredSpiClock = SPICLK;
    param.msbFirst = USCI_B_SPI_MSB_FIRST;
    param.clockPhase = USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    returnValue =  USCI_B_SPI_initMaster(USCI_B0_BASE, &param);

    if (STATUS_FAIL == returnValue){
        //LED On
        GPIO_setOutputHighOnPin(
            GPIO_PORT_P1,
            GPIO_PIN0
            );
        return;
    }

    //Enable SPI module
    USCI_B_SPI_enable(USCI_B0_BASE);

    //Enable Receive interrupt
	USCI_B_SPI_clearInterrupt(USCI_B0_BASE,
		USCI_B_SPI_RECEIVE_INTERRUPT);
    USCI_B_SPI_enableInterrupt(USCI_B0_BASE,
		USCI_B_SPI_RECEIVE_INTERRUPT);

    //Now with SPI signals initialized, reset slave
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P2,
        GPIO_PIN7
        );

    //LED On
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P4,
        GPIO_PIN7
        );

    //Wait for slave to initialize
    __delay_cycles(100);

    //Initialize data values
    transmitData = 0x00;

    //USCI_A0 TX buffer ready?
    while (!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,
               USCI_B_SPI_TRANSMIT_INTERRUPT));

    //Transmit Data to slave
    USCI_B_SPI_transmitData(USCI_B0_BASE, transmitData);
}

void SPI_Send_Data(uint8_t spiSendData) {
    //USCI_B0 TX buffer ready?
    while (!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,
                USCI_B_SPI_TRANSMIT_INTERRUPT)) ;

    receiveData = USCI_B_SPI_receiveData(USCI_B0_BASE);

    //Increment data
    spiSendData++;

    //Send next value
    USCI_B_SPI_transmitData(USCI_B0_BASE,
    spiSendData
    );
}

//******************************************************************************
//
//This is the USCI_B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCI_B0_ISR (void)
{
    switch (__even_in_range(UCB0IV,4)){
        //Vector 2 - RXIFG
        case 2:
            // //USCI_B0 TX buffer ready?
            // while (!USCI_B_SPI_getInterruptStatus(USCI_B0_BASE,
            //            USCI_B_SPI_TRANSMIT_INTERRUPT)) ;

            // receiveData = USCI_B_SPI_receiveData(USCI_B0_BASE);

            // //Increment data
            // transmitData++;

            // //Send next value
            // USCI_B_SPI_transmitData(USCI_B0_BASE,
            // transmitData
            // );

            // //Delay between transmissions for slave to process information
            // __delay_cycles(40);

            break;
        default: break;
    }
}
