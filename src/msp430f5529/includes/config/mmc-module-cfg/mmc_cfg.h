#ifndef MMC_CFG_H
#define MMC_CFG_H

#include <msp430f5529.h>
#include "mmc_spi_cfg.h"
#include "mmc_comm_spi_cfg.h"
#include <std_types.h>
#include <msp430.h>
#include <driverlib.h>

#define SD_BLOCKSIZE                512
#define SD_BLOCKSIZE_NBITS          9
#define PERIPH_CLOCKRATE            1000000     // 1MHz

void MMC_Init(void);
int MMC_Init_Card(mmc_context_t *sdc);
int MMC_Read_Block(mmc_context_t *sdc, u32 blockaddr, unsigned char *data);
int MMC_Write_Block(mmc_context_t *sdc, u32 blockaddr, unsigned char *data);

void MMC_Delay(char number);
//void trigger(void);
void MMC_Send_Command(mmc_context_t *sdc, unsigned char cmd, unsigned char *argument);
void MMC_Change_Argument(unsigned char *argument, u32 value);
void MMC_Wait_Notbusy (mmc_context_t *sdc);
void MMC_Shift_Align(unsigned char *ar);

#endif /* MMC_CFG_H_ */
