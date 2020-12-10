/*
 * fir_filter.h
 *
 *  Created on: 05.12.2020
 *      Author: Erik
 */

#ifndef INCLUDES_CONFIG_FIR_FILTER_H_
#define INCLUDES_CONFIG_FIR_FILTER_H_

#include <msp430.h>
#include <driverlib.h>
#include <std_types.h>

#define filter_coef 101
#define nc (filter_coef * 2) - 1

/*function declaration*/
void fir_filter_init();
uint16_t fir_filter(uint16_t new_sample);

#endif /* INCLUDES_CONFIG_FIR_FILTER_H_ */
