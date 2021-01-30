#include "main.h"

/* GLOBAL VARs */
uint8_t g_timer_1khz_flag = 0;
uint8_t g_timer_1sec_flag = 0;
uint16_t g_adc_result = 0;
uint8_t g_short_ECG_flag = 0;
uint8_t g_long_ECG_flag = 0;
uint8_t g_tmp_return = 0;

uint8_t tmpCnt = 0;

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
            EnableGlobalInterrupt();
            /* Init MSP430 END */
            //g_sys_state = IDLE_STATE; // Change state
            g_sys_state = ECG_SHORT; // Change state
            break;

        case ECG_SHORT:
            if (g_timer_1khz_flag)
            {
                g_timer_1khz_flag = 0;
                ST_ECG();
                
                /* BEGIN Writing on SD Card */
//                f_open(&file, "/ecgdata.csv", FA_OPEN_EXISTING | FA_WRITE);
//                g_tmp_return = f_putc('a', &file);
//                g_tmp_return = f_putc('b', &file);
//                g_tmp_return = f_putc('c', &file);
                char tmpArray[4];
                sprintf(tmpArray, "%d", g_adc_result);
                g_tmp_return = f_puts(tmpArray, &file);
                tmpCnt++;
                if (tmpCnt == 5) {
                    f_close(&file);
                }
//                g_tmp_return = f_printf(&file, '5678xyz');
//                g_tmp_return = f_puts('c', &file);
//                g_tmp_return = f_puts('d', &file);
//                g_tmp_return = f_printf(&file, 'e');
                //f_write(&file, g_txbuffer, sizeof(g_txbuffer), &bytesWritten);
                /* END Writing on SD Card */


            }
            if (g_long_ECG_flag)
            {
                g_long_ECG_flag = 0;
                Uart_ECG_Error();
            }
            if (!g_short_ECG_flag)
            {
                Clear_Wave_LT(); // Clear läuft auch am Display bei "Stop"
                g_sys_state = IDLE_STATE;
            }
            break;

        case ECG_LONG:
            if (g_timer_1khz_flag)
            {
                g_timer_1khz_flag = 0;
                LT_ECG();
            }
            if (g_short_ECG_flag)
            {
                g_short_ECG_flag = 0;
                Uart_ECG_Error();
            }
            if (!g_long_ECG_flag)
            {
                Clear_Wave_LT(); // Clear läuft auch am Display bei "Stop"
                g_sys_state = IDLE_STATE;
            }
            break;

        case ENERGY_SAVING_MODE:
            break;

        case IDLE_STATE:
            g_sys_state = ECG_SHORT;
            if (g_short_ECG_flag)
            {
                g_sys_state = ECG_SHORT;
            }
            if (g_long_ECG_flag)
            {
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
