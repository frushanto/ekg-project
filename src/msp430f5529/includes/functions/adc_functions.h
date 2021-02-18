#ifndef INCLUDES_FUNCTIONS_ADC_FUNCTIONS_H_
#define INCLUDES_FUNCTIONS_ADC_FUNCTIONS_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include "main.h"

/* Function declarations */
void ADC_Akku_Average_Value(void);
extern uint32_t g_akku_percentage;

#endif /* INCLUDES_FUNCTIONS_ADC_FUNCTIONS_H_ */
