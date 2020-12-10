#include <includes/config/timer_cfg.h>

#define COMPARE_VALUE 32768

void Init_Timer() {
    //Start timer in continuous mode sourced by ACLK
    Timer_A_initContinuousModeParam initContParam = {0};
    initContParam.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    initContParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParam.timerClear = TIMER_A_DO_CLEAR;
    initContParam.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

    //Initiaze compare mode
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0
        );

    Timer_A_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParam.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    // Define CLK cycles for CCR0 Interrupt
    initCompParam.compareValue = COMPARE_VALUE;
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);

    Timer_A_startCounter( TIMER_A1_BASE,
    		TIMER_A_CONTINUOUS_MODE
                );
}

//void Timer_one_sec(void) {
//    Timer_A_startCounter( TIMER_A1_BASE,
//                          TIMER_A_STOP_MODE
//                 );
//
//    Timer_A_initCompareModeParam initCompParam = {0};
//    initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
//    initCompParam.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
//    initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
//    initCompParam.compareValue = COMPARE_VALUE;
//    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);
//
//    Timer_A_startCounter( TIMER_A1_BASE,
//            TIMER_A_CONTINUOUS_MODE
//                );
//    // ACLK = 32768 Hz
//    uint16_t timerIdleCycles = (timer_delay_ms * 32768) / 1000;
//
//}

//******************************************************************************
//
//This is the TIMER1_A3 interrupt vector service routine.
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
    uint16_t compVal = Timer_A_getCaptureCompareCount(TIMER_A1_BASE,
    		TIMER_A_CAPTURECOMPARE_REGISTER_0)
    		+ COMPARE_VALUE;

    // TODO send UART to display
    UART_Timer_One_Sec();

    // Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0,
        compVal
        );
}
