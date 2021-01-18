#include "main.h"

/* State machine */
typedef enum { 
    SYS_INIT,
    SET_VARS,
    ECG_SHORT,
    ECG_LONG,
    ENERGY_SAVING_MODE,
    IDLE_STATE,
    SYS_ERROR,
    SYS_DIRTY_START,
    SYS_BAD_KEY
} STATE_MACHINE_e;

STATE_MACHINE_e g_sys_state = SYS_INIT;

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);

/* Function definitions */
void main(void) {
    switch (g_sys_state) {
        case SYS_INIT:
            /* Init MSP430 BEGIN */
            Init_Watchdog();
            Init_GPIO();
            Init_CLK();
            Init_Timers();
            Init_UART();
            Init_ADC();
            // Init_SPI();
            /* !!! For test purposes leave Init_MMC() line commented out!!! */
            //Init_MMC();
            EnableGlobalInterrupt();
            /* Init MSP430 END */

            g_sys_state = SET_VARS;        // Change state

        case SET_VARS:;
            uint16_t g_help_var = 0;
            uint16_t g_millisecs = 0;
            uint16_t g_test_puls = 0;
            g_sys_state = ECG_SHORT;       // Change state

        case ECG_SHORT:
            while(1) {
                if(timer_1hz_flag == 1 && page_two_start_stop == 1) {
                    timer_1hz_flag = 0;
                    Test_ADC();
                    if(adc_flag == 1) {
                        adc_flag = 0;
                        UART_ECG(adc_result);
                        // UART_THRESHOLD();          //*****
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

                    g_help_var = (adc_result / 8) - 100;
                    /* TEST BPM */
                    if(g_help_var <= 220) {
                        g_millisecs++;
                    }
                    else if((g_help_var > 220) && g_millisecs)
                    {
                        g_test_puls = (uint16_t) (60000 / g_millisecs);
                        g_millisecs = 0;
                    }
                    new_bpm(g_test_puls);
                }
                // if(page_two_start_stop == 0) {
                //     UART_Timer_Reset();
                // }
            }
            g_sys_state = IDLE_STATE;      // Change state

        case IDLE_STATE:

            while(1) {
                // Endless loop
            }

        case SYS_BAD_KEY:
            // Something went wrong
            while(1) {
                // Endless loop
            }

        default:
            while(1) {
                // Endless loop
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
