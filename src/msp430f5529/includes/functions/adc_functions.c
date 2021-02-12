#include "includes/functions/adc_functions.h"

// Local vars
uint8_t cnt_akkuaverage = 0;
uint32_t akku_percentage = 0;
uint32_t akku_averageValue = 0;

//const uint16_t adc_akku_offset = 28950;     // To be tested
const uint16_t adc_akku_offset = 18950;
const uint16_t adc_akku_divider = 68;       // To be tested

#define ADC_AKKU_SEC        10

void ADC_Akku_Average_Value(){
    if (g_timer_1sec_flag)
    {
        g_timer_1sec_flag = 0;

        Start_ADC();
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
            //                uart_transmit_data_value ((g_akku_vol - ADC_OFFSET)/ADC_DIVIDER);
            //                uart_transmit_data_value (80);
            uart_transmit_data_end();
        }

    }
}

