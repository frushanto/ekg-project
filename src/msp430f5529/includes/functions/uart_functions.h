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

/* Function declarations */
void Uart_EKG_Kurve_KZ(uint16_t adc_value);
void Uart_EKG_Kurve_LZ(uint16_t adc_value);
void Test_Plus_Eins();
void Test_Minus_Eins();
void Clear_Waveform_KZ();
void Clear_Waveform_LZ();
void Uart_Page_One();

#endif /* INCLUDES_FUNCTIONS_UART_FUNCTIONS_H_ */
