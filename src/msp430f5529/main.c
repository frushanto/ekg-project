#include "main.h"

/* GLOBAL VARs */
uint8_t g_timer_250Hz_flag = 0;
uint8_t g_timer_1sec_flag = 0;
uint8_t g_timer_uart_1sec = 0;
uint16_t g_adc_result = 0;
uint16_t g_adc_result_storage[LONG_ECG_STORAGE_SIZE] = {0};     // 4 sec storage (f = 250 Hz)
uint16_t g_adc_result_storage_cpy[LONG_ECG_STORAGE_SIZE] = {0}; // Copy of storage array
uint16_t g_adc_result_cnt = 0;
bool g_adc_result_storage_full = FALSE;
uint8_t g_short_ECG_flag = 0;
uint8_t g_long_ECG_flag = 0;
uint16_t g_akku_vol = 0;
uint8_t g_tmp_return = 0;
uint16_t g_writingCyclesCnt = 0;
uint8_t g_5v_flag = 0;
uint8_t g_cnt_sec = 0;
uint8_t g_cnt_min = 0;
uint8_t g_cnt_hour = 0;
uint16_t g_cnt_msec_long = 0;
uint8_t g_cnt_sec_long = 0;
uint8_t g_cnt_min_long = 0;
uint8_t g_cnt_hour_long = 0;
bool g_ecg_long_5v_on = TRUE;
bool g_ecg_long_btn_pressed = FALSE;
bool g_sd_card_inserted = FALSE;
bool g_bt_connected = FALSE;
bool g_adc_new_values = 0;
uint8_t g_user_select = 0;
uint16_t bt_test = 0;
uint8_t g_timer_250Hz_Buzzer = 0;
uint8_t g_buzzer_1sec_flag = 0;
uint8_t g_buzzer_on_flag = 0;
uint16_t g_buzzer_cnt = 0;
bool g_buzzer_20_percent = FALSE;
uint16_t g_buzzer_1kz_cnt = 1;

STATE_MACHINE_e g_sys_state = SYS_INIT; // Enter sys Init at beginning
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

//Init_FAT will brick the CPU when card does not respond
#ifndef LAUNCHPAD
            Init_FAT();
#endif

            Init_UART_BT();
            Init_Median_Filter();
            EnableGlobalInterrupt();
            /* Init MSP430 END */

            g_sys_state = IDLE_STATE; // Change state
            break;

        case IDLE_STATE:

            //Check if Start of short ECG is requested
            if (g_short_ECG_flag && g_timer_uart_1sec) // Sync timer by changing state to ECG Short
            {
                g_timer_uart_1sec = 0; // Sync timer back to 0
                // Start create/write new .csv
                SD_CreateNewCSV();
                SD_WriteUserInCSV();
                g_sys_state = ECG_SHORT;
            }

            //Check if Start of long ECG is requested
            if (g_long_ECG_flag && g_timer_uart_1sec // Sync timer by changing state to ECG Long
                && g_sd_card_inserted)
            {
                // Check Akku > 80%
                Check_Akku_Percentage();
                g_timer_uart_1sec = 0; // Sync timer back to 0
                // Start create/write new .csv
                SD_CreateNewCSV();
                SD_WriteUserInCSV();
                SD_StopWriting();
                g_sys_state = ECG_LONG;
            }
            else if (!g_sd_card_inserted) // Don't set Long_ECG Flag if SD not inserted
            {
                g_long_ECG_flag = 0;
            }

//Testcode for work with Launchpad
#ifdef LAUNCHPAD

            //Sende Wert einmal pro Sekunde und toggel LED
            if (get_1hz_flag_bt())
            {
                //Toggel LED2 auf Launchpad
                GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN7);

                //Startet Senden mit DMA
                send_bt_value_dma(bt_test++);

                //Reset Counter
                if (bt_test > 4096)
                    bt_test = 0;
            }

#endif

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
            LT_ECG();
            SD_Energy_Saving_Long_ECG();

            //Check if switch to short ECG requested
            if (g_short_ECG_flag)
            {
                g_short_ECG_flag = 0;
                Long_ECG_Error();
            }

            //Check if stop of long term ECG requested
            if (!g_long_ECG_flag)
            {
                memset(g_adc_result_storage_cpy, 0, 2000);
                Clear_Wave_LT();
                Clear_ECG_Timer_LT();
                // Stop writing in .csv
                SD_StopWriting();
                g_adc_result_cnt = 0;
                g_sys_state = IDLE_STATE;
            }

            break;

        case ENERGY_SAVING_MODE:
            State_sys_Energy_Saving_Mode();
            break;

        case SYS_WAKEUP:
            State_sys_Wakeup_Mode();
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

/*** Delay Function, Input in ms ***/
void delay(uint16_t delay_time)
{
    //Buffer actual time
    uint32_t start_time = get_ms();

    //Until time is passed do nothing
    while (get_ms() < start_time + delay_time)
    {
        __no_operation();
    }
}
