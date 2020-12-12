/*****************************************\
Author: Erik Thüry    Date: 05.12.2020

Für den FIR-Filter wurden mit Matlab 201 Koeffizienten berechnet
Die Nyquistfrequenz beträgt 500Hz (Samplingfrequenz 1000Hz / 2)
Die untere Grenzfrequenz des Stopbandes liegt bei 0,085*Nyquistfrequenz (42,5Hz)
Die obere Grenzfrequenz liegt bei 0,115*Nyquistfrequenz (57,5Hz)
\*****************************************/

#include <fir_filter.h>

//#define filter_coef 101
//#define nc (filter_coef * 2) - 1

#define filter_coef 201

static float b[filter_coef];
static uint16_t circular_buffer[filter_coef];

void fir_filter_init()
{
    uint8_t i;

    //filter coefficients
    b[0] = 0.000509081586485681;
    b[1] = 0.000489899053280178;
    b[2] = 0.000423142988221058;
    b[3] = 0.000313201258203320;
    b[4] = 0.000168265387671783;
    b[5] = 0;
    b[6] = -0.000177079293983667;
    b[7] = -0.000346559913047284;
    b[8] = -0.000491423326911605;
    b[9] = -0.000595622859842229;
    b[10] = -0.000645831117967535;
    b[11] = -0.000633174197139673;
    b[12] = -0.000554720449250790;
    b[13] = -0.000414472845837797;
    b[14] = -0.000223636421434704;
    b[15] = 0;
    b[16] = 0.000233619646620438;
    b[17] = 0.000451785419561560;
    b[18] = 0.000629466952285865;
    b[19] = 0.000745323624931788;
    b[20] = 0.000784789335777460;
    b[21] = 0.000742475862746039;
    b[22] = 0.000623457814564558;
    b[23] = 0.000443125646374867;
    b[24] = 0.000225483257968728;
    b[25] = 0;
    b[26] = -0.000202629854015477;
    b[27] = -0.000355252835662062;
    b[28] = -0.000438833094384236;
    b[29] = -0.000446078664141710;
    b[30] = -0.000383426344207576;
    b[31] = -0.000270885301569308;
    b[32] = -0.000139533159115237;
    b[33] = -0.0000268187748289001;
    b[34] = 0.0000297973879915103;
    b[35] = 0;
    b[36] = -0.000132526618388822;
    b[37] = -0.000364730845947638;
    b[38] = -0.000671404133538106;
    b[39] = -0.00100578304630584;
    b[40] = -0.00130391793957654;
    b[41] = -0.00149263359739897;
    b[42] = -0.00150031910116767;
    b[43] = -0.00126924364864915;
    b[44] = -0.000767689611569236;
    b[45] = 0;
    b[46] = 0.000987293030884232;
    b[47] = 0.00210474064557105;
    b[48] = 0.00322552112249575;
    b[49] = 0.00419731534793870;
    b[50] = 0.00485966306674662;
    b[51] = 0.00506505742148606;
    b[52] = 0.00470129344823023;
    b[53] = 0.00371211475660673;
    b[54] = 0.00211309522004317;
    b[55] = 0;
    b[56] = -0.00245241005470323;
    b[57] = -0.00500209495204516;
    b[58] = -0.00736162801912679;
    b[59] = -0.00922852885661483;
    b[60] = -0.0103210107404734;
    b[61] = -0.0104151077192162;
    b[62] = -0.00937857857091559;
    b[63] = -0.00719696938089845;
    b[64] = -0.00398780070562949;
    b[65] = 0;
    b[66] = 0.00440268129199344;
    b[67] = 0.00877361881792295;
    b[68] = 0.0126280154183696;
    b[69] = 0.0154959431959189;
    b[70] = 0.0169779574220786;
    b[71] = 0.0167967752580206;
    b[72] = 0.0148385350816070;
    b[73] = 0.0111779793261917;
    b[74] = 0.00608348014008412;
    b[75] = 0;
    b[76] = -0.00648938761252334;
    b[77] = -0.0127201930099786;
    b[78] = -0.0180158471870255;
    b[79] = -0.0217623097993214;
    b[80] = -0.0234795772992728;
    b[81] = -0.0228816766294315;
    b[82] = -0.0199176898479814;
    b[83] = -0.0147882445359179;
    b[84] = -0.00793453282060689;
    b[85] = 0;
    b[86] = 0.00823198128643247;
    b[87] = 0.0159182404890172;
    b[88] = 0.0222452712090301;
    b[89] = 0.0265182179757492;
    b[90] = 0.0282393510045536;
    b[91] = 0.0271669277541852;
    b[92] = 0.0233473576844566;
    b[93] = 0.0171164722656051;
    b[94] = 0.00906913374973182;
    b[95] = 0;
    b[96] = -0.00917841949427305;
    b[97] = -0.0175316223097354;
    b[98] = -0.0242024120309254;
    b[99] = -0.0285026685235791;
    b[100] = 0.969591976884882;

    for(i = 0; i < 100; i++)
    {
        b[200 - i] = b[i];
    }


    // circular buffer stores the last 201 ADC values
    // at the beginning of the measurement the buffer has to be initialised with 0

    for(i = 0; i < filter_coef; i++)    //   (i = nc; i >= 0; i--)
    {
        circular_buffer[i] = 0;
    }
}

// Nach jedem neuen Wert vom ADC wird die Funktion fir_filter aufgerufen
// Im circular_buffer stehen die letzten 201 Samplewerte, diese werden mit
// den Filterkoeffizienten multipliziert und aufsummiert und ergeben den neuen gefilterten Signalwert

//double fir_filter(int new_sample)
//{
//    static int index = 0;
//    double output = 0;
//    int i;
//
//    circular_buffer[index] = new_sample;
//
//    for (i = 0; i < filter_coef - 1; i++)
//    {
//        output += b[i] * (circular_buffer[(index + i) % nc] + circular_buffer[(index + nc - i - 1) % nc]);
//    }
//    output += b[100] * circular_buffer[(100 + index) % nc];
//
//    index = (++index) % nc;  // index läuft von 0 bis 200 // index überlauf einfacher gestalten
//
//    return output;  // keine impliziten Umwandlungen
//}


uint16_t fir_filter(uint16_t new_sample)
{
    static uint8_t index = 0;
    float sum = 0;
    uint8_t i;

    circular_buffer[index] = new_sample;

    index = (index + 1) % filter_coef;

    for(i = 0; i < filter_coef; i++)
    {
        sum += b[i] * circular_buffer[(index + i) % filter_coef];
    }

    if(index == 200)
    {
        index = 200;
    }
    return (uint16_t)sum;
}






/*
 * int fir_filter(int new_sample)
{​​​​
static int index = 0;
static uint8_t first_run = 1;
double y = 0;

if (first_run)
fir_filter_init(), first_run = 0;

circular_buffer[index] = (double) new_sample;

index++;
if (index >= nc)
index = 0;

for (int i = 0; i < nc; i++)
{​​​​
y += b[i] * circular_buffer[(index + i) % nc];
}​​​​

return (int)y;
}*/

