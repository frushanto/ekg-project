#include "mmc_cfg.h"
#include "mmc_comm_spi_cfg.h"
#include "mmc_spi_cfg.h"

static unsigned char argument[4];

static unsigned char mmc_buffer[SD_BLOCKSIZE];
static mmc_context_t sdc;
uint8_t mmc_ok;
uint8_t mmc_counter, mmc_wait;

void Init_MMC(void) {
	MMC_Init();
}

void MMC_Init(void) {
	mmc_ok = 0;
	mmc_buffer[0] = 0xAA;
	mmc_buffer[1] = 0xFF;
	mmc_buffer[2] = 0x00;

	// Lat
	mmc_buffer[3] = 4;
	mmc_buffer[4] = 4;
	mmc_buffer[5] = 2;
	mmc_buffer[6] = 0;
	mmc_buffer[7] = '.';
	mmc_buffer[8] = 8;
	mmc_buffer[9] = 8;
	mmc_buffer[10] = 2;
	mmc_buffer[11] = 3;
	mmc_buffer[12] = 'N';

	// Lat
	mmc_buffer[13] = 0;
	mmc_buffer[14] = 7;
	mmc_buffer[15] = 1;
	mmc_buffer[16] = 0;
	mmc_buffer[17] = 6;
	mmc_buffer[18] = '.';
	mmc_buffer[19] = 3;
	mmc_buffer[20] = 4;
	mmc_buffer[21] = 3;
	mmc_buffer[22] = 1;
	mmc_buffer[23] = 'W';

	// Configure trigger bits.
	/*
	P8OUT &= ~BIT2;
	P8DIR |= BIT2;
	*/

	// Initialize MMC
	sdc.busyflag = 0;							// Busy Flag
	sdc.timeout_write = PERIPH_CLOCKRATE/8;		// Set Write Clock Rate
	sdc.timeout_read = PERIPH_CLOCKRATE/20;		// Set Read Clock Rate
	mmc_ok = MMC_Init_Card(&sdc);					// Initialize SD
	//spi_set_divisor(4);							// Speed up clock

	/* Read in the first block on the SD Card */
	if (mmc_ok == 1) {
		MMC_Read_Block(&sdc, 0x00, mmc_buffer);
		mmc_buffer[0] = 0x05;
		MMC_Read_Block(&sdc, 0x00, mmc_buffer);
		for (mmc_counter = 0; mmc_counter < 50; mmc_counter++) {
			MMC_Send_Byte(mmc_buffer[mmc_counter]);
			for (mmc_wait = 0; mmc_wait < 200; mmc_wait++);
			mmc_counter++;
		}
		MMC_Read_Block(&sdc, 0x01, mmc_buffer);
		MMC_Read_Block(&sdc, 0x02, mmc_buffer);
		MMC_Read_Block(&sdc, 0x03, mmc_buffer);
		MMC_Read_Block(&sdc, 0x04, mmc_buffer);
		MMC_Read_Block(&sdc, 0x04, mmc_buffer);
	}

}

/* This function initializes the SD card -- forces it into SPI mode */
int MMC_Init_Card(mmc_context_t *sdc) {

	char i;										// Looping Variables
	unsigned char temp[1];						// Received Byte

	sdc->busyflag = 0;							// Busy Flag -> 0

	temp[0] = 0x00;

	MMC_CS_Assert();							// CS -> Low

	MMC_Delay(200);								// Pulse > 74 cycles
	MMC_CS_Deassert();							// CS -> High
	MMC_Delay(80);								// Pulse > 16 cycles

	MMC_CS_Assert();							// CS -> Low

	/* 
	 * To initialize the card in SPI mode:
	 * 
	 * 1. CMD0 arg: 0x0, CRC: 0x95 (response: 0x01) - note that in case of 
	 * 0xFF or garbled response you should simply repeat this step.
	 * 
	 * 2. CMD8 arg: 0x000001AA, CRC: 0x87 (response: 0x01, followed by echo 
	 * of arg, in this case 0x000001AA) - while it may seem that this 
	 * command is optional, it's completely mandatory for newer cards. While 
	 * 0x1AA is a common arg value here, you may actually pass other values 
	 * as well.
	 * 
	 * 3a. CMD55 arg: 0x0, CRC: any, 0x65 actually (response: 0x01; CMD55 
	 * being the prefix to every ACMD; if the response is 0x05, you've got an 
	 * old card - repeat CMD1 with arg 0x0 [CRC 0xF9] instead of CMD55/ACMD41).
	 * 
	 * 3b. ACMD41, arg: 0x40000000, CRC: any, 0x77 actually (note that this 
	 * argument assumes the card is a HCS one, which is usually the case; use 
	 * 0x0 arg [CRC 0xE5] for older cards). If response is 0x0, you're OK; if 
	 * it's 0x01, goto 3a; if it's 0x05, see note on it above (in 3a.).
	 */

	/* CMD 0 sequence -- This forces the SD card into SPI mode.
	 * Needs to be sent very slow */
	MMC_Send_Byte(0xFF);					// Dummy Byte
	MMC_Send_Byte(0x40);					// Send CMD0
	for (i = 0; i < 10; i++);
	MMC_Send_Byte(0x00);
	for (i = 0; i < 10; i++);
	MMC_Send_Byte(0x00);
	for (i = 0; i < 10; i++);
	MMC_Send_Byte(0x00);
	for (i = 0; i < 10; i++);
	MMC_Send_Byte(0x00);
	for (i = 0; i < 10; i++);
	MMC_Send_Byte(0x95);
	for (i = 0; i < 10; i++);
	MMC_Send_Byte(0xFF);					// Dummy Byte
	do {
		temp[0] = MMC_Receive_Byte();		// Receive and store
	} while (temp[0] != 0x01);				// Repeat until Idle Byte, 0x01, is received
	temp[0] = 0xFF;							// Reset temp[0]

	MMC_Send_Byte(0xFF);					// Dummy Byte
	MMC_CS_Deassert();						// CS -> High

	MMC_Delay(80);							// Delay

	/* CMD8 sequence */
	/* CMD8 arg: 0x000001AA, CRC: 0x87 (response: 0x01, 
	 * followed by echo of arg, in this case 0x000001AA)
	 * */
	MMC_CS_Assert();						// CS -> Low
	MMC_Send_Byte(0xFF);					// Dummy Byte
	MMC_Send_Byte(0x01);					// Send CMD8
	MMC_Send_Byte(0xAA);					// Send CMD8
	MMC_Send_Byte(0x87);					// Send CRC
	do {
		temp[0] = MMC_Receive_Byte();		// Receive and store
	} while (temp[0] != 0x01);

	MMC_Send_Byte(0xFF);					// Dummy Byte
	MMC_CS_Deassert();						// CS -> High
	MMC_Delay(80);								// Delay

	// Declare empty argument
	for (i = 0; i < 4; i++) {
		argument[i] = 0;
	}						

	do {
		/* CMD55 sequence */
		MMC_CS_Assert();						// CS -> Low
		MMC_Send_Command(sdc, CMD55, argument);	// Send CMD55
		do {
			temp[0] = MMC_Receive_Byte();		// Receive and store
		} while (temp[0] == 0xFF);				// Try until I get something not 0xFF, want 0x01

		MMC_Send_Byte(0xFF);					// Dummy Byte
		MMC_CS_Deassert();						// CS -> High

		MMC_Delay(10);							// Small Delay

		/* CMD41 sequence */
		MMC_CS_Assert();						// CS -> Low
		MMC_Send_Command(sdc, ACMD41, argument);	// Send ACMD41
		do {
			temp[0] = MMC_Receive_Byte();		// Receive and store
		} while (temp[0] == 0xFF);				// Try until I get something not 0xFF, want 0x00
	} while (temp[0] != 0x00);

	MMC_Send_Byte(0xFF);						// Dummy Byte
	MMC_CS_Deassert();							// CS -> High

	MMC_Delay(80);								// Delay
	for (i = 0; i < 10; i++);

	/* CMD16 sequence */
	MMC_Change_Argument(argument, SD_BLOCKSIZE);	// Change argument to blocksize
	MMC_CS_Assert();							// CS -> Low
	MMC_Send_Command(sdc, CMD16, argument);		// Send CMD16
	do {
		temp[0] = MMC_Receive_Byte();			// Receive and store
	} while (temp[0] != 0x00);					// Try until I get 0x00

	MMC_Send_Byte(0xFF);						// Dummy Byte
	MMC_CS_Deassert();							// CS -> High

	if (temp[0] == 0x00)
		return 1;								// Successfully Initialized! =]
	else
		return 0;								// Did not initialize correctly . . .
}

// Delay code to give the SD Card time to process
void MMC_Delay(char number) {
	char i;
	/* Null for now */
	for (i = 0; i < number; i++) {
		/* Clock out an idle byte (0xFF) */
		MMC_Send_Byte(0xFF);
	}
}

// Debug code to trigger the 'scope externally on rising edge
/*
void trigger(void) {
	char i;
	P8OUT |= BIT2;
	for(i = 0; i < 10; i++);
	P8OUT &= ~BIT2;
}
*/

void MMC_Send_Command(mmc_context_t *sdc, unsigned char cmd, 
unsigned char *argument) {
	int i;
	MMC_Send_Byte(0xFF);					// Dummy Byte
	MMC_Send_Byte((cmd & 0x3F) | 0x40);		// Command Byte -- Converts CMD to HEX and send
	// Argument -- MSB first
	for (i = 3; i >= 0; i--) {
		MMC_Send_Byte(argument[i]);
	}
	MMC_Send_Byte(0xFF);					// Ignore Checksum -- Send Don't Care
	MMC_Send_Byte(0xFF);					// Dummy Byte
}

void MMC_Change_Argument(unsigned char *argument, u32 value) {
	argument[3] = (unsigned char)(value >> 24);
	argument[2] = (unsigned char)(value >> 16);
	argument[1] = (unsigned char)(value >> 8);
	argument[0] = (unsigned char)(value);
}


// Read/Write stuff

int MMC_Read_Block(mmc_context_t *sdc, u32 blockaddr, unsigned char *data) {
	unsigned long int i = 0;
	unsigned char temp[1];
	/* Adjust the block address to a linear address */
	blockaddr <<= SD_BLOCKSIZE_NBITS;
	/* Wait until any old transfers are finished */
	MMC_Wait_Notbusy(sdc); //<-- fix this later
	/* Pack the address */
	MMC_Change_Argument(argument, blockaddr);

	/* Need to add size checking -- CMD 17 */
	MMC_CS_Assert();							// CS -> Low
	MMC_Send_Command(sdc, CMD17, argument);		// Send CMD16
	do {
		temp[0] = MMC_Receive_Byte();				// Receive and store
	} while (temp[0] != 0x00);					// Try until I get 0x00

	MMC_Send_Byte(0xFF);						// Dummy Byte
	MMC_CS_Deassert();							// CS -> High


	/* 
	Check for an error, like a misaligned read
	if (response[0] != 0)
		return 0;
	*/

	/* Re-assert CS to continue the transfer */
	MMC_CS_Assert();

	/* Wait for the token */

	do {
		temp[0] = MMC_Receive_Byte();
	} while (temp[0] != 0xFE);

	for (i = 0; i < SD_BLOCKSIZE; i++) {
		data[i] = MMC_Receive_Byte();
	}

	do {
		temp[0] = MMC_Receive_Byte();				// Receive and store
	} while (temp[0] != 0xFF);					// Try until I get 0x00

	MMC_Send_Byte(0xFF);						// Dummy Byte
	MMC_CS_Deassert();							// CS -> High

	MMC_Shift_Align(data);

	return 1;
}

int MMC_Write_Block(mmc_context_t *sdc, u32 blockaddr, unsigned char *data) {
	unsigned long int i = 0;
	unsigned char temp[1];
	/* Adjust the block address to a linear address */
	blockaddr <<= SD_BLOCKSIZE_NBITS;
	/* Wait until any old transfers are finished */
	MMC_Wait_Notbusy (sdc); //<-- fix this later
	/* Pack the address */
	MMC_Change_Argument(argument, blockaddr);

	/* Need to add size checking -- CMD 24 */
	MMC_CS_Assert();							// CS -> Low
	MMC_Send_Command(sdc, CMD24, argument);		// Send CMD24
	do {
		temp[0] = MMC_Receive_Byte();			// Receive and store
	} while (temp[0] != 0x00);					// Try until I get 0x00

	MMC_Send_Byte(0xFF);						// Dummy Byte
	MMC_CS_Deassert();							// CS -> High

	/* Re-assert CS to continue the transfer */
	MMC_CS_Assert();

	// Write needs an extra byte before writing
	MMC_Receive_Byte();

	/* Wait for the token */

	/*
	do {
		temp[0] = MMC_Receive_Byte();
	} while (temp[0] != 0xFE);
	*/

	for (i=0; i< SD_BLOCKSIZE; i++) {
		MMC_Send_Byte(data[i]);
	}

	do {
		temp[0] = MMC_Receive_Byte();				// Receive and store
	} while (temp[0] == 0xFF);					// Try until I get 0x00

	do {
		temp[0] = MMC_Receive_Byte();				// Receive and store
	} while (temp[0] != 0xFF);					// Try until I get 0x00

	MMC_Send_Byte(0xFF);						// Dummy Byte
	MMC_CS_Deassert();							// CS -> High

	return 1;
}

void MMC_Wait_Notbusy (mmc_context_t *sdc)
{
	if (sdc->busyflag == 1)
	{
		while (MMC_Receive_Byte() != 0xFF);
		sdc->busyflag = 0;
	}
	/* Deassert CS */
	MMC_CS_Deassert();
	/* Send some extra clocks so the card can resynchronize on the next
	transfer */
	MMC_Delay(2);
}

void MMC_Shift_Align(unsigned char *ar) {
	unsigned char old;
	unsigned char new;
	old = 0x01;
	new = 0x02;
    int i;
    for (i = 0; i < SD_BLOCKSIZE; i++) {
    	new = ar[i];
    	old = old & 0x03;
    	old = old * 0x40;
    	new = new/0x04;
    	new = old | new;
    	old = ar[i];
    	ar[i] = new;
    }
}

