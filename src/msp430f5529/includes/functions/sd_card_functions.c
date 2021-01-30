#include "sd_card_functions.h"

#define SD_BUFFER_MAX_SIZE  512

unsigned char MST_Data, SLV_Data;
BYTE buffer[32];
char filename[] = "/testfile.txt";
char txbufferInit[] = "SD Card init successful";
char g_txbuffer[SD_BUFFER_MAX_SIZE] = "Test text for txt and csv files";
int result = 1;
unsigned int size;
unsigned int bytesWritten;

void SD_WriteCSV(void);

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
    
    f_open(&file, "/ecgdata.csv", FA_CREATE_ALWAYS | FA_WRITE);
//    f_close(&file);


//    f_mkdir("/test");
//    f_open(&file, "/test/newfile.txt", FA_CREATE_ALWAYS | FA_WRITE);
//    f_write(&file, g_txbuffer, sizeof(g_txbuffer), &bytesWritten);
//    f_close(&file);
//
//    f_open(&file, "/ecgdata.csv", FA_CREATE_ALWAYS | FA_WRITE);
//    f_write(&file, g_txbuffer, sizeof(g_txbuffer), &bytesWritten);
//    f_close(&file);
}

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
