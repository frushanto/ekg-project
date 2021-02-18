/*
 * uart_functions.c
 *
 *  Created on: 17.01.2021
 *      Author: hannes
 */
#include "includes/functions/uart_functions.h"

uint16_t test_plus_minus = 80;
uint16_t adc_value = 0;

void Uart_ECG_Wave_ST()
{
    adc_value = ((g_adc_result / 12)) - 80 ;   // TODO: edit value for Display
    uart_transmit_data_start("add 1,0,");
    uart_transmit_data_value(adc_value);
    uart_transmit_data_end();
}

void Uart_ECG_Wave_LT()
{
    adc_value = ((g_adc_result / 12) - 80);
    uart_transmit_data_start("add 13,0,");
    uart_transmit_data_value(adc_value);
    uart_transmit_data_end();
}

void Clear_Wave_ST()
{
    uart_transmit_data_start("cle 1,0");
    uart_transmit_data_end();
}

void Clear_Wave_LT()
{
    uart_transmit_data_start("cle 13,0");
    uart_transmit_data_end();
}

void Short_ECG_Error()
{
    uart_transmit_data_start("page 5");
    uart_transmit_data_end();
}

void Long_ECG_Error()
{
    uart_transmit_data_start("page 6");
    uart_transmit_data_end();
}

void SD_Card_Error()
{
   uart_transmit_data_start("page 11");
   uart_transmit_data_end();
}

void SD_Card_Timeout() 
{

}

void Akku_80_Error()
{
   uart_transmit_data_start("page 12");
   uart_transmit_data_end();
}

void ST_ECG()
{
    Uart_ECG_Wave_ST();
    calculate_bpm_ST();
    send_bt_value(g_adc_result);
}

void LT_ECG()
{
    Start_ADC();
    Uart_ECG_Wave_LT();
    calculate_bpm_LT();
    send_bt_value(g_adc_result);
}

void ECG_Timer_LT()
{
    if (g_timer_uart_1sec == 1)
    {
        g_timer_uart_1sec = 0;
        g_cnt_sec++;
        if (g_cnt_sec > 59 && g_cnt_min <= 59)
        {
            g_cnt_min++;
            g_cnt_sec = 0;
        }
        uart_transmit_data_start("page3.seconds.val=");
        uart_transmit_data_value(g_cnt_sec);
        uart_transmit_data_end();
        if (g_cnt_min > 59 && g_cnt_sec > 59)
        {
            g_cnt_hour++;
            g_cnt_min = 0;
            g_cnt_sec = 0;
        }
        uart_transmit_data_start("page3.minutes.val=");
        uart_transmit_data_value(g_cnt_min);
        uart_transmit_data_end();
        if (g_cnt_hour == 24)
        {
            g_cnt_hour = 0;
            g_cnt_min = 0;
            g_cnt_sec = 0;
        }
        uart_transmit_data_start("page3.hours.val=");
        uart_transmit_data_value(g_cnt_hour);
        uart_transmit_data_end();
    }
    if (g_cnt_hour == 24)
    {
        g_long_ECG_flag = 0;
    }
}

void Clear_ECG_Timer_LT(void)
{
    g_cnt_sec = 0;
    g_cnt_min = 0;
    g_cnt_hour = 0;
    g_timer_uart_1sec = 0;
    uart_transmit_data_start("page3.seconds.val=");
    uart_transmit_data_value(g_cnt_sec);
    uart_transmit_data_end();

    uart_transmit_data_start("page3.minutes.val=");
    uart_transmit_data_value(g_cnt_min);
    uart_transmit_data_end();

    uart_transmit_data_start("page3.hours.val=");
    uart_transmit_data_value(g_cnt_hour);
    uart_transmit_data_end();
}

void ECG_Timer_ST()
{
    if (g_timer_uart_1sec == 1)
    {
        g_timer_uart_1sec = 0;
        g_cnt_sec++;
        if (g_cnt_sec > 59 && g_cnt_min <= 59)
        {
            g_cnt_min++;
            g_cnt_sec = 0;
        }
        uart_transmit_data_start("page2.seconds.val=");
        uart_transmit_data_value(g_cnt_sec);
        uart_transmit_data_end();
        if (g_cnt_min > 59 && g_cnt_sec > 59)
        {
            g_cnt_min = 0;
            g_cnt_sec = 0;
        }
        uart_transmit_data_start("page2.minutes.val=");
        uart_transmit_data_value(g_cnt_min);
        uart_transmit_data_end();
    }
    if (g_cnt_min == 2)
    {
        g_short_ECG_flag = 0;
    }
}

void Clear_ECG_Timer_ST(void)
{
    g_cnt_sec = 0;
    g_cnt_min = 0;
    g_timer_uart_1sec = 0;
    uart_transmit_data_start("page2.seconds.val=");
    uart_transmit_data_value(g_cnt_sec);
    uart_transmit_data_end();

    uart_transmit_data_start("page2.minutes.val=");
    uart_transmit_data_value(g_cnt_min);
    uart_transmit_data_end();
}

void Set_Bluetooth_Icon_Display(uint8_t state)
{
    uart_transmit_data_start("page0.bluetooth.val=");
    uart_transmit_data_value(state);
    uart_transmit_data_end();
}

void Set_SD_Icon_Display(uint8_t state)
{
    uart_transmit_data_start("page0.sdcard.val=");
    uart_transmit_data_value(state);
    uart_transmit_data_end();
}

void Check_BT_Connection()
{
    // Check if BT is connetced and show it on Display
    if(g_bt_state_flag == 1)
    {
        GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
        g_bt_connected = TRUE;  // needed?
        Set_Bluetooth_Icon_Display(1);
        g_bt_state_flag = 2;
    }else if(g_bt_state_flag == 0)
    {
        GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
        g_bt_connected = FALSE; // needed?
        Set_Bluetooth_Icon_Display(0);
        g_bt_state_flag = 2;
    }
}