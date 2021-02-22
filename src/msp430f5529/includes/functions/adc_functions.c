#include "includes/functions/adc_functions.h"

/* Akku calculations
 *
 * 2,620V im Maximalfall (ADC)
 * 2,121V im Minimalfall (ADC)
 *
 * -> ADC Bereich
 *
 * 0 - 4095 Werte
 * 3V = 4095
 * 0V = 0
 *
 * 3720 ADC Wert Max -> FIXED
 * 2720 ADC Wert Min -> FIXED -> OFFSET
 *
 * -> After offset
 *
 * ADC Akku Max = 1000 -> FIXED
 * ADC Akku Min = 0
 *
 *
 * 
 *
 * Akku max = 4,2V
 * Akku min = 3,3V
 *
 *  */

// Local vars
static uint8_t cnt_akkuaverage = 0;
uint32_t g_akku_percentage = 0;
static uint32_t akku_averageValue = 0;
static uint8_t pre_akku_percentage = 100;

const uint16_t adc_akku_offset = 27200;     // FIXED
const uint16_t adc_akku_divider = 100;       // NOT FIXED

#define ADC_AKKU_SEC        1

void ADC_Akku_Average_Value(){
    if(g_sys_state == IDLE_STATE)
    {
        Start_ADC();
    }

    if (g_akku_vol)
    {

        // Send ADC Value of accumulator to Bluetooth
        // send_bt_value(g_akku_vol);

        akku_averageValue += g_akku_vol;
        cnt_akkuaverage++;

        if (cnt_akkuaverage == ADC_AKKU_SEC)
        {
            cnt_akkuaverage = 0;
            akku_averageValue /= ADC_AKKU_SEC;
            g_akku_percentage = (((akku_averageValue * 10) - adc_akku_offset) / adc_akku_divider);
            akku_averageValue = 0;
           if(g_akku_percentage < 1){
               g_akku_percentage = 1;
           }
           if(g_akku_percentage > 100){
               g_akku_percentage = 100;
           }
           if(g_akku_percentage > pre_akku_percentage)
           {
               g_akku_percentage = pre_akku_percentage;
           }

            pre_akku_percentage = g_akku_percentage;


            uart_transmit_data_start("page0.akku.val=");
            uart_transmit_data_value(g_akku_percentage);
            uart_transmit_data_end();
        }
    }
}

