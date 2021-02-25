#ifndef INCLUDES_FUNCTIONS_SD_CARD_FUNCTIONS_H_
#define INCLUDES_FUNCTIONS_SD_CARD_FUNCTIONS_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include <includes/config/uart_cfg.h>
#include "main.h"
#include "includes/config/sd-card-cfg/ff.h"
//#include <stdio.h>
//#include <string.h>

FIL file;                   /* Opened file object */
FATFS fatfs;                /* File system object */
DIRS dir;                   /* Directory object   */
FRESULT errCode;            /* Error code object  */
FRESULT res;                /* Result object      */
UINT bytesRead;             /* Bytes read object  */
UINT read;                  /* Read bytes object  */

typedef enum
{
    MODE_NORMAL,
    MODE_5V_OFF,
    MODE_5V_ON
} LONG_ECG_STATE_MACHINE_e;

extern LONG_ECG_STATE_MACHINE_e g_long_ecg_state;
extern bool display_sleep_mode;

/* Function declarations */
FRESULT WriteFile(char*, char*, WORD);
void Init_FAT(void);
void SD_CreateNewCSV(void);
void SD_StartWriting(void);
void SD_StopWriting(void);
void SD_SetTimeStamp(void);
void Check_SD_Card_Connection(void);
void SD_Energy_Saving_Long_ECG(void);

#endif /* INCLUDES_FUNCTIONS_SD_CARD_FUNCTIONS_H_ */
