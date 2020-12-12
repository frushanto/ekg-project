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

/* Function declarations */
void Init_UART(void);
void Test_UART(uint16_t adc_value);
void Test_UART_With_Display(void);
void Test_UART_Erik(uint8_t test_value);

#endif /* INCLUDES_CONFIG_UART_CFG_H_ */
