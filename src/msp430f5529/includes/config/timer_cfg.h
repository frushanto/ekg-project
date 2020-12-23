#ifndef INCLUDES_CONFIG_TIMER_CFG_H_
#define INCLUDES_CONFIG_TIMER_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>

/* Function declarations */
void Init_Timers(void);
void Init_Timer_A(void);
void Init_Timer_B(void);

#endif /* INCLUDES_CONFIG_TIMER_CFG_H_ */
