/*
 * bpm_function.c
 *
 *  Created on: 16.01.2021
 *      Author: hannes
 */


#include "bpm_function.h"

void calculate_bpm()
{
    watchdog_ecg ++;
    if (adc_result <= threshold_ecg_value)
    {
        millisecs++;
    }

    else if ((adc_result > threshold_ecg_value) && millisecs)
    {
        bpm = (uint16_t) (60000 / millisecs);
        if ((bpm < 121) && (bpm > 39))
        {
            uart_transmit_data_start("puls.val=");
            uart_transmit_data_value (bpm);
            uart_transmit_data_end();
        }
        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);

        watchdog_ecg = 0;
        millisecs = 0;
        maximum_ecg_value = 0;
        minimum_ecg_value = 4095;
    }

    if (adc_result < minimum_ecg_value)
    {
        minimum_ecg_value = adc_result;
    }

    if (adc_result > maximum_ecg_value)
    {
        maximum_ecg_value = adc_result;
    }

    if (watchdog_ecg > 2000)
    {
        watchdog_ecg = 1000;
        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);
        maximum_ecg_value = 0;
        minimum_ecg_value = 4095;
    }
}
