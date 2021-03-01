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
#include "includes/config/bluetooth.h"
#include "includes/config/sd-card-cfg/diskio.h"
#include "includes/config/sd-card-cfg/ff.h"
#include "includes/config/sd-card-cfg/ffconf.h"
#include "includes/config/sd-card-cfg/HAL_SDCard.h"
#include "includes/config/sd-card-cfg/mmc.h"
#include "includes/functions/uart_functions.h"
#include "includes/functions/bpm_function.h"
#include "includes/functions/sd_card_functions.h"
#include "includes/functions/adc_functions.h"
#include "integer.h"
#include "stdio.h"
#include "time.h"
#include "includes/config/median_cfg.h"

#define G_SAMPLE_RATE 250
#define LONG_ECG_STORAGE_SIZE 1000

//Define this when testing on Launchpad, otherwise comment it out
// #define LAUNCHPAD

typedef enum
{
    SYS_INIT,
    SYS_NO_ACTION,  // "Leerlauf" - ohne Funktion
    ECG_SHORT,
    ECG_LONG,
    ENERGY_SAVING_MODE,
    IDLE_STATE,
    SYS_ERROR,
    SYS_WAKEUP,
    SYS_BAD_KEY
} STATE_MACHINE_e;

/***** GLOBALE VARS *****/
extern uint8_t g_short_ECG_flag;
extern uint8_t g_long_ECG_flag;
extern uint8_t g_timer_250Hz_flag;
extern uint8_t g_timer_1sec_flag;
extern uint16_t g_adc_result;
extern uint16_t g_adc_result_storage[];
extern uint16_t g_adc_result_storage_cpy[];
extern uint16_t g_adc_result_cnt;
extern bool g_adc_result_storage_full;
extern uint16_t g_akku_vol;
extern uint8_t g_tmp_return;
extern uint16_t g_writingCyclesCnt;
extern STATE_MACHINE_e g_sys_state;
extern sMedianFilter_t medianFilter;
extern uint8_t g_timer_uart_1sec;
extern uint8_t g_5v_flag;
extern bool g_ecg_long_5v_on;
extern bool g_ecg_long_btn_pressed;
extern uint8_t g_cnt_sec;
extern uint8_t g_cnt_min;
extern uint8_t g_cnt_hour;
extern uint16_t g_cnt_msec_long;
extern uint8_t g_cnt_sec_long;
extern uint8_t g_cnt_min_long;
extern uint8_t g_cnt_hour_long;

extern bool g_sd_card_inserted;
extern bool g_bt_connected;
extern bool g_adc_new_values;
// extern bool g_display_sleep_flag;

extern uint8_t g_timer_250Hz_Buzzer;
extern uint8_t g_buzzer_1sec_flag;
extern uint8_t g_buzzer_on_flag;
extern uint16_t g_buzzer_cnt;

extern uint8_t bt_flag;

extern uint8_t g_user_select;
/*** Prototypes ***/
void delay(uint16_t delay_time);

#endif /* MAIN_H_ */
