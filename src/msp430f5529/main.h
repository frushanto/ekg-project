/*
 * main.h
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <msp430.h>
#include <includes/driverlib/driverlib.h>
#include <includes/driverlib/std_types.h>
#include "includes/config/adc_cfg.h"
#include "includes/config/uart_cfg.h"
#include "includes/config/gpio_cfg.h"
#include "includes/config/clk_cfg.h"
#include "includes/config/timer_cfg.h"
#include "includes/config/fir_filter.h"
#include "includes/config/iir_filter.h"
#include "includes/config/spi_cfg.h"
#include "includes/config/mmc-module-cfg/mmc_cfg.h"
#include "includes/config/mmc-module-cfg/mmc_comm_spi_cfg.h"
#include "includes/config/mmc-module-cfg/mmc_spi_cfg.h"
#include "includes/functions/uart_functions.h"
#include "includes/functions/bpm_function.h"

typedef enum
{
    SYS_INIT,
    SYS_NO_ACTION,  // "Leerlauf" - ohne Funktion
    ECG_SHORT,
    ECG_LONG,
    ENERGY_SAVING_MODE,
    IDLE_STATE,
    SYS_ERROR,
    SYS_DIRTY_START,
    SYS_BAD_KEY
} STATE_MACHINE_e;

/***** GLOBALE VARS *****/
extern uint8_t g_short_ECG_flag;
extern uint8_t g_long_ECG_flag;
extern uint8_t g_timer_1khz_flag;
extern uint8_t g_timer_1sec_flag;
extern uint16_t g_adc_result;
extern STATE_MACHINE_e g_sys_state;

#endif /* MAIN_H_ */
