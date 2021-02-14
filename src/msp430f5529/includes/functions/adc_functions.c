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
 * 2,620V = 3577 ADC Wert
 * 2,121V = 2895 ADC Wert = Offset
 *
 * -> After offset
 *
 * ADC Akku Max = 682
 * ADC Akku Min = 0
 *
 *
 * do not believe this -> Akku = 3,51V = 23,3% = 3004 ADC
 *
 * Akku max = 4,2V
 * Akku min = 3,3V
 *
 *  */

// Local vars
uint8_t cnt_akkuaverage = 0;
uint32_t akku_percentage = 0;
uint32_t akku_averageValue = 0;

const uint16_t adc_akku_offset = 28950;     // To be tested
const uint16_t adc_akku_divider = 68;       // To be tested

#define ADC_AKKU_SEC        10

void ADC_Akku_Average_Value(){
    if (g_timer_1sec_flag)
    {
        g_timer_1sec_flag = 0;

        Start_ADC();
        if(g_akku_vol == 0){
            Start_ADC();
        }
        send_bt_value(g_akku_vol);  // Send ADC Value of Akku to Bluetooth

        akku_averageValue += g_akku_vol;
        cnt_akkuaverage++;

        if (cnt_akkuaverage == ADC_AKKU_SEC)
        {
            cnt_akkuaverage = 0;
            akku_averageValue /= ADC_AKKU_SEC;
            akku_percentage = (((akku_averageValue * 10) - adc_akku_offset) / adc_akku_divider);
            akku_averageValue = 0;
            uart_transmit_data_start("page0.akku.val=");
            uart_transmit_data_value(akku_percentage);
            uart_transmit_data_end();
        }

    }
}

