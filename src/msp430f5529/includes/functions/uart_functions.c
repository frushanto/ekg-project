/*
 * uart_functions.c
 *
 *  Created on: 17.01.2021
 *      Author: hannes
 */
#include "includes/functions/uart_functions.h"

uint16_t test_plus_minus = 80;

void Uart_ECG_Wave_ST(uint16_t adc_value)
{
    adc_value = (adc_value / 8) - 100;
    uart_transmit_data_start("add 1,0,");
    uart_transmit_data_value(adc_value);
    uart_transmit_data_end();
}

void Uart_ECG_Wave_LT(uint16_t adc_value)
{
    adc_value = (adc_value / 8) - 100;
    uart_transmit_data_start("add 13,0,");
    uart_transmit_data_value(adc_value);
    uart_transmit_data_end();
}

void Test_Plus_Eins()
{
    test_plus_minus = test_plus_minus + 1;
    uart_transmit_data_start("page3.puls.val=");
    uart_transmit_data_value(test_plus_minus);
    uart_transmit_data_end();
}

void Test_Minus_Eins()
{
    test_plus_minus = test_plus_minus - 1;
    uart_transmit_data_start("page3.puls.val=");
    uart_transmit_data_value(test_plus_minus);
    uart_transmit_data_end();
}

void Clear_Wave_ST()
{
    uart_transmit_data_start("cle 1,0");
    uart_transmit_data_end();
}

void Clear_Wave_LT()
{
    uart_transmit_data_start("cle 13,0");
    uart_transmit_data_end();
}

void Short_ECG_Error()
{
    uart_transmit_data_start("page 5");
    uart_transmit_data_end();
}

void Long_ECG_Error()
{
    uart_transmit_data_start("page 6");
    uart_transmit_data_end();
}

void ST_ECG()
{
    Start_ADC();
//    Uart_ECG_Wave_ST(g_adc_result);
    UART_serialplot(g_adc_result, 0); // Hier bpm ans Display senden
    calculate_bpm_ST();
}

void LT_ECG()
{
    Start_ADC();
    Uart_ECG_Wave_LT(g_adc_result);
    calculate_bpm_LT();
}
