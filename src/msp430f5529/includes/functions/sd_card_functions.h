#ifndef INCLUDES_FUNCTIONS_SD_CARD_FUNCTIONS_H_
#define INCLUDES_FUNCTIONS_SD_CARD_FUNCTIONS_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include <includes/config/uart_cfg.h>
#include <ctype.h>
#include "main.h"
#include "includes/config/sd-card-cfg/ff.h"
#include "time.h"
//#include <stdio.h>
//#include <string.h>

FIL file;                   /* Opened file object */
FATFS fatfs;                /* File system object */
DIRS dir;                   /* Directory object   */
FRESULT errCode;            /* Error code object  */
FRESULT res;                /* Result object      */
UINT bytesRead;             /* Bytes read object  */
UINT read;                  /* Read bytes object  */

/* Function declarations */
FRESULT WriteFile(char*, char*, WORD);
void Init_FAT(void);
void SD_TestWriteOnSD(void);
void SD_CreateNewCSV(void);
void SD_StartWriting(void);
void SD_StopWriting(void);

#endif /* INCLUDES_FUNCTIONS_SD_CARD_FUNCTIONS_H_ */
