#include "includes/functions/adc_functions.h"

#define VALUES      10



uint32_t akkuAverageValue = 0;
uint8_t cnt = 0;


// uint16_t ADC_AkkuPercentage(uint16_t akkuValue) {

//     akkuAverageValue += akkuValue;
//     // akkuAverageValue = ((akkuAverageValue * 10) - adc_akku_offset)/adc_akku_divider;
//     return akkuAverageValue;
// }