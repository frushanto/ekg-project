#include <includes/config/timer_cfg.h>


/*** Module Variables ***/
//Endless counter with 1kHz
static uint32_t counter_1khz = 0;
static bool idle_1hz_send_bt_flag = false;

void Init_Timers() {
    Init_Timer_A();
}

void Init_Timer_A() {

    // Initialize Timer A0 for 1kHz
    Timer_A_initUpModeParam confTimerA0 = {0};
    confTimerA0.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // 20447232Hz
    confTimerA0.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64; // -> 20447232Hz / 64 = 319488Hz
    confTimerA0.timerPeriod = 319; // 319488Hz / 1000Hz == 319 // 1278 == 250 Hz
    confTimerA0.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    confTimerA0.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    confTimerA0.timerClear = TIMER_A_DO_CLEAR;
    confTimerA0.startTimer = true;

    //Start Timer A0
    Timer_A_initUpMode(TIMER_A0_BASE, &confTimerA0);



    // Start timer
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);

    /*** Init TIMER_A1 sourced by ACLK ***/
    Timer_A_initUpModeParam confTimerA1 = {0};
    confTimerA1.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    confTimerA1.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    confTimerA1.timerPeriod = 32768;
    confTimerA1.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    confTimerA1.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    confTimerA1.timerClear = TIMER_A_DO_CLEAR;
    confTimerA1.startTimer = true;
    Timer_A_initUpMode(TIMER_A1_BASE, &confTimerA1);

    // Start timer
    Timer_A_clearTimerInterrupt(TIMER_A2_BASE);

    /*** Init TIMER_A2 sourced by SMCLK ***/
        Timer_A_initUpModeParam confTimerA2 = {0};
        confTimerA2.clockSource = TIMER_A_CLOCKSOURCE_SMCLK; // 20447232Hz
        confTimerA2.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64; // -> 20447232Hz / 64 = 319488Hz
        confTimerA2.timerPeriod = 1278; // 319488Hz / 1000Hz == 319 // 1278 == 250 Hz
        confTimerA2.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
        confTimerA2.captureCompareInterruptEnable_CCR0_CCIE =
            TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
        confTimerA2.timerClear = TIMER_A_DO_CLEAR;
        confTimerA2.startTimer = true;
        Timer_A_initUpMode(TIMER_A2_BASE, &confTimerA2);
}

/*** Getter for Time since Startup in milliseconds  ***/
uint32_t get_ms (void)
{
    return counter_1khz;
}

/*** Getter for Time since Startup in milliseconds  ***/
bool get_1hz_flag_bt (void)
{
    if (idle_1hz_send_bt_flag){
        //Reset Flag in Function
        idle_1hz_send_bt_flag = 0;
        return true;
    }
    return false;
}

//Timer A0 Interupt 1kHz
#pragma vector=TIMER0_A0_VECTOR
__interrupt
void TIMER0_A0_ISR (void)
{
    //increment endless-counter
    counter_1khz++;

    //Send BT value with 200Hz
    if ( (g_sys_state == ECG_SHORT) && (counter_1khz % 5 == 0) )
        send_bt_value_dma(g_adc_result);

    if (counter_1khz % 1 == 0)
        idle_1hz_send_bt_flag = true;

    if((g_sys_state == ECG_LONG) && (counter_1khz % 4 == 0)){ 
        Start_ADC();                 // use it!!!
    }

    if(g_buzzer_20_percent == TRUE && g_buzzer_1kz_cnt)
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P6, GPIO_PIN1);
        g_buzzer_1kz_cnt ++;
        if(g_buzzer_1kz_cnt == 500)
        {
            g_buzzer_1kz_cnt = 0;
            g_buzzer_20_percent = FALSE;
            GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN1);
        }
    }
}



//******************************************************************************
//
//This is the TIMER1_A0 interrupt vector service routine.
// TIMER 1 HZ
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER1_A0_VECTOR)))
#endif
void TIMER1_A0_ISR (void)
{
    // Sync timer counting for display & sd card
	g_timer_1sec_flag = 1;

    // Sync timer for ECG Short and Long
    if(g_short_ECG_flag || g_long_ECG_flag){
    g_timer_uart_1sec = 1;
	}
}

//******************************************************************************
//
//This is the TIMER2_A0 interrupt vector service routine.
// TIMER 250 HZ
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER2_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER2_A0_VECTOR)))
#endif
void TIMER2_A0_ISR (void)
{
    g_timer_250Hz_flag = 1;

    if(g_buzzer_on_flag){
        g_timer_250Hz_Buzzer = 1;
        g_buzzer_cnt++;
    }
}
