/* TODOs 
- SD Module (Hannes & Ivan)
    - Test with LONG&SHORT ECG (test flags)
    - Test on PCB
- Gesamttest auf PCB (Hannes & Ivan)
- Display (Hannes)
    - Timer
    - Bluetooth
*/

#include "main.h"

/* GLOBAL VARs */
uint8_t g_timer_1khz_flag = 0;
uint8_t g_timer_1sec_flag = 0;
uint16_t g_adc_result = 0;
uint8_t g_short_ECG_flag = 0;
uint8_t g_long_ECG_flag = 0;
uint16_t g_akku_vol = 0;
uint8_t g_tmp_return = 0;
uint16_t g_writingCyclesCnt = 0;

/* For median filter */
#define NUM_ELEMENTS    7
sMedianFilter_t medianFilter;
static sMedianNode_t medianBuffer[NUM_ELEMENTS];
/* End median filter */

STATE_MACHINE_e g_sys_state = SYS_INIT;
/* END GLOBAL VARs */

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);

/* Function definitions */
void main(void) {
    while (1) {
        switch (g_sys_state) {

        case SYS_INIT:
            /* Init MSP430 BEGIN */
            Init_Watchdog();
            Init_GPIO();
            Init_CLK();
            Init_Timers();
            Init_UART();
            Init_ADC();
            Init_SPI();
            Init_FAT();            //mount, set directory to read from, assign file
            /* Init median filter */
            medianFilter.numNodes = NUM_ELEMENTS;
            medianFilter.medianBuffer = medianBuffer;
            MEDIANFILTER_Init(&medianFilter); // Init median filter
            EnableGlobalInterrupt();
            /* Init MSP430 END */
            
            g_sys_state = IDLE_STATE; // Change state
            break;

        case ECG_SHORT:
            if (g_timer_1khz_flag)
            {
                g_timer_1khz_flag = 0;
                ST_ECG();
                // Testing SD Card
                // Write in .csv
                SD_StartWriting();
                // Leave short ECG maually
                g_writingCyclesCnt++;
                if (g_writingCyclesCnt == 10) {
                    g_short_ECG_flag = 0;
                    g_writingCyclesCnt = 0;
                }
            }
            if (g_long_ECG_flag)
            {
                g_long_ECG_flag = 0;
                Short_ECG_Error();
            }
            if (!g_short_ECG_flag)
            {
                Clear_Wave_ST();
                // Stop writing in .csv
                SD_StopWriting();
                g_sys_state = IDLE_STATE;
            }
            break;

        case ECG_LONG:
            if (g_timer_1khz_flag)
            {
                g_timer_1khz_flag = 0;
                LT_ECG();
                // Write in .csv
                SD_StartWriting();
            }
            if (g_short_ECG_flag)
            {
                g_short_ECG_flag = 0;
                Long_ECG_Error();
            }
            if (!g_long_ECG_flag)
            {
                Clear_Wave_LT();
                // Stop writing in .csv
                SD_StopWriting();
                g_sys_state = IDLE_STATE;
            }
            break;

        case ENERGY_SAVING_MODE:
            break;

        case IDLE_STATE:
            // Testing SD Card
            g_short_ECG_flag = 1;
            if (g_short_ECG_flag)
            {
                // Start create/write new .csv
                SD_CreateNewCSV();
                g_sys_state = ECG_SHORT;
            }
            if (g_long_ECG_flag)
            {
                // Start create/write new .csv
                SD_CreateNewCSV();
                g_sys_state = ECG_LONG;
            }
            break;

        case SYS_ERROR:
            break;

        case SYS_DIRTY_START:
            break;

        case SYS_BAD_KEY:
            break;

        default:
            break;
        }     // while(1)
    }
}

/*** Configure Watchdog Timer ***/
void Init_Watchdog(void)
{
    WDT_A_hold(WDT_A_BASE);
}

/*** Global Interrupt Enable ***/
void EnableGlobalInterrupt()
{
    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    // Enter interrupt
    __bis_SR_register(GIE);
    // For debugger
    __no_operation();
}
