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
#include <stdlib.h> // Used for:
#include <string.h> // dynamic array
#include <time.h>

/* Function declarations */
void Init_UART(void);
void Test_UART(uint16_t adc_value);

/***** GLOBALE VARS *****/

extern uint8_t page_two_start_stop;
extern uint8_t adc_flag;
extern uint8_t timer_1hz_flag;
extern uint16_t sec;
extern uint16_t min;

extern uint16_t adc_result;

#endif /* INCLUDES_CONFIG_UART_CFG_H_ */
