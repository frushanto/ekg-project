#include <includes/config/uart_cfg.h>

#define UART_BASE_A0 0
#define UART_BASE_A1 1

#define UART_BASE UART_BASE_A0
#define UART_MESSAGE_MAX_LENGTH 12
#define UART_TX_MAX_LENGTH 24

#define RECEIVE_DATA_COUNT 0x02

uint8_t uart_received_data[UART_MESSAGE_MAX_LENGTH] = {0x00};
uint8_t uart_received_data_counter = 0;

char uart_transmit_set_val[UART_TX_MAX_LENGTH] = {0x00};

uint8_t uart_transmit_full_message[UART_TX_MAX_LENGTH] = {0x00};
volatile uint8_t i = 0;
volatile uint8_t fm_counter = 0;

//******************************//
//*UART TRANSMIT DATA FUNCTIONS*//
//*START************************//
/* Transmit array with Nextion command */
void uart_transmit_data_start(char nextion_command[])
{
    memset(uart_transmit_full_message, 0x00, UART_TX_MAX_LENGTH);
    fm_counter = 0;
    memset(uart_transmit_set_val, 0x00, UART_TX_MAX_LENGTH);
    strcpy(uart_transmit_set_val, nextion_command);
    for (i = 0; i < strlen((char const *)uart_transmit_set_val); i++)
    {
        USCI_A_UART_transmitData(USCI_A0_BASE, uart_transmit_set_val[i]);
        uart_transmit_full_message[fm_counter] = uart_transmit_set_val[i];
        fm_counter++;
        /* Wait transmission is completed */
        while (USCI_A_UART_queryStatusFlags(
                   USCI_A0_BASE, USCI_A_UART_BUSY) == USCI_A_UART_BUSY)
            ;
    }
}
/* Transmit array with value */
void uart_transmit_data_value(uint16_t transmit_value)
{
    uint16_t value = transmit_value;
    char buffer[50];
    sprintf(buffer, "%d", value);
    for (i = 0; i < strlen((char const *)buffer); i++)
    {
        USCI_A_UART_transmitData(USCI_A0_BASE, buffer[i]);
        uart_transmit_full_message[fm_counter] = buffer[i];
        fm_counter++;
        /* Wait transmission is completed */
        while (USCI_A_UART_queryStatusFlags(
                   USCI_A0_BASE, USCI_A_UART_BUSY) == USCI_A_UART_BUSY)
            ;
    }
}
/* Transmit 3 times 0xFF to send command to display */
void uart_transmit_data_end()
{
    uint8_t uart_transmit_cmd_ff = 0xFF;
    for (i = 0; i < 3; i++)
    {
        USCI_A_UART_transmitData(USCI_A0_BASE, uart_transmit_cmd_ff);
        uart_transmit_full_message[fm_counter] = uart_transmit_cmd_ff;
        fm_counter++;
        /* Wait transmission is completed */
        while (USCI_A_UART_queryStatusFlags(
                   USCI_A0_BASE, USCI_A_UART_BUSY) == USCI_A_UART_BUSY)
            ;
    }
}
//******************************//
//*UART TRANSMIT DATA FUNCTIONS*//
//*END**************************//

//******************************//
//*UART RECEIVED DATA FUNCTIONS*//
//*START************************//
void uart_receive_data_end()
{
    uart_received_data_counter = 0;
    memset(uart_received_data, 0x00, UART_MESSAGE_MAX_LENGTH);
}
//******************************//
//*UART RECEIVED DATA FUNCTIONS*//
//*END**************************//

void Init_UART()
{

#if UART_BASE == UART_BASE_A0
    /* Configure GPIO pins for UART A0 BASE
     * P3.4/UCA0RXD/UCA0SOMI
     * P3.3/UCA0TXD/UCA0SIMO
     * */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P3, GPIO_PIN4); // UART Rx Pin - P3.4
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P3, GPIO_PIN3); // UART Tx Pin - P3.3

#elif UART_BASE == UART_BASE_A1

    /* Configure GPIO pins for UART A1 BASE
     * P4.5/UCA1RXD
     * P4.4/UCA1TXD
     * */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P4, GPIO_PIN5); // UART Rx Pin
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P4, GPIO_PIN4); // UART Tx Pin

#else
#error "Please specify UART BASE"
#endif

    USCI_A_UART_initParam uart_cfg = {0};

    /* 
     * From: Table 39-5. Recommended Settings for
     * Typical Crystals and Baud Rates at Page 1039/1189 User guide.
     * For more information about baudrate setting see 39.3.10
     * Setting a Baud Rate at page 1036 User Guide
     */

    /*************************** README ***************************/
    /**************************************************************/
    /**************************************************************/
    /*                  BAUDRATE CHANGED TO 115200                */
    /**************************************************************/
    /**************************************************************/

    //uart_cfg.selectClockSource = USCI_A_UART_CLOCKSOURCE_ACLK;
    uart_cfg.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK; // -> 20.000.000 Hz (20MHz)
    uart_cfg.clockPrescalar = 173;                              //173 for 20Mhz -> Table 36-4, p.952 User's Guide
    uart_cfg.firstModReg = 0;
    uart_cfg.secondModReg = 5; //5 for 20Mhz
    uart_cfg.parity = USCI_A_UART_NO_PARITY;
    uart_cfg.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    uart_cfg.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    uart_cfg.uartMode = USCI_A_UART_MODE;
    uart_cfg.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    // Init UART A0
    if (STATUS_FAIL == USCI_A_UART_init(USCI_A0_BASE, &uart_cfg))
    {
        return;
    }
    USCI_A_UART_enable(USCI_A0_BASE);
    USCI_A_UART_clearInterrupt(USCI_A0_BASE,
                               USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(USCI_A0_BASE,
                                USCI_A_UART_RECEIVE_INTERRUPT);
}

/*
 * EUSCI_A_UART_enable() enables the EUSI_A_UART and the module
 * is now ready for transmit and receive. It is recommended to
 * initialize the EUSI_A_UART via EUSCI_A_UART_init(), enable
 * the required interrupts and then enable EUSI_A_UART via
 * EUSCI_A_UART_enable().
 * */

#pragma vector = USCI_A0_VECTOR
__interrupt void UART_A0_ISR(void)
{
    switch (__even_in_range(UCA0IV, 4))
    {
    case 0:
        break; // Vector 0 - no interrupt
    case 2:    // Vector 2 - RXIFG
        if (uart_received_data_counter < UART_MESSAGE_MAX_LENGTH)
        {
            uart_received_data[uart_received_data_counter] =
                USCI_A_UART_receiveData(USCI_A0_BASE);
            uart_received_data_counter++;
        }
        else
        {
            uart_received_data_counter = 0;
        }

        /* Display turn on: 0=0x87 1=0xFF 2=0xFF 3=0xFF */
        if (uart_received_data[0] == 0x87 && uart_received_data[1] == 0xFF && uart_received_data[2] == 0xFF && uart_received_data[3] == 0xFF)
        {
            uart_receive_data_end();
        }

        /* Display Sleep X,Y: 0=0x68 6=0xFF 7=0xFF 8=0xFF */
        else if (uart_received_data[0] == 0x68 && uart_received_data[6] == 0xFF && uart_received_data[7] == 0xFF && uart_received_data[8] == 0xFF)
        {
            uart_receive_data_end();
        }

        /* Display page2 'kurzzeit' ECG ***START***: 65 02 06 00 FF FF FF */
        else if (uart_received_data[0] == 0x65 && uart_received_data[1] == 0x02 && uart_received_data[2] == 0x06 && uart_received_data[3] == 0x00 &&
                 uart_received_data[4] == 0xFF && uart_received_data[5] == 0xFF && uart_received_data[6] == 0xFF)
        {
            g_short_ECG_flag = 1;
            uart_receive_data_end();
        }

        /* Display page2 'kurzzeit' ECG ***STOP***: 65 07 01 00 FF FF FF */
        else if (uart_received_data[0] == 0x65 && uart_received_data[1] == 0x07 && uart_received_data[2] == 0x01 && uart_received_data[3] == 0x00 &&
                 uart_received_data[4] == 0xFF && uart_received_data[5] == 0xFF && uart_received_data[6] == 0xFF)
        {
            g_short_ECG_flag = 0;
            uart_receive_data_end();
        }

        /* Display page3 'langzeit' ECG ***START***: 65 03 02 00 FF FF FF */
        else if (uart_received_data[0] == 0x65 && uart_received_data[1] == 0x03 && uart_received_data[2] == 0x02 && uart_received_data[3] == 0x00 &&
                 uart_received_data[4] == 0xFF && uart_received_data[5] == 0xFF && uart_received_data[6] == 0xFF)
        {
            g_long_ECG_flag = 1;
            uart_receive_data_end();
        }

        /* Display page3 'langzeit' ECG ***STOP***: 65 08 01 00 FF FF FF */
        else if (uart_received_data[0] == 0x65 && uart_received_data[1] == 0x08 && uart_received_data[2] == 0x01 && uart_received_data[3] == 0x00 &&
                 uart_received_data[4] == 0xFF && uart_received_data[5] == 0xFF && uart_received_data[6] == 0xFF)
        {
            g_long_ECG_flag = 0;
            uart_receive_data_end();
        }

        /* Display select USER ONE: 65 04 03 00 FF FF FF */
        else if (uart_received_data[0] == 0x65 && uart_received_data[1] == 0x04 && uart_received_data[2] == 0x03 && uart_received_data[3] == 0x00 &&
                 uart_received_data[4] == 0xFF && uart_received_data[5] == 0xFF && uart_received_data[6] == 0xFF)
        {
            g_user_select = 1;
            uart_receive_data_end();
        }

        /* Display select USER TWO: 65 04 04 00 FF FF FF */
        else if (uart_received_data[0] == 0x65 && uart_received_data[1] == 0x04 && uart_received_data[2] == 0x04 && uart_received_data[3] == 0x00 &&
                 uart_received_data[4] == 0xFF && uart_received_data[5] == 0xFF && uart_received_data[6] == 0xFF)
        {
            g_user_select = 2;
            uart_receive_data_end();
        }

        /* Display select USER THREE: 65 04 05 00 FF FF FF */
        else if (uart_received_data[0] == 0x65 && uart_received_data[1] == 0x04 && uart_received_data[2] == 0x05 && uart_received_data[3] == 0x00 &&
                 uart_received_data[4] == 0xFF && uart_received_data[5] == 0xFF && uart_received_data[6] == 0xFF)
        {
            g_user_select = 3;
            uart_receive_data_end();
        }

        /* Display Interrupt when 5V turned ON */
        else if ((uart_received_data[0] < 0x65) ||
                 ((uart_received_data[0] > 0x65) && (uart_received_data[0] < 0x68)) ||
                 ((uart_received_data[0] > 0x68) && (uart_received_data[0] < 0x87)) ||
                 (uart_received_data[0] > 0x87))
        {
            g_user_select = 0;
            uart_receive_data_end();

            // For ECG_LONG Energy Saving Mode
            if ((g_sys_state == ECG_LONG) && (display_sleep_mode == TRUE))
            {
                if ((g_long_ecg_state == MODE_5V_OFF) || (g_long_ecg_state == MODE_5V_ON))
                {
                    // Set Display to Sleep Mode
                    uart_transmit_data_start("sleep=");
                    uart_transmit_data_value(1);
                    uart_transmit_data_end();
                }
                else if (g_long_ecg_state == MODE_NORMAL)
                {
                    Display_Exit_Sleep_Mode();
                    uart_transmit_data_start("page 3");
                    uart_transmit_data_end();
                    display_sleep_mode = FALSE;
                }
            }
        }

        USCI_A_UART_clearInterrupt(USCI_A0_BASE,
                                   USCI_A_UART_RECEIVE_INTERRUPT);
        break;
    case 4:
        break; // Vector 4 - TXIFG
    default:
        break;
    }
}
