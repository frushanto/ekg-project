#include "main.h"

/* STATE MACHINE */
#define CASE_BADKEY                     -1
#define CASE_SYS_INIT                   1
#define CASE_SET_VARS                   2
#define CASE_ECG_SHORT                  3
#define CASE_ECG_LONG                   4
#define CASE_ENEGRY_SAVING_MODE         5
#define CASE_IDLE_STATE                 6
#define CASE_SYS_ERROR                  7
#define CASE_SYS_DIRTY_START            8

typedef struct { 
    char *case_key; 
    int case_val; 
} STATE_MACHINE_s;

static STATE_MACHINE_s lookuptable[] = {
    { "CASE_SYS_INIT", CASE_SYS_INIT }, 
    { "CASE_SET_VARS", CASE_SET_VARS }, 
    { "CASE_ECG_SHORT", CASE_ECG_SHORT }, 
    { "CASE_ECG_LONG", CASE_ECG_LONG },
    { "CASE_ENEGRY_SAVING_MODE", CASE_ENEGRY_SAVING_MODE },
    { "CASE_IDLE_STATE", CASE_IDLE_STATE },
    { "CASE_SYS_ERROR", CASE_SYS_ERROR },
    { "CASE_SYS_DIRTY_START", CASE_SYS_DIRTY_START }
};

#define NKEYS (sizeof(lookuptable)/sizeof(STATE_MACHINE_s))

int Get_KeyFromString(char *key) {
    int i;
    for (i = 0; i < NKEYS; i++) {
        const STATE_MACHINE_s *case_sym = lookuptable[i];
        if (strcmp(case_sym->case_key, key) == 0)
            return case_sym->case_val;
    }
    return CASE_BADKEY;
}

char[] g_sys_state = "CASE_SYS_INIT";

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);

/* Function definitions */
void main(void) {
    switch (Get_KeyFromString(g_sys_state)) {
        case CASE_SYS_INIT:
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

            g_sys_state = CASE_SET_VARS;        // Change state
            break;

        case CASE_SET_VARS:
        {
            uint16_t help_var = 0;
            uint16_t millisecs = 0;
            uint16_t test_puls = 0;
        }

            g_sys_state = CASE_ECG_SHORT;       // Change state
            break;

        case CASE_ECG_SHORT:
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
        // if(page_two_start_stop == 0) {
        //     UART_Timer_Reset();
        // }
            }

            g_sys_state = CASE_IDLE_STATE;      // Change state
            break;

        case CASE_IDLE_STATE:

            while(1) {
                // Endless loop
            }
            break;

        case CASE_BADKEY:
            // Something went wrong
            while(1) {
                // Endless loop
            }
            break;
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
