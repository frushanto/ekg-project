#include "includes/functions/adc_functions.h"

/* Akku calculations
 *
 * Akku max = 4,2V
 * Akku min = 3,3V
 * 
 * -> ADC Bereich
 *
 * 0 - 4095 Werte
 *
 * 3720 ADC Wert Max
 * 2720 ADC Wert Min -> calculate OFFSET
 *
 * -> After offset
 *
 * ADC Akku Max = 1000
 * ADC Akku Min = 0
 *
 *  */

// Local vars
static uint8_t cnt_akkuaverage = 0;
uint32_t g_akku_percentage = 0;
static uint32_t akku_averageValue = 0;
static uint8_t pre_akku_percentage = 100;

const uint16_t adc_akku_offset = 27200;
const uint16_t adc_akku_divider = 100;

#define ADC_AKKU_SEC 3

void ADC_Akku_Average_Value()
{
    if (g_sys_state == IDLE_STATE)
    {
        Start_ADC();
    }

    if (g_akku_vol)
    {

        akku_averageValue += g_akku_vol;
        cnt_akkuaverage++;

        if (cnt_akkuaverage == ADC_AKKU_SEC)
        {
            cnt_akkuaverage = 0;
            akku_averageValue /= ADC_AKKU_SEC;
            g_akku_percentage = (((akku_averageValue * 10) - adc_akku_offset) / adc_akku_divider);
            akku_averageValue = 0;
            if (g_akku_percentage < 1)
            {
                g_akku_percentage = 1;
            }
            if (g_akku_percentage > 100)
            {
                g_akku_percentage = 100;
            }
            if (g_akku_percentage > pre_akku_percentage)
            {
                g_akku_percentage = pre_akku_percentage;
            }

            pre_akku_percentage = g_akku_percentage;

            uart_transmit_data_start("page0.akku.val=");
            uart_transmit_data_value(g_akku_percentage);
            uart_transmit_data_end();

            if (g_akku_percentage <= 20)
            {
                g_buzzer_20_percent = TRUE;
            }
        }
    }
}
