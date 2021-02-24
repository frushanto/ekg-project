/*
 * clk_cfg.h
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#ifndef INCLUDES_CONFIG_CLK_CFG_H_
#define INCLUDES_CONFIG_CLK_CFG_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>

/* DEFINES */

//Target frequency for MCLK in kHz
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ   20000   // 20 MHz
#define FLLREF_KHZ                          32
// MCLK/FLLRef Ratio
// 20000 kHz / 32 kHz = 625 kHz
#define UCS_MCLK_FLLREF_RATIO       UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ/FLLREF_KHZ


/* Function declarations */
void Init_CLK(void);

#endif /* INCLUDES_CONFIG_CLK_CFG_H_ */
