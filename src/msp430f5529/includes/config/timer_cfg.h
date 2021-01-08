#ifndef INCLUDES_CONFIG_TIMER_CFG_H_
#define INCLUDES_CONFIG_TIMER_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>

/* Function declarations */
void Init_Timers(void);
void Init_Timer_A(void);
void Init_Timer_B(void);

/***** GLOBALE VARS *****/

extern uint8_t uart_puls_counter;
extern uint8_t adc_start_stop;
extern uint8_t timer_start_stop;

extern uint16_t adc_result;

#endif /* INCLUDES_CONFIG_TIMER_CFG_H_ */
