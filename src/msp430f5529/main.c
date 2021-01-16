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
    Init_SPI();
    /* !!! For test purposes leave Init_MMC() line commented out!!! */
    //Init_MMC();
    EnableGlobalInterrupt();
    /* Init MSP430 END */

    uint16_t help_var = 0;
    uint16_t millisecs = 0;
    uint16_t test_puls = 0;

    while(1) {
        if(timer_1hz_flag == 1 && page_two_start_stop == 1) {
            timer_1hz_flag = 0;
            Test_ADC();
            if(adc_flag == 1) {
                adc_flag = 0;
                UART_ECG(adc_result);
//                UART_THRESHOLD();          //*****
                if(page_two_start_stop == 0) {          // Clear läuft auch am Display bei "Stop"
                    Clear_Waveform();
                }
            }
            /* Timer Display */
            if(timer_1sek_flag == 1) {
                timer_1sek_flag = 0;
                UART_Timer_Page_Two_Sec();
                UART_Timer_Page_Two_Min();
            }

            help_var = (adc_result / 8) - 100;
            /* TEST BPM */
            if(help_var <= 220) {
                millisecs++;
            }
            else if((help_var > 220) && millisecs)
            {
                test_puls = (uint16_t) (60000 / millisecs);
                millisecs = 0;
            }
            new_bpm(test_puls);
        }
        if(page_two_start_stop == 0) {
            UART_Timer_Reset();
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
