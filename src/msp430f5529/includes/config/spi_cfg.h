#ifndef INCLUDES_CONFIG_SPI_CFG_H_
#define INCLUDES_CONFIG_SPI_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>

/* Function declarations */
void Init_SPI(void);
void SPI_Send_Data(uint8_t spiSendData);

#endif /* INCLUDES_CONFIG_SPI_CFG_H_ */
