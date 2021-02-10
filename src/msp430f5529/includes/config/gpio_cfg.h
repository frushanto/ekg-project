/*
 * gpio_cfg.h
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#ifndef INCLUDES_CONFIG_GPIO_CFG_H_
#define INCLUDES_CONFIG_GPIO_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>

/* Function declarations */
void Init_GPIO(void);
void GPIO_Buzzer_Single_Beep(void);

#endif /* INCLUDES_CONFIG_GPIO_CFG_H_ */
