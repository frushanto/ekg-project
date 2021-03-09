#ifndef INCLUDES_FUNCTIONS_BPM_FUNCTION_H_
#define INCLUDES_FUNCTIONS_BPM_FUNCTION_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include <includes/config/uart_cfg.h>
#include "main.h"

/*Defines*/
#define tachy_brady_cnt_threshold 20
#define tachy_threshold 100
#define brady_threshold 60

/* Function declarations */
void calculate_bpm_ST(void);
void calculate_bpm_LT(void);

#endif /* INCLUDES_FUNCTIONS_BPM_FUNCTION_H_ */
