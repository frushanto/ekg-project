#ifndef INCLUDES_CONFIG_TIMER_CFG_H_
#define INCLUDES_CONFIG_TIMER_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include "main.h"

/* Function declarations */
void Init_Timers(void);
void Init_Timer_A(void);
void Init_Timer_B(void);


//extern uint8_t flag;
//extern uint16_t uart_timer_one_sec;
//extern uint8_t adc_ready;
//extern uint8_t centisecond;


#endif /* INCLUDES_CONFIG_TIMER_CFG_H_ */
