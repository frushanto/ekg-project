/*
 * adc_cfg.h
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#ifndef INCLUDES_CONFIG_ADC_CFG_H_
#define INCLUDES_CONFIG_ADC_CFG_H_

#include <msp430.h>
#include <driverlib.h>
#include <std_types.h>

/* Function declarations */
void Init_ADC(void);
void Test_ADC(void);

/***** GLOBALE VARS *****/

extern uint8_t uart_puls_counter;
extern uint8_t adc_start_stop;
extern uint8_t timer_start_stop;

extern uint16_t adc_result;

#endif /* INCLUDES_CONFIG_ADC_CFG_H_ */
