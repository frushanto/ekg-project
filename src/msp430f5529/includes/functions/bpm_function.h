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

/*Defines*/
#define tachy_brady_cnt_threshold 20
#define tachy_threshold 100  // zum Testen kannst einfach die obere Schwelle runter oder die untere Schwelle hochsetzten
#define brady_threshold 60


#endif /* INCLUDES_FUNCTIONS_BPM_FUNCTION_H_ */
