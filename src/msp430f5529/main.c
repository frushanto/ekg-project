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
//    Init_SPI();
    /* !!! For test purposes leave Init_MMC() line commented out!!! */
    //Init_MMC();
    EnableGlobalInterrupt();
    /* Init MSP430 END */

    while (1)
    {
        if (g_timer_1hz_flag == 1 && g_page_two_start_stop == 1)
        {
            g_timer_1hz_flag = 0;
            Test_ADC();
            Uart_EKG_Kurve_KZ(g_adc_result);
            calculate_bpm_KZ();
            if (g_page_two_start_stop == 0)
            {
                Clear_Waveform_KZ();   // Clear läuft auch am Display bei "Stop"
            }
            /* Timer Display */
            /// TODO
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
