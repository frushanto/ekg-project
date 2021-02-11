#include "sd_card_functions.h"

#define SD_BUFFER_MAX_SIZE  512
#define SD_CSV_ARR_LENGTH   23
#define TB_SIZE             80

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
unsigned int size;
unsigned int bytesWritten;

char timeBuffer[TB_SIZE];

char commaArr[1];
char adcSingleResultArr[4];
char localtimeArr[80];

void SD_TestWriteOnSD(void);

void Init_FAT(void){
    errCode = 20;
    
    while (errCode != FR_OK) {                              //go until f_open returns FR_OK (function successful)
        errCode = f_mount(0, &fatfs);                       //mount drive number 0
        errCode = f_opendir(&dir, "/");				    	//root directory

        errCode = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
        if (errCode != FR_OK)
            result = 0;                                     //used as a debugging flag
    }
    f_write(&file, txbufferInit, sizeof(txbufferInit), &bytesWritten);
    f_close(&file);
}

void SD_CreateNewCSV(void) {

    // File naming for SHORT ECG
    if (g_short_ECG_flag == 1) {

        f_open(&file, csvNameShortECGArr, FA_CREATE_ALWAYS | FA_WRITE);

        // Next letter in file name
        firstLetter = csvNameShortECGArr[6];
        secondLetter = csvNameShortECGArr[7];
        if (isalpha(secondLetter) && tolower(secondLetter) != 'z') {
            csvNameShortECGArr[7] = ++secondLetter;
        } else if (isalpha(firstLetter) && tolower(firstLetter) != 'z') {
            csvNameShortECGArr[6] = ++firstLetter;
            csvNameShortECGArr[7] = 'A';
        } else {
            csvNameShortECGArr[6] = 'A';
            csvNameShortECGArr[7] = 'A';
        }
    // File naming for LONG EKG
    } else {
        f_open(&file, csvNameLongECGArr, FA_CREATE_ALWAYS | FA_WRITE);

        // Next letter in file name
        firstLetter = csvNameLongECGArr[5];
        secondLetter = csvNameLongECGArr[6];
        if (isalpha(secondLetter) && tolower(secondLetter) != 'z') {
            csvNameLongECGArr[6] = ++secondLetter;
        } else if (isalpha(firstLetter) && tolower(firstLetter) != 'z') {
            csvNameLongECGArr[5] = ++firstLetter;
            csvNameLongECGArr[6] = 'A';
        } else {
            csvNameLongECGArr[5] = 'A';
            csvNameLongECGArr[6] = 'A';
        }
    }
}

void SD_GetCurrentTime(void) {
    time_t curtime;
    struct tm *loctime; /* Get the current time. */
    curtime = time (NULL); /* Convert it to local time representation. */
    loctime = localtime (&curtime); /* Print out the date and time in the standard format. */

    /* Date format - dd-MM-YYYY HH:MM:SS */
    uint16_t year = loctime->tm_year + 1900; // Year adjustment
    uint8_t hour = loctime->tm_hour + 7; // from 7 to 31 -> time zone adjustment
    uint8_t month = loctime->tm_mon + 1; // Month adjustment (1-12)

    if (hour >= 24) {
        hour = hour - 24;
    }

    sprintf(timeBuffer, "%02d-%02d-%d %02d:%02d:%02d\n",
            loctime->tm_mday, month, year,
            hour, loctime->tm_min, loctime->tm_sec);
}

void SD_StartWriting(void) {
    // Set adc value
    sprintf(adcSingleResultArr, "%d", g_adc_result);
    g_tmp_return = f_puts(adcSingleResultArr, &file);

    // Set comma
    sprintf(commaArr, "%s", ",");
    g_tmp_return = f_puts(commaArr, &file);

    // Set current time
    SD_GetCurrentTime();
    g_tmp_return = f_puts(timeBuffer, &file);
}

void SD_StopWriting(void) {
    f_close(&file);
}

// TODO line below to be tested
//f_mount(0,0);       // unmount sd card if needed
