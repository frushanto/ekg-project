#include <msp430.h>
#include <msp430f5529.h>
#include "mmc_spi_cfg.h"

uint8_t mmc_receive_byte;
uint16_t gie;

/* Initialize and enable the SPI module */
/*
void spi_initialize()
{
	P3SEL |= BIT0+BIT1;                       // SPI SDI -- P3.0 -- MOSI, SPI SDO -- P3.1 -- MISO
	P3SEL |= BIT2;                            // SPI SCLK -- P3.2

	P3OUT |= BIT7; 							  // Setup P3.7 as the SS signal, active // low. So, initialize it high.
	P3DIR |= BIT7; 							  // Setup P3.7 as an output

	UCB0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    // 3-pin, 8-bit SPI master
											  // Clock polarity high, MSB
	UCB0CTL1 |= UCSSEL_2;                     // SMCLK
	UCB0BR0 = 0x02;                           // /2
	UCB0BR1 = 0;                              //

	UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	UCB0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}
*/

/* Set the baud-rate divisor. The correct value is computed by dividing
the clock rate by the desired baud rate. The minimum divisor allowed
is 2. */
/*
void spi_set_divisor(unsigned int divisor)
{
	UCB0CTL1 |= UCSWRST;; // Temporarily disable the SPI module
	UCB0BR1 = divisor >> 8;
	UCB0BR0 = divisor;
	UCB0CTL1 &= ~UCSWRST; // SPI enable // Re-enable SPI
}
*/

/* Assert the CS signal, active low (CS=0) */
void MMC_CS_Assert()
{
	// Pin 2.7
	P2OUT &= ~BIT7;
}
/* Deassert the CS signal (CS=1) */
void MMC_CS_Deassert()
{
	// Pin 2.7
	P2OUT |= BIT7;
}

/* Send a single byte over the SPI port */
void MMC_Send_Byte(uint8_t input)
{
	gie = __get_SR_register() & GIE;		// Store current GIE state
	__disable_interrupt();                          // Make this operation atomic

	UCB0IFG &= ~UCRXIFG;
	while (!(UCB0IFG&UCTXIFG));
	/* Send the byte */
	UCB0TXBUF = input;
	/* Wait for the byte to be sent */
	while (UCB0STAT & UCBUSY);		// Wait for all TX/RX to finish
	UCB0RXBUF;						// Dummy read to empty RX buffer
									// and clear any overrun conditions

	__bis_SR_register(gie);         // Restore original GIE state
	// TODO Test the line below
	//while ((UCB0IFG & UCRXIFG) == 0) { }
	//for (i = 0; i < 5; i++);
}
/* Receive a byte. Output an 0xFF (the bus idles high) to receive the byte */
uint8_t MMC_Receive_Byte()
{
	gie = __get_SR_register() & GIE;	// Store current GIE state
    __disable_interrupt();                      // Make this operation atomic

	UCB0IFG &= ~UCRXIFG;			// Ensure RXIFG is clear
	while (!(UCB0IFG&UCTXIFG));		// Wait while not ready for TX
	UCB0TXBUF = 0xFF;				// Write dummy byte
	while (!(UCB0IFG & UCRXIFG)) ;  // Wait for RX buffer (full)
	mmc_receive_byte = UCB0RXBUF;
	__bis_SR_register(gie);        // Restore original GIE state
	return mmc_receive_byte;
}

/* Disable the SPI MMC module. This function assumes the module had
* already been initialized. */
void MMC_Disable()
{
	/* Put the SPI MMC module in reset mode */
	UCB0CTL1 |= UCSWRST;
}
void MMC_Enable()
{
    /* Take the SPI MMC module out of reset mode */
	UCB0CTL1 &= ~UCSWRST;
}
