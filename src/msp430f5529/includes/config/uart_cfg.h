/*
 * uart_cfg.h
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#ifndef INCLUDES_CONFIG_UART_CFG_H_
#define INCLUDES_CONFIG_UART_CFG_H_

#include <msp430.h>
#include <driverlib.h>
#include <std_types.h>

/* Function declarations */
void Init_UART(void);
void Test_UART(void);
void Test_UART_With_Display(void);

#endif /* INCLUDES_CONFIG_UART_CFG_H_ */
