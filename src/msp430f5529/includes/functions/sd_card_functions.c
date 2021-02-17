#include "sd_card_functions.h"

#define SD_BUFFER_MAX_SIZE  512
#define SD_CSV_ARR_LENGTH   23
#define TB_SIZE             10
#define SD_INIT_TIMEOUT     1000

unsigned char MST_Data, SLV_Data;
BYTE buffer[32];
char filename[] = "/testfile.txt";
char csvNameShortECGArr[SD_CSV_ARR_LENGTH] = "/SHORTAA.CSV";
char csvNameLongECGArr[SD_CSV_ARR_LENGTH] = "/LONGAA.CSV";
const char nextLine[] = ".\n";
char txbufferInit[] = "SD Card init successful";
char g_txbuffer[SD_BUFFER_MAX_SIZE] = "Test text for txt and csv files";
char firstLetter, secondLetter;
int result = 1;
uint16_t sdCardTimeout = 0;
unsigned int size;
unsigned int bytesWritten;

char timeBuffer[TB_SIZE];

char commaArr[1];
char adcSingleResultArr[4];
char localtimeArr[80];

void SD_TestWriteOnSD(void);

void Init_FAT(void){
    errCode = 20;
    
    // If Card Detect = TRUE -> Try to init
    if (GPIO_getInputPinValue(GPIO_PORT_P2, 
            GPIO_PIN0)) {
        g_sd_card_inserted = TRUE;
        //go until f_open returns FR_OK (function successful)
        while ((errCode != FR_OK) || (sdCardTimeout <= SD_INIT_TIMEOUT)) {  
            sdCardTimeout++;
            //mount drive number 0                            
            errCode = f_mount(0, &fatfs); 
            //root directory                      
            errCode = f_opendir(&dir, "/");				    	

            errCode = f_open(&file, filename, 
                FA_CREATE_ALWAYS | FA_WRITE);
            if (errCode != FR_OK) {
                //used as a debugging flag
                result = 0;
            }
            if (sdCardTimeout == SD_INIT_TIMEOUT) {
                // TODO send message to display
                SD_Card_Error();
            }
        }
        sdCardTimeout = 0;
        f_write(&file, txbufferInit, 
            sizeof(txbufferInit), &bytesWritten);
        f_close(&file);
    } else {
        g_sd_card_inserted = FALSE;
        SD_Card_Error();
    }
}

void SD_CreateNewCSV(void) {

    if(g_sd_card_inserted) {
        // File naming for SHORT ECG
        if (g_short_ECG_flag == 1) {

            f_open(&file, csvNameShortECGArr, 
                FA_CREATE_ALWAYS | FA_WRITE);

            // Next letter in file name
            firstLetter = csvNameShortECGArr[6];
            secondLetter = csvNameShortECGArr[7];
            if (isalpha(secondLetter) && 
                tolower(secondLetter) != 'z') {
                csvNameShortECGArr[7] = ++secondLetter;
            } else if (isalpha(firstLetter) && 
                tolower(firstLetter) != 'z') {
                csvNameShortECGArr[6] = ++firstLetter;
                csvNameShortECGArr[7] = 'A';
            } else {
                csvNameShortECGArr[6] = 'A';
                csvNameShortECGArr[7] = 'A';
            }
        // File naming for LONG EKG
        } else {
            f_open(&file, csvNameLongECGArr, 
                FA_CREATE_ALWAYS | FA_WRITE);

            // Next letter in file name
            firstLetter = csvNameLongECGArr[5];
            secondLetter = csvNameLongECGArr[6];
            if (isalpha(secondLetter) && 
                tolower(secondLetter) != 'z') {
                csvNameLongECGArr[6] = ++secondLetter;
            } else if (isalpha(firstLetter) && 
                tolower(firstLetter) != 'z') {
                csvNameLongECGArr[5] = ++firstLetter;
                csvNameLongECGArr[6] = 'A';
            } else {
                csvNameLongECGArr[5] = 'A';
                csvNameLongECGArr[6] = 'A';
            }
        }
    }
}

void SD_SetTimeStamp(void){
    memset(timeBuffer,0x00,TB_SIZE);
    if (g_sys_state == ECG_SHORT){
        sprintf(timeBuffer, "%02d:%02d \n",g_cnt_min, g_cnt_sec);
    }else if (g_sys_state == ECG_LONG){
        sprintf(timeBuffer, "%02d:%02d:%02d \n",g_cnt_hour, g_cnt_min, g_cnt_sec);
    }
}

void SD_StartWriting(void) {
    if(g_sd_card_inserted) {
        // Set adc value
        sprintf(adcSingleResultArr, "%d", g_adc_result);
        g_tmp_return = f_puts(adcSingleResultArr, &file);

        // Set comma
        sprintf(commaArr, "%s", ",");
        g_tmp_return = f_puts(commaArr, &file);

        // Set timestamp
        SD_SetTimeStamp();
        g_tmp_return = f_puts(timeBuffer, &file);
    }
}

void SD_StopWriting(void) {
    if (g_sd_card_inserted) {
        f_close(&file);
    }
}

// TODO line below to be tested
//f_mount(0,0);       // unmount sd card if needed
