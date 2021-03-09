#include "includes/functions/uart_functions.h"

uint16_t adc_value = 0;

void Uart_ECG_Wave_ST(void)
{
    adc_value = ((g_adc_result / 20));
    uart_transmit_data_start("add 1,0,");
    uart_transmit_data_value(adc_value);
    uart_transmit_data_end();
}

void Uart_ECG_Wave_LT(void)
{
    adc_value = ((g_adc_result / 20));
    uart_transmit_data_start("add 7,0,");
    uart_transmit_data_value(adc_value);
    uart_transmit_data_end();
}

void Clear_Wave_ST(void)
{
    uart_transmit_data_start("cle 1,0");
    uart_transmit_data_end();
}

void Clear_Wave_LT(void)
{
    uart_transmit_data_start("cle 7,0");
    uart_transmit_data_end();
}

void Short_ECG_Error(void)
{
    uart_transmit_data_start("page 5");
    uart_transmit_data_end();
}

void Long_ECG_Error(void)
{
    uart_transmit_data_start("page 6");
    uart_transmit_data_end();
}

void SD_Card_Error(void)
{
    uart_transmit_data_start("page 11");
    uart_transmit_data_end();
}

void Check_Akku_Percentage(void)
{
    if (g_akku_percentage < 80)
    {
        Akku_80_Error();
        g_long_ECG_flag = 0;
        g_sys_state = IDLE_STATE;
    }
}

void Akku_80_Error(void)
{
    uart_transmit_data_start("page 12");
    uart_transmit_data_end();
}

void ST_ECG(void)
{
    Uart_ECG_Wave_ST();
    calculate_bpm_ST();
}

void LT_ECG(void)
{
    Uart_ECG_Wave_LT();
}

void ECG_Timer_LT(void)
{
    uart_transmit_data_start("page3.minutes.val=");
    uart_transmit_data_value(g_cnt_min_long);
    uart_transmit_data_end();

    uart_transmit_data_start("page3.hours.val=");
    uart_transmit_data_value(g_cnt_hour_long);
    uart_transmit_data_end();

    if (g_cnt_hour_long == 24)
    {
        g_long_ECG_flag = 0;
    }
}

void Clear_ECG_Timer_LT(void)
{
    g_cnt_sec_long = 0;
    g_cnt_min_long = 0;
    g_cnt_hour_long = 0;

    uart_transmit_data_start("page3.minutes.val=");
    uart_transmit_data_value(g_cnt_min_long);
    uart_transmit_data_end();

    uart_transmit_data_start("page3.hours.val=");
    uart_transmit_data_value(g_cnt_hour_long);
    uart_transmit_data_end();
}

void ECG_Timer_ST(void)
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

void Display_Exit_Sleep_Mode(void)
{
    uart_transmit_data_start("sleep=");
    uart_transmit_data_value(0);
    uart_transmit_data_end();
}

void Display_Tachykardie(void)
{
    uart_transmit_data_start("page 17");
    uart_transmit_data_end();
}

void Display_Bradykardie(void)
{
    uart_transmit_data_start("page 18");
    uart_transmit_data_end();
}

void Check_BT_Connection(void)
{
    // Check if BT is connetced and show it on Display
    if (g_bt_state_flag == 1)
    {
        Display_Exit_Sleep_Mode();
        GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
        g_bt_connected = TRUE;
        Set_Bluetooth_Icon_Display(1);
        g_bt_state_flag = 2;
    }
    else if (g_bt_state_flag == 0)
    {
        Display_Exit_Sleep_Mode();
        GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
        g_bt_connected = FALSE;
        Set_Bluetooth_Icon_Display(0);
        g_bt_state_flag = 2;
    }
}
