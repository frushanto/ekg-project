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
uint32_t get_ms (void);
bool get_1hz_flag_bt (void);

#endif /* INCLUDES_CONFIG_TIMER_CFG_H_ */
