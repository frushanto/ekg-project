/*
 * iir_filter.h
 *
 *  Created on: 08.12.2020
 *      Author: Erik
 */

#ifndef INCLUDES_CONFIG_IIR_FILTER_H_
#define INCLUDES_CONFIG_IIR_FILTER_H_

#include <msp430.h>
#include <driverlib.h>
#include <std_types.h>

#define iir_filter_coef 7

/*function declaration*/
void iir_filter_init();
uint16_t iir_filter(uint16_t new_sample);


#endif /* INCLUDES_CONFIG_IIR_FILTER_H_ */
