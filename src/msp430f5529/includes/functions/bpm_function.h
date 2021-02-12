/*
 * bpm_function.h
 *
 *  Created on: 16.01.2021
 *      Author: hannes
 */

#ifndef INCLUDES_FUNCTIONS_BPM_FUNCTION_H_
#define INCLUDES_FUNCTIONS_BPM_FUNCTION_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include <includes/config/uart_cfg.h>
#include "main.h"

/* Function declarations */
void calculate_bpm_ST();
void calculate_bpm_LT();

/* Global VARs */
// extern uint16_t g_adc_result;

#endif /* INCLUDES_FUNCTIONS_BPM_FUNCTION_H_ */