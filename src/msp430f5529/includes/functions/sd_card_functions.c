#include "sd_card_functions.h"

unsigned char MST_Data, SLV_Data;
BYTE buffer[32];
unsigned char filename[] = "/testfile.txt";
unsigned char txbufferInit[] = "SD Card init successful";
int result = 1;
unsigned int size;
unsigned int bytesWritten;

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
    tempFunc_WriteToFile("Blah blah");
}

void tempFunc_WriteToFile(const void *txbuff) {
    const BYTE *txbuffer = txbuff;
    // Create new txt file and write data
    f_mkdir("/test");
    f_open(&file, "/test/newfile.txt", FA_CREATE_ALWAYS | FA_WRITE);
    //strcpy(txbuffer, "Test data Test data Test data");
    f_write(&file, txbuffer, sizeof(txbuffer), &bytesWritten);

    //res = f_read(&file, buffer, sizeof(buffer), &size);
    f_close(&file);
    // TODO line below to be tested
    //f_mount(0,0);       // unmount sd card if needed
}

