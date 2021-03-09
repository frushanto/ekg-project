#ifndef INCLUDES_CONFIG_UART_CFG_H_
#define INCLUDES_CONFIG_UART_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

/* Function declarations */
void Init_UART(void);
void uart_transmit_data_start(char nextion_command[]);
void uart_transmit_data_value(uint16_t transmit_value);
void uart_transmit_data_end(void);
void uart_receive_data_end(void);

#endif /* INCLUDES_CONFIG_UART_CFG_H_ */
