/*
 * bpm_function.c
 *
 *  Created on: 16.01.2021
 *      Author: hannes
 */
#include "includes/functions/bpm_function.h"

void calculate_bpm_ST()
{
    static uint16_t bpm = 60;
    static uint16_t millisecs = 0;
    static uint16_t threshold_ecg_value = 2200;
    static uint16_t maximum_ecg_value = 0;
    static uint16_t minimum_ecg_value = 4095;
    static uint16_t watchdog_ecg = 0;

    watchdog_ecg ++;
    if (g_adc_result <= threshold_ecg_value)
    {
        millisecs++;
    }

    else if ((g_adc_result > threshold_ecg_value) && millisecs)
    {
        bpm = (uint16_t) (60000 / millisecs);




        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);

        watchdog_ecg = 0;
        millisecs = 0;
        maximum_ecg_value = 0;
        minimum_ecg_value = 4095;
    }

    if (g_adc_result < minimum_ecg_value)
    {
        minimum_ecg_value = g_adc_result;
    }

    if (g_adc_result > maximum_ecg_value)
    {
        maximum_ecg_value = g_adc_result;
    }

    if (watchdog_ecg > 2000)
    {
        watchdog_ecg = 1000;
        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);
        maximum_ecg_value = 0;
        minimum_ecg_value = 4095;
    }

    if (g_timer_1sec_flag)
    {
        g_timer_1sec_flag = 0;
        uint8_t medianValue = (uint8_t) MEDIANFILTER_Insert(&medianFilter, bpm);
        UART_serialplot(g_adc_result, medianValue); // Hier bpm ans Display senden
    }
}

void calculate_bpm_LT()
{
    uint8_t bpm = 60;
    static uint16_t millisecs = 0;
    static uint16_t threshold_ecg_value = 2200;
    static uint16_t maximum_ecg_value = 0;
    static uint16_t minimum_ecg_value = 4095;
    static uint16_t watchdog_ecg = 0;

    watchdog_ecg ++;
    if (g_adc_result <= threshold_ecg_value)
    {
        millisecs++;
    }

    else if ((g_adc_result > threshold_ecg_value) && millisecs)
    {
        bpm = (uint16_t) (60000 / millisecs);
        if ((bpm < 121) && (bpm > 39))
        {
            uart_transmit_data_start("page3.puls.val=");
            uart_transmit_data_value (bpm);
            uart_transmit_data_end();
        }
        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);

        watchdog_ecg = 0;
        millisecs = 0;
        maximum_ecg_value = 0;
        minimum_ecg_value = 4095;
    }

    if (g_adc_result < minimum_ecg_value)
    {
        minimum_ecg_value = g_adc_result;
    }

    if (g_adc_result > maximum_ecg_value)
    {
        maximum_ecg_value = g_adc_result;
    }

    if (watchdog_ecg > 2000)
    {
        watchdog_ecg = 1000;
        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);
        maximum_ecg_value = 0;
        minimum_ecg_value = 4095;
    }
}
