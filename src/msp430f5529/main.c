#include "main.h"

/* GLOBAL VARs */
uint8_t g_timer_250Hz_flag = 0;
uint8_t g_timer_1sec_flag = 0;
uint8_t g_timer_uart_1sec = 0;
uint16_t g_adc_result = 0;
uint8_t g_short_ECG_flag = 0;
uint8_t g_long_ECG_flag = 0;
uint16_t g_akku_vol = 0;
uint8_t g_tmp_return = 0;
uint16_t g_writingCyclesCnt = 0;
uint8_t g_5v_flag = 0;
uint8_t g_cnt_sec = 0;
uint8_t g_cnt_min = 0;
uint8_t g_cnt_hour = 0;
bool g_sd_card_inserted = FALSE;

bool g_adc_new_values = 0;

/* BUZZER VARs*/
uint8_t g_timer_250Hz_Buzzer = 0;
uint8_t g_buzzer_1sec_flag = 0;
uint8_t g_buzzer_on_flag = 0;
uint16_t g_buzzer_cnt = 0;

/* For median filter */
#define NUM_ELEMENTS    11
sMedianFilter_t medianFilter;
static sMedianNode_t medianBuffer[NUM_ELEMENTS];
/* End median filter */

STATE_MACHINE_e g_sys_state = SYS_INIT;
/* END GLOBAL VARs */

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);

/* Function definitions */
void main(void)
{
    while (1)
    {
        switch (g_sys_state)
        {

        case SYS_INIT:
            /* Init MSP430 BEGIN */
            Init_Watchdog();
            Init_GPIO();
            Init_CLK();
            Init_Timers();
            Init_UART();
            Init_ADC();
            Init_SPI();

            /* Test Card Detect Pin */
            if (GPIO_getInputPinValue(GPIO_PORT_P2, 
            GPIO_PIN0)) {
                g_sd_card_inserted = TRUE;      // inserted = TRUE ; not inserted = FALSE
            }

            Init_FAT();
            //Init UART Interface for Bluetooth                 
            Init_UART_BT();             
            /* Init median filter */
            medianFilter.numNodes = NUM_ELEMENTS;
            medianFilter.medianBuffer = medianBuffer;
            MEDIANFILTER_Init(&medianFilter); // Init median filter
            EnableGlobalInterrupt();
            /* Init MSP430 END */

            g_sys_state = IDLE_STATE;   // Change state
            break;

        case IDLE_STATE:

            //Check if Start of short ECG is requested
            if (g_short_ECG_flag && g_timer_uart_1sec)  // Sync timer by changing state to ECG Short
            {
                g_timer_uart_1sec = 0;                  // Sync timer back to 0 
                // Start create/write new .csv
                SD_CreateNewCSV();
                g_sys_state = ECG_SHORT;
            }

            //Check if Start of short ECG is requested
            if (g_long_ECG_flag && g_timer_uart_1sec)   // Sync timer by changing state to ECG Long
            {
                g_timer_uart_1sec = 0;                  // Sync timer back to 0 
                // Start create/write new .csv
                SD_CreateNewCSV();
                g_sys_state = ECG_LONG;
            }
            bt_state = GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1);

            break;

        case ECG_SHORT:

            //Update Time for ECG
            ECG_Timer_ST();

            //Start ADC at given frequency
            if (g_timer_250Hz_flag)
            {
                //Give ADC Start Command
                Start_ADC();

                //Reset Timer Flag
                g_timer_250Hz_flag = 0;
            }

            //When ADC finished Conversion
            if (g_adc_new_values)
            {
                //Compute new Values and publish to Display
                ST_ECG();

                // Write in csv
                SD_StartWriting();

                //Reset Flag
                g_adc_new_values = false;
            }

            //Check if switch to long time ECG requested
            if (g_long_ECG_flag)
            {
                g_long_ECG_flag = 0;
                Short_ECG_Error();
            }

            //Check if stop of Short ECG requested
            if (!g_short_ECG_flag)
            {
                Clear_Wave_ST();
                Clear_ECG_Timer_ST();
                // Stop writing in .csv
                SD_StopWriting();
                g_sys_state = IDLE_STATE;
            }

            break;

        case ECG_LONG:

            //Update Time for ECG
            ECG_Timer_LT();

            //Start ADC at given frequency
            if (g_timer_250Hz_flag)
            {
                //Give ADC Start Command
                Start_ADC();

                //Reset Timer Flag
                g_timer_250Hz_flag = 0;
            }

            //When ADC finished Conversion
            if (g_adc_new_values)
            {
                //Compute new Values and publish to Display
                LT_ECG();

                // Write in csv
                SD_StartWriting();

                //Reset Flag
                g_adc_new_values = false;
            }

            //Check if switch to short ECG requested
            if (g_short_ECG_flag)
            {
                g_short_ECG_flag = 0;
                Long_ECG_Error();
            }

            //Check if stop of long term ECG requested
            if (!g_long_ECG_flag)
            {
                Clear_Wave_LT();
                Clear_ECG_Timer_LT();
                // Stop writing in .csv
                SD_StopWriting();
                g_sys_state = IDLE_STATE;
            }

            break;

        case ENERGY_SAVING_MODE:
            State_sys_Energy_Saving_Mode();
            break;

        case SYS_WAKEUP:
            State_sys_Wakeup_Mode();
            break;

        case SYS_BAD_KEY:
            break;

        case SYS_ERROR:
            break;

        default:
            break;
        }

        //Check AKKU in chosen Cases
        if (g_timer_1sec_flag)
        {
            g_timer_1sec_flag = 0;
            if (g_sys_state == IDLE_STATE ||
                g_sys_state == ECG_SHORT ||
                g_sys_state == ECG_LONG)
            {
                ADC_Akku_Average_Value();
            }
        }
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
