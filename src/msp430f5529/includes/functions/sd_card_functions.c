#include "sd_card_functions.h"

unsigned char MST_Data, SLV_Data;
BYTE buffer[32];
int result = 1;
//unsigned char buffer[20];
unsigned int size;

void Init_FAT(void){
    errCode = -1;
    
    while (errCode != FR_OK) {                              //go until f_open returns FR_OK (function successful)
        errCode = f_mount(0, &fatfs);                       //mount drive number 0
        errCode = f_opendir(&dir, "/");				    	//root directory

        errCode = f_open(&file, "/testfile.txt", FA_CREATE_ALWAYS | FA_WRITE);
        if (errCode != FR_OK)
            result = 0;                                     //used as a debugging flag
    }
    unsigned int bytesWritten;
    f_write(&file, "SD Card init successful", 23, &bytesWritten);
    res = f_read(&file, buffer, sizeof(buffer), &size);
    f_close(&file);
    // TODO line below to be tested
    //f_mount(0,0);       // unmount sd card if needed
}
