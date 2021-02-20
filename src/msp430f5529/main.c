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
bool g_bt_connected = FALSE;
// bool g_display_sleep_flag = TRUE;
bool g_adc_new_values = 0;
/* BUZZER VARs*/
uint8_t g_timer_250Hz_Buzzer = 0;
uint8_t g_buzzer_1sec_flag = 0;
uint8_t g_buzzer_on_flag = 0;
uint16_t g_buzzer_cnt = 0;

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
            Init_FAT();               
            Init_UART_BT();
            Init_Median_Filter();
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
            // Check Akku > 80%
            Check_Akku_Percentage();
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
                /* Steps for evergy saving in ECG_LONG
                - MCU is recording long ecg
                - 5V deactivated
                - receive ADC values as usual
                - store ADC values for 1 sec on MCU
                - activate 5V for transfer
                - send ADC values to SD Card and save in existing file
                - repeat */

                //Compute new Values and publish to Display
                LT_ECG();

                // Deactivate 5V
                
                // Get ADC values as usual
                // -> will be executed automatically

                // Store ADC values on MCU
                SD_Save_ADC_Values();

                // Activate 5V

                // Send ADC values to SD Card & save to the 
                // same file

                // Write in csv
                //SD_StartWriting();

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
                // Send Akku Value to Display
                ADC_Akku_Average_Value();
                // Check if Bluetooth is connetced and show it on Display
                Check_BT_Connection();
                // Check if SD Card is connetced and show it on Display
                Check_SD_Card_Connection();
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
