#include "main.h"

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);


#define R_THRESHOLD 2200
#define puls_buffer 5

uint8_t bpm = 60;
uint16_t millisecs = 0;
uint16_t adc_value = 0;
//uint16_t maximum = 0;
uint8_t flag = 0;
uint16_t threshold = 2200;
uint16_t max = 0;
uint16_t min = 4095;
uint16_t watchdog_var = 0;
uint8_t one_sec = 0;
uint8_t adc_ready = 0;
bool enable_functionality = false;
uint8_t puls[puls_buffer] = { 0 };
uint8_t index = 0;
uint16_t puls_value_trx = 0;
uint8_t j = 0;
uint16_t akku_vol = 0;
uint16_t adc_result = 0;
bool buzzer_flag = false;


/* Function definitions */
void main(void) {
    /* Init MSP430 BEGIN */
    Init_Watchdog();
    Init_GPIO();
    Init_CLK();
    Init_Timers();
    Init_UART();
    Init_ADC();
    EnableGlobalInterrupt();
    /* Init MSP430 END */

    /* IIR Test */
//    iir_filter_init();
    /* FIR Test */
//    fir_filter_init();



    while(1) {


        if(adc_ready && enable_functionality)
        {
        	adc_ready = 0;


            watchdog_var++;
            Start_ADC();

            adc_value = adc_result;
            UART_serialplot(adc_result, akku_vol);  // nach UART_serialplot ist die Variable adc_result nicht mehr gültig. Warum auch immer
                                               // außerdem stürzt die MCU ab wenn bpm den Wert 100 erreicht ?!?!?!?!
                                               // beim Wert 99 ist die MCU nach etwa 5 min auch hängen geblieben

/*
            if(adc_value <= threshold)
            {
                millisecs++;
            }

            else if((adc_value > threshold) && (millisecs > 400))
            {
                bpm = (uint16_t) (60000 / millisecs);
//                if((bpm < 121) && (bpm > 39))
//                {
//                    UART_serialplot(adc_value, bpm); // Hier bpm ans Display senden
//                }
                threshold = max - 0.2 * (max - min);

                watchdog_var = 0;
                millisecs = 0;
                max = 0;
                min = 4095;
            }

            if(adc_value < min)
            {
                min = adc_value;
            }

            if(adc_value > max)
            {
                max = adc_value;
            }

            if(watchdog_var > 2000)
            {
                watchdog_var = 1000;
                threshold = max - 0.2 * (max - min);
                max = 0;
                min = 4095;
            }

            if(one_sec)
            {
                one_sec = 0;

//                puls_value_trx = 0;
//                puls[index] = bpm;
//                index++;
//                if(index >= puls_buffer)
//                   index = 0;
//
//                for(j = 0; j < puls_buffer; j++)
//                {
//                    puls_value_trx = puls_value_trx + puls[j];
//                }
//                puls_value_trx = puls_value_trx / puls_buffer;

                UART_serialplot(adc_value, bpm);

            }
            */
        }

    }
}

/*** Configure Watchdog Timer ***/
void Init_Watchdog(void) {
    WDT_A_hold(WDT_A_BASE);
}

/*** Global Interrupt Enable ***/
void EnableGlobalInterrupt() {
    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    // Enter interrupt
    __bis_SR_register(GIE);
    // For debugger
    __no_operation();
}
