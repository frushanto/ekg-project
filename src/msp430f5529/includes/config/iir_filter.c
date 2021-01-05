/*****************************************\
Author: Erik Thüry    Date: 05.12.2020
Für den IIR-Butterworth-Filter wurden mit Matlab 14 Koeffizienten berechnet
Die Nyquistfrequenz betraegt 500Hz (Samplingfrequenz 1000Hz / 2)
Die untere Grenzfrequenz des Stopbandes liegt bei 0,085*Nyquistfrequenz (42,5Hz)
Die obere Grenzfrequenz liegt bei 0,115*Nyquistfrequenz (57,5Hz)
\*****************************************/


#include <iir_filter.h>

#define iir_filter_coef 7   // für 7 filterkoeffizienten

//#define iir_filter_coef 9   // für 9 filterkoeffizienten

float w_pres[iir_filter_coef - 1] = {0}, w_past[iir_filter_coef - 1] = {0};
float a_iir[iir_filter_coef], b_iir[iir_filter_coef];
float output = 0;

// 7 filterkoeffizienten

void iir_filter_init()
{

    //iir filter coefficients


    // coefficients for 500Hz
//    a_iir[0] = 1;
//    a_iir[1] = -4.56983004780163;
//    a_iir[2] = 9.59809982642961;
//    a_iir[3] = -11.5803074886572;
//    a_iir[4] = 8.46229125889880;
//    a_iir[5] = -3.55221316006272;
//    a_iir[6] = 0.685535977284665;
//
//    b_iir[0] = 0.827971295622401;
//    b_iir[1] = -4.03697332458995;
//    b_iir[2] = 9.04499223568442;
//    b_iir[3] = -11.6284040473422;
//    b_iir[4] = 9.04499223568442;
//    b_iir[5] = -4.03697332458995;
//    b_iir[6] = 0.827971295622401;

    // coefficients for 1kHz
    a_iir[0] = 1;
    a_iir[1] = -5.533277645806018;
    a_iir[2] = 13.022281228776492;
    a_iir[3] = -16.665570591553692;
    a_iir[4] = 12.228727799005487;
    a_iir[5] = -4.879507630525537;
    a_iir[6] = 0.828146275386264;

    b_iir[0] = 0.910025430690818;
    b_iir[1] = -5.198684885372917;
    b_iir[2] = 12.629552220962562;
    b_iir[3] = -16.680986097277930;
    b_iir[4] = 12.629552220962562;
    b_iir[5] = -5.198684885372917;
    b_iir[6] = 0.910025430690818;
}




uint16_t iir_filter(uint16_t new_sample)
{

    output = b_iir[0] * (float)new_sample + w_past[0];

    w_pres[5] = b_iir[6] * new_sample - a_iir[6] * output;
    w_pres[4] = b_iir[5] * new_sample - a_iir[5] * output + w_past[5];
    w_pres[3] = b_iir[4] * new_sample - a_iir[4] * output + w_past[4];
    w_pres[2] = b_iir[3] * new_sample - a_iir[3] * output + w_past[3];
    w_pres[1] = b_iir[2] * new_sample - a_iir[2] * output + w_past[2];
    w_pres[0] = b_iir[1] * new_sample - a_iir[1] * output + w_past[1];

    w_past[5] = w_pres[5];
    w_past[4] = w_pres[4];
    w_past[3] = w_pres[3];
    w_past[2] = w_pres[2];
    w_past[1] = w_pres[1];
    w_past[0] = w_pres[0];

    return (uint16_t)output;
}


/*
void iir_filter_init()
{
    a_iir[0] = 1;
    a_iir[1] = -7.38246205150448;
    a_iir[2] = 24.1980032731298;
    a_iir[3] = -45.9680525468639;
    a_iir[4] = 55.3363818639147;
    a_iir[5] = -43.2214964084978;
    a_iir[6] = 21.3928466606793;
    a_iir[7] = -6.13676514867858;
    a_iir[8] = 0.781618740278998;

    b_iir[0] = 0.884092042847053;
    b_iir[1] = -6.73404762170722;
    b_iir[2] = 22.7711007446172;
    b_iir[3] = -44.6203404549351;
    b_iir[4] = 55.3984649608141;
    b_iir[5] = -44.6203404549351;
    b_iir[6] = 22.7711007446172;
    b_iir[7] = -6.73404762170722;
    b_iir[8] = 0.884092042847053;
}

uint16_t iir_filter(uint16_t new_sample)
{

    output = b_iir[0] * (float)new_sample + w_past[0];

    w_pres[7] = b_iir[8] * new_sample - a_iir[8] * output;
    w_pres[6] = b_iir[7] * new_sample - a_iir[7] * output + w_past[7];
    w_pres[5] = b_iir[6] * new_sample - a_iir[6] * output + w_past[6];
    w_pres[4] = b_iir[5] * new_sample - a_iir[5] * output + w_past[5];
    w_pres[3] = b_iir[4] * new_sample - a_iir[4] * output + w_past[4];
    w_pres[2] = b_iir[3] * new_sample - a_iir[3] * output + w_past[3];
    w_pres[1] = b_iir[2] * new_sample - a_iir[2] * output + w_past[2];
    w_pres[0] = b_iir[1] * new_sample - a_iir[1] * output + w_past[1];

    w_past[7] = w_pres[7];
    w_past[6] = w_pres[6];
    w_past[5] = w_pres[5];
    w_past[4] = w_pres[4];
    w_past[3] = w_pres[3];
    w_past[2] = w_pres[2];
    w_past[1] = w_pres[1];
    w_past[0] = w_pres[0];

    return (uint16_t)output;
}
*/

