#include "main.h"

/* GLOBAL VARs */
uint8_t g_timer_1khz_flag = 0;
uint8_t g_timer_1sec_flag = 0;
uint16_t g_adc_result = 0;
uint8_t g_short_ECG_flag = 0;
uint8_t g_long_ECG_flag = 0;

STATE_MACHINE_e g_sys_state = SYS_INIT;
/* END GLOBAL VARs */

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);
// SD Card Module
FRESULT WriteFile(char*, char*, WORD);
void fat_init(void);
FIL file;                   /* Opened file object */
FATFS fatfs;                /* File system object */
DIRS dir;                   /* Directory object   */
FRESULT errCode;            /* Error code object  */
FRESULT res;                /* Result object      */
UINT bytesRead;             /* Bytes read object  */
UINT read;                  /* Read bytes object  */

unsigned char MST_Data,SLV_Data;
BYTE buffer[32];
int result=1;

/* Function definitions */
void main(void)
{
    unsigned char buffer[20];
    unsigned int size;

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

            
            /*** BEGIN SD Card Code ***/
            fat_init();            //mount, set directory to read from, assign file
            //unsigned int bytesWritten;
            //f_write(&file, "FAYSAL", 6, &bytesWritten);
            res = f_read(&file, buffer, sizeof(buffer), &size);
            f_close(&file);
            // TODO line below to be tested
            //f_mount(0,0);       // unmount sd card if needed
            /*** END SD Card Code ***/


            EnableGlobalInterrupt();
            /* Init MSP430 END */
            g_sys_state = IDLE_STATE; // Change state
            break;

        case ECG_SHORT:
            if (g_timer_1khz_flag)
            {
                g_timer_1khz_flag = 0;
                ST_ECG();
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

void fat_init(void){
    errCode = -1;

    while (errCode != FR_OK){                               //go until f_open returns FR_OK (function successful)
        errCode = f_mount(0, &fatfs);                       //mount drive number 0
        errCode = f_opendir(&dir, "/");				    	//root directory

        errCode = f_open(&file, "/teste.txt", FA_CREATE_ALWAYS | FA_WRITE);
        if(errCode != FR_OK)
            result=0;                                       //used as a debugging flag
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
