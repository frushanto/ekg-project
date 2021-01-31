#include <includes/config/timer_cfg.h>

/*
 * Date: 2021-01-22
 * Author: Ivan Kozlov
 * Description: Timer Setup Parameters
 * 
 * ============================ TIMER A ============================
 * Timer A is triggered by SMCLK = 20447232Hz.
 *
 * Timer A1 has a clock divider = 32, which decreases timer triggered
 * frequency down to 20447232Hz / 32 = 638976Hz.
 * Timer A1 triggered frequency = 638976Hz.
 *
 * Timer A2 has a clock divider = 32, which decreases timer triggered
 * frequency down to 20447232Hz / 32 = 638976Hz.
 * Timer A2 triggered frequency = 638976Hz.
 * 
 * Timer A1 is used for 1kHz interrupt routine.
 * Timer A2 is used for 100Hz interrupt routine.
 * =================================================================
 * 
 * ============================ TIMER B ============================
 * Timer B is triggered by ACLK = 32768Hz.
 * Timer B0 has a clock divider = 1, which has NO influence on timer
 * frequency. Timer B0 frequency = 32768Hz.
 * 
 * Timer B0 is used for 1Hz interrupt routine.
 * =================================================================
 */

/* 
 * ===================== CONSTANTS CALCULATION =====================
 * 
 * Timer B0 constant calculation
 * ACLK = 32768Hz 
 * DIVIDER = 1
 * Compare value = ACLK / Desired frequency / DIVIDER
 * => Compare value = 32768Hz / 1Hz / 1
 * => Compare value = 32768
 */
uint16_t const timer_b0_compare_value_1hz_aclk = 32768;

/* 
 * Timer A1 constant calculation
 * SMCLK = 20447232Hz
 * DIVIDER = 32
 * Compare value = SMCLK / Desired frequency / DIVIDER
 * => Compare value = 20447232Hz / 1000Hz / 32
 * => Compare value = 639
 */
uint16_t const timer_a1_compare_value_1khz_smclk = 639;

/* 
 * Timer A2 constant calculation
 * SMCLK = 20447232Hz
 * DIVIDER = 32
 * Compare value = SMCLK / Desired frequency / DIVIDER
 * => Compare value = 20447232Hz / 100Hz / 32
 * => Compare value = 6390
 */
uint16_t const timer_a2_compare_value_100hz_smclk = 6390;

/*
 * Timer test 
 */
uint32_t timer_a2_100hz_test = 0;
uint32_t timer_a1_1khz_test = 0;
uint32_t timer_b0_1hz_test = 0;

 /* ============================================================= */

void Init_Timers() {
    Init_Timer_A();
}

void Init_Timer_A() {

    // Start timer
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);

    /*** Init TIMER_A1 sourced by ACLK ***/
    Timer_A_initUpModeParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 32; //(32768 / 4);
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    param.timerClear = TIMER_A_DO_CLEAR;
    param.startTimer = true;
    Timer_A_initUpMode(TIMER_A1_BASE, &param);
}

//******************************************************************************
//
//This is the TIMER1_A0 interrupt vector service routine.
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
    // Put code for LEDs here
    //Toggle P1.0
    GPIO_toggleOutputOnPin(
		GPIO_PORT_P2,
		GPIO_PIN3
		);
//    GPIO_toggleOutputOnPin(
//    		GPIO_PORT_P2,
//    		GPIO_PIN4
//    		);

}
