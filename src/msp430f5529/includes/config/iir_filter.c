/*****************************************\
Author: Erik Thüry    Date: 05.12.2020

Für den IIR-Butterworth-Filter wurden mit Matlab 14 Koeffizienten berechnet
Die Nyquistfrequenz beträgt 500Hz (Samplingfrequenz 1000Hz / 2)
Die untere Grenzfrequenz des Stopbandes liegt bei 0,085*Nyquistfrequenz (42,5Hz)
Die obere Grenzfrequenz liegt bei 0,115*Nyquistfrequenz (57,5Hz)
\*****************************************/


#include <iir_filter.h>

static double w_pres[iir_filter_coef - 1] = {0}, w_past[iir_filter_coef - 1] = {0};
static double a_iir[iir_filter_coef], b_iir[iir_filter_coef];


void iir_filter_init()
{

    //iir filter coefficients

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

    //

}




double iir_filter(int new_sample)
{
//    extern double w_pres, w_past;
//    extern double b_iir, a_iir;

    double output = b_iir[1] * new_sample + w_past[0];

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

    return output;
}








