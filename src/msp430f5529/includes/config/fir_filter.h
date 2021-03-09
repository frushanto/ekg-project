#ifndef INCLUDES_CONFIG_FIR_FILTER_H_
#define INCLUDES_CONFIG_FIR_FILTER_H_

#include <msp430.h>
#include <driverlib.h>
#include <std_types.h>

/*function declaration*/
void fir_filter_init(void);
uint16_t fir_filter(uint16_t new_sample);

#endif /* INCLUDES_CONFIG_FIR_FILTER_H_ */
