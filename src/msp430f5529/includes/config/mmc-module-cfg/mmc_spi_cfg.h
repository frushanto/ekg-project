#ifndef MMC_SPI_CFG_H
#define MMC_SPI_CFG_H

#include <msp430.h>
#include <msp430f5529.h>
#include <stdint.h>

//void spi_initialize();
//void spi_set_divisor(unsigned int divisor);
void MMC_CS_Assert();
void MMC_CS_Deassert();
void MMC_Send_Byte(uint8_t input);
uint8_t MMC_Receive_Byte();
void MMC_Enable();
void MMC_Disable();

#endif

