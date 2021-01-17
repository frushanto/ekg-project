/*
 * uart_functions.c
 *
 *  Created on: 17.01.2021
 *      Author: hannes
 */
#include "includes/functions/uart_functions.h"

uint16_t test_plus_minus = 80;

void Uart_EKG_Kurve_KZ(uint16_t adc_value){
    adc_value = (adc_value / 8) - 100;
    uart_transmit_data_start("add 1,0,");
    uart_transmit_data_value(adc_value);
    uart_transmit_data_end();
}

void Uart_EKG_Kurve_LZ(uint16_t adc_value){
    adc_value = (adc_value / 8) - 100;
    uart_transmit_data_start("add 13,0,");
    uart_transmit_data_value(adc_value);
    uart_transmit_data_end();
}

void Test_Plus_Eins(){
    test_plus_minus = test_plus_minus + 1;
    uart_transmit_data_start("page3.puls.val=");
    uart_transmit_data_value(test_plus_minus);
    uart_transmit_data_end();
}

void Test_Minus_Eins(){
    test_plus_minus = test_plus_minus - 1;
    uart_transmit_data_start("page3.puls.val=");
    uart_transmit_data_value(test_plus_minus);
    uart_transmit_data_end();
}

void Clear_Waveform_KZ(){
    uart_transmit_data_start("cle 1,0");
    uart_transmit_data_end();
}

void Clear_Waveform_LZ(){
    uart_transmit_data_start("cle 13,0");
    uart_transmit_data_end();
}

void Uart_Page_One(){
    uart_transmit_data_start("page 1");
    uart_transmit_data_end();
}
