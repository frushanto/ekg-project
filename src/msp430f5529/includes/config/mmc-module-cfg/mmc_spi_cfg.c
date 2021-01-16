#include <msp430.h>
#include <msp430f5529.h>
#include "mmc_spi_cfg.h"

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
void MMC_Send_Byte(unsigned char input)
{
	char i;
	UCB0IFG &= ~UCRXIFG;
	while (!(UCB0IFG&UCTXIFG));
	/* Send the byte */
	UCB0TXBUF = input;
	/* Wait for the byte to be sent */
	// TODO Test the line below
	//while ((UCB0IFG & UCRXIFG) == 0) { }
	for (i = 0; i < 50; i++);
}
/* Receive a byte. Output an 0xFF (the bus idles high) to receive the byte */
unsigned char MMC_Receive_Byte()
{
	char i;
	unsigned char tmp;
	UCB0IFG &= ~UCRXIFG;
	while (!(UCB0IFG&UCTXIFG));
	/* Send the byte */
	UCB0TXBUF = 0xFF;
	/* Wait for the byte to be received */
	// TODO Test the line below
	//while ((UCB0IFG & UCRXIFG) == 0) { }
	tmp = UCB0RXBUF;
	for (i = 0; i < 50; i++);
	return tmp;
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
