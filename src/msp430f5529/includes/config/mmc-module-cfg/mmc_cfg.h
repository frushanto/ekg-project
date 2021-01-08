#ifndef MMC_CFG_H
#define MMC_CFG_H

#include <msp430f5529.h>
#include "mmc_spi_cfg.h"
#include "mmc_comm_spi_cfg.h"

#define SD_BLOCKSIZE 512
#define SD_BLOCKSIZE_NBITS 9
#define PERIPH_CLOCKRATE 8000000

int init_SD(sd_context_t *sdc);
int read_block(sd_context_t *sdc, u32 blockaddr, unsigned char *data);
int write_block(sd_context_t *sdc, u32 blockaddr, unsigned char *data);

void sd_delay(char number);
void trigger(void);
void sd_send_command(sd_context_t *sdc, unsigned char cmd, unsigned char *argument);
void change_argument(unsigned char *argument, u32 value);
void sd_wait_notbusy (sd_context_t *sdc);
void shift_align(unsigned char *ar);

#endif /* MMC_CFG_H_ */
