/*
 * uart_cfg.h
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#ifndef INCLUDES_CONFIG_UART_CFG_H_
#define INCLUDES_CONFIG_UART_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function declarations */
void Init_UART(void);
void uart_transmit_data_start(uint8_t nextion_command[]);
void uart_transmit_data_value(uint16_t transmit_value);
void uart_transmit_data_end();
void uart_receive_data_end();

/***** GLOBALE VARS *****/
extern uint8_t g_kz_start_stop;
extern uint8_t g_lz_start_stop;

#endif /* INCLUDES_CONFIG_UART_CFG_H_ */
