#ifndef INCLUDES_CONFIG_BLUETOOTH_H_
#define INCLUDES_CONFIG_BLUETOOTH_H_

//Includes
#include "usci_a_uart.h"
#include "driverlib.h"
#include <stdint.h>
#include "main.h"

//Defines
#define BAUD_RATE 38400

//Prototypes
bool Init_UART_BT(void); //Init USCI_A0
bool send_bt_value(uint16_t value);
bool send_bt_string(char *str);
bool send_bt_value_dma(uint16_t value);

#endif /* INCLUDES_CONFIG_BLUETOOTH_H_ */
