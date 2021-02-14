/*
 * gpio_cfg.h
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#ifndef INCLUDES_CONFIG_GPIO_CFG_H_
#define INCLUDES_CONFIG_GPIO_CFG_H_

#include <msp430.h>
#include <driverlib.h>
#include <std_types.h>
#include <main.h>

/* Function declarations */
void Init_GPIO(void);
void Buzzer_active(void);
void State_sys_Energy_Saving_Mode(void);
void State_sys_Wakeup_Mode(void);

#endif /* INCLUDES_CONFIG_GPIO_CFG_H_ */
