#include "sd_card_functions.h"

#define SD_BUFFER_MAX_SIZE  512
#define SD_CSV_ARR_LENGTH   23

unsigned char MST_Data, SLV_Data;
BYTE buffer[32];
char filename[] = "/testfile.txt";
char csvnameArr[SD_CSV_ARR_LENGTH] = "/ECGAA.CSV";
char txbufferInit[] = "SD Card init successful";
char g_txbuffer[SD_BUFFER_MAX_SIZE] = "Test text for txt and csv files";
char firstLetter, secondLetter;
int result = 1;
unsigned int size;
unsigned int bytesWritten;

char commaArr[1];
char adcSingleResultArr[4];
char localtimeArr[80];

void SD_TestWriteOnSD(void);

void Init_FAT(void){
    errCode = -1;
    
    while (errCode != FR_OK) {                              //go until f_open returns FR_OK (function successful)
        errCode = f_mount(0, &fatfs);                       //mount drive number 0
        errCode = f_opendir(&dir, "/");				    	//root directory

        errCode = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
        if (errCode != FR_OK)
            result = 0;                                     //used as a debugging flag
    }
    f_write(&file, txbufferInit, sizeof(txbufferInit), &bytesWritten);
    f_close(&file);

    
    // f_open(&file, "/ecgdata.csv", FA_CREATE_ALWAYS | FA_WRITE);
//    f_close(&file);


//    f_mkdir("/test");
//    f_open(&file, "/test/newfile.txt", FA_CREATE_ALWAYS | FA_WRITE);
//    f_write(&file, g_txbuffer, sizeof(g_txbuffer), &bytesWritten);
//    f_close(&file);
//
    // f_open(&file, "/ecgdata.csv", FA_CREATE_ALWAYS | FA_WRITE);
//    f_write(&file, g_txbuffer, sizeof(g_txbuffer), &bytesWritten);
//    f_close(&file);
}

char *strnfromchar(char destination[], char source, size_t num) {
    destination[0] = source;    // copy the character into the string
    destination[1] = '\0';      // null-terminate the string

    return destination;         // common convention for str functions
}

void SD_CreateNewCSV(void) {
    // TODO add date to file name
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);

//    strftime(csvnameArr, SD_CSV_ARR_LENGTH, "/ecgdata%m%d%H%M%S.csv", timeinfo);
//    strftime(csvnameArr, SD_CSV_ARR_LENGTH, "/ecgdata.csv", timeinfo);
//    sprintf(csvnameArr, "%s", "/ECGDATAA.CSV");
    f_open(&file, csvnameArr, FA_CREATE_ALWAYS | FA_WRITE);
//    f_close(&file);

    // Next letter in file name
    firstLetter = csvnameArr[4];
    secondLetter = csvnameArr[5];
    if (isalpha(secondLetter) && tolower(secondLetter) != 'z') {
        csvnameArr[5] = ++secondLetter;
    } else if (isalpha(firstLetter) && tolower(firstLetter) != 'z') {
        csvnameArr[4] = ++firstLetter;
        csvnameArr[5] = 'A';
    } else {
        csvnameArr[4] = 'A';
        csvnameArr[5] = 'A';
    }



//    else {
//        firstLetter = secondLetter;
//        if (isalpha(firstLetter) && tolower(firstLetter) != 'z') {
//            csvnameArr[5] = ++secondLetter;
//            csvnameArr[4] = ++firstLet;
//        }
//    }
}

void SD_StartWriting(void) {
    // TODO write time stuff in separate function
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    // Set adc value
    sprintf(adcSingleResultArr, "%d", g_adc_result);
    g_tmp_return = f_puts(adcSingleResultArr, &file);

    // Set comma
    sprintf(commaArr, "%s", ",");
    g_tmp_return = f_puts(commaArr, &file);

    // TODO fix bug with transferring time to array
    strftime(localtimeArr, 80, "The time is %I:%M %p.\n", timeinfo);
    g_tmp_return = f_puts(localtimeArr, &file);
}

void SD_StopWriting(void) {
    f_close(&file);
}

// void SD_TestWriteOnSD (void) {
//     g_writingCyclesCnt++;
//     // TODO set local time config to separate function
//     // Local time config
//     time_t rawtime;
//     struct tm * timeinfo;
//     time (&rawtime);
//     timeinfo = localtime (&rawtime);
//     // printf ("%s", asctime (timeinfo));

//     // Open file for writing
// //        f_open(&file, "/ecgdata.csv", FA_OPEN_EXISTING | FA_WRITE);

//     // Set adc value
//     sprintf(adcSingleResultArr, "%d", g_adc_result);
//     g_tmp_return = f_puts(adcSingleResultArr, &file);

//     // Set comma
//     sprintf(commaArr, "%s", ",");
//     g_tmp_return = f_puts(commaArr, &file);

//     // Print local time
//     //sprintf(localtimeArr, "%s", asctime (timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec));
// //                strftime(localtimeArr, 30, "%H%M%S", timeinfo);
//     // TODO fix bug with transferring time to array
//     strftime(localtimeArr, 80, "The time is %I:%M %p.\n", timeinfo);
//     g_tmp_return = f_puts(localtimeArr, &file);

//     // Next line
// //    sprintf(adcSingleResultArr, "%s", "\r\n");
// //    g_tmp_return = f_puts(adcSingleResultArr, &file);

//     // TODO i.e. Display control with UART cmd
//     if (g_writingCyclesCnt == 100) {
//         f_close(&file);
//     }
// }

//void SD_WriteCSV(void) {
//    uint16_t sd_cnt = 0;
//    for (sd_cnt = 0; sd_cnt < SD_BUFFER_MAX_SIZE; sd_cnt++) {
//        g_txbuffer[sd_cnt] = '\0';
//    }
//    strcpy(g_txbuffer, "New data for csv file\n");
//    f_open(&file, "/ecgdata.csv", FA_CREATE_ALWAYS | FA_WRITE);
//    f_write(&file, g_txbuffer, sizeof(g_txbuffer), &bytesWritten);
//    f_close(&file);
//}

    // TODO line below to be tested
    //f_mount(0,0);       // unmount sd card if needed
