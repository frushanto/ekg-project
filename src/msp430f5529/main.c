#include "main.h"

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);

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

    while(1) {
        if(timer_start_stop == 1 && page_two_start_stop == 1) {
            timer_start_stop = 0;
            Test_ADC();
            if(adc_start_stop == 1) {
                adc_start_stop = 0;
                UART_ECG(adc_result);
                if(page_two_start_stop == 0) {
                    Clear_Waveform();
                }
            }
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
