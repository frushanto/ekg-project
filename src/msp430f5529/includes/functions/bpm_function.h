/*
 * bpm_function.h
 *
 *  Created on: 16.01.2021
 *      Author: hannes
 */

#ifndef INCLUDES_FUNCTIONS_BPM_FUNCTION_H_
#define INCLUDES_FUNCTIONS_BPM_FUNCTION_H_

#include <std_types.h>
#include <msp430.h>
#include <driverlib.h>

uint8_t bpm = 60;
static uint16_t millisecs = 0;
static uint16_t threshold_ecg_value = 2200;
static uint16_t maximum_ecg_value = 0;
static uint16_t minimum_ecg_value = 4095;
static uint16_t watchdog_ecg = 0;

extern uint16_t adc_result;

#endif /* INCLUDES_FUNCTIONS_BPM_FUNCTION_H_ */
