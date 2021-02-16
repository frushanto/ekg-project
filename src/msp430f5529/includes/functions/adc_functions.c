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
 * 3577 ADC Wert Max -> NOT FIXED
 * 2720 ADC Wert Min -> FIXED -> OFFSET
 *
 * -> After offset
 *
 * ADC Akku Max = 682 -> NOT FIXED
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
static uint8_t cnt_akkuaverage = 0;
static uint32_t akku_percentage = 0;
static uint32_t akku_averageValue = 0;

const uint16_t adc_akku_offset = 27200;     // FIXED
const uint16_t adc_akku_divider = 68;       // NOT FIXED

#define ADC_AKKU_SEC        10

void ADC_Akku_Average_Value(){

    Start_ADC();

    if (g_akku_vol)
    {

        // Send ADC Value of accumulator to Bluetooth
        send_bt_value(g_akku_vol);

        akku_averageValue += g_akku_vol;
        cnt_akkuaverage++;

        if (cnt_akkuaverage == ADC_AKKU_SEC)
        {
            cnt_akkuaverage = 0;
            akku_averageValue /= ADC_AKKU_SEC;
            akku_percentage = (((akku_averageValue * 10) - adc_akku_offset) / adc_akku_divider);
            akku_averageValue = 0;
//            if(akku_percentage < 1){
//                akku_percentage = 1;
//            }
//            if(akku_percentage > 100){
//                akku_percentage = 100;
//            }
            uart_transmit_data_start("page0.akku.val=");
            uart_transmit_data_value(akku_percentage);
            uart_transmit_data_end();
        }
    }
}

