/*
 * bpm_function.c
 *
 *  Created on: 16.01.2021
 *      Author: hannes
 */
#include "includes/functions/bpm_function.h"


uint8_t bpm = 0;
static uint16_t bpm_cnt = 0;
static uint16_t threshold_ecg_value = 2200;
static uint16_t maximum_ecg_value = 0;
static uint16_t minimum_ecg_value = 4095;
static uint16_t watchdog_ecg = 0;
static uint8_t tachy_cnt = 0;
static uint8_t brady_cnt = 0;

void calculate_bpm_ST()
{
    watchdog_ecg ++;    // 250Hz 
    // if (g_adc_result <= threshold_ecg_value)
    // {
        bpm_cnt++;
    // }else 
    if ((g_adc_result > threshold_ecg_value) && bpm_cnt > 100)
    {
        bpm = (uint16_t) ((G_SAMPLE_RATE*60) / bpm_cnt);


        /*Abfrage auf Tachykardie oder Bradykardie*/
        if (bpm < brady_threshold)
        {
            brady_cnt = brady_cnt + 1;
            if (brady_cnt >= tachy_brady_cnt_threshold)
            {
                //Hier kommt das Displaykommando f�r die Warnung Bradykardie rein
            }
        }
        else if (bpm > tachy_threshold)
        {
            tachy_cnt = tachy_cnt + 1;
            if (tachy_cnt >= tachy_brady_cnt_threshold)
            {
                // Hier kommt das Displaykommando f�r die Warnung Tachykardie rein
            }
        }
        else
        {
            brady_cnt = 0;
            tachy_cnt = 0;
        }
        /*Ende der Abfrage*/

        int medianValue = MEDIANFILTER_Insert(&medianFilter, bpm);

        // Send BPM to Display
        uart_transmit_data_start("page2.puls.val=");
        uart_transmit_data_value (medianValue);
        uart_transmit_data_end();

        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);

        watchdog_ecg = 0;
        bpm_cnt = 0;
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

    if (watchdog_ecg > 500)
    {
        watchdog_ecg = 250;
        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);
        maximum_ecg_value = 0;
        minimum_ecg_value = 4095;
    }
}

void calculate_bpm_LT()
{
    watchdog_ecg ++;    // 250Hz
    // if (g_adc_result <= threshold_ecg_value)
    // {
        bpm_cnt++;
    // }else 
    if ((g_adc_result > threshold_ecg_value) && bpm_cnt > 100)
    {
        bpm = (uint16_t) ((G_SAMPLE_RATE*60) / bpm_cnt);

        int medianValue = MEDIANFILTER_Insert(&medianFilter, bpm);

        // Send BPM to Display
        uart_transmit_data_start("page3.puls.val=");
        uart_transmit_data_value (medianValue);
        uart_transmit_data_end();

        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);

        watchdog_ecg = 0;
        bpm_cnt = 0;
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

    if (watchdog_ecg > 500)
    {
        watchdog_ecg = 250;
        threshold_ecg_value = maximum_ecg_value - 0.2 * (maximum_ecg_value - minimum_ecg_value);
        maximum_ecg_value = 0;
        minimum_ecg_value = 4095;
    }
}
