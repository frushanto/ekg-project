#ifndef INCLUDES_FUNCTIONS_UART_FUNCTIONS_H_
#define INCLUDES_FUNCTIONS_UART_FUNCTIONS_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include <includes/config/uart_cfg.h>
#include <includes/functions/bpm_function.h>
#include <includes/config/adc_cfg.h>

/* Function declarations */
void Uart_ECG_Wave_ST(void);
void Uart_ECG_Wave_LT(void);
void Test_Plus_Eins(void);
void Test_Minus_Eins(void);
void Clear_Wave_ST(void);
void Clear_Wave_LT(void);
void Short_ECG_Error(void);
void Long_ECG_Error(void);
void SD_Card_Error(void);
void Check_Akku_Percentage(void);
void Akku_80_Error(void);
void ST_ECG(void);
void LT_ECG(void);
void ECG_Timer_LT(void);
void ECG_Timer_ST(void);
void Clear_ECG_Timer_ST(void);
void Clear_ECG_Timer_LT(void);
void Set_Bluetooth_Icon_Display(uint8_t state);
void Set_SD_Icon_Display(uint8_t state);
void Check_BT_Connection(void);
void Display_Exit_Sleep_Mode(void);
void Display_Tachykardie(void);
void Display_Bradykardie(void);

#endif /* INCLUDES_FUNCTIONS_UART_FUNCTIONS_H_ */
