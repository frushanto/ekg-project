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

uint16_t adc_result;

/* Function declarations */
void Init_ADC(void);
void Test_ADC(void);


#endif /* INCLUDES_CONFIG_ADC_CFG_H_ */
