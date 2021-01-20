/*
 * uart_functions.h
 *
 *  Created on: 17.01.2021
 *      Author: hannes
 */

#ifndef INCLUDES_FUNCTIONS_UART_FUNCTIONS_H_
#define INCLUDES_FUNCTIONS_UART_FUNCTIONS_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include <includes/config/uart_cfg.h>
#include <includes/functions/bpm_function.h>
#include <includes/config/adc_cfg.h>

/* Function declarations */
void Uart_ECG_Wave_ST(uint16_t adc_value);
void Uart_ECG_Wave_LT(uint16_t adc_value);
void Test_Plus_Eins();
void Test_Minus_Eins();
void Clear_Wave_ST();
void Clear_Wave_LT();
void Uart_ECG_Error();
void ST_ECG();
void LT_ECG();

// extern uint16_t g_adc_result;

#endif /* INCLUDES_FUNCTIONS_UART_FUNCTIONS_H_ */
