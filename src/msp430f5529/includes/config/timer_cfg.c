#include <includes/config/timer_cfg.h>

#define COMPARE_VALUE_ONE_SEC   32768
#define COMPARE_VALUE_10_MS     COMPARE_VALUE_ONE_SEC / 10 //100

void Init_Timer() {

    /*** BEGIN Init TIMER_A1 in continuous mode sourced by ACLK ***/
    Timer_A_initContinuousModeParam initContParamTimerA1 = {0};
    initContParamTimerA1.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    initContParamTimerA1.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initContParamTimerA1.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParamTimerA1.timerClear = TIMER_A_DO_CLEAR;
    initContParamTimerA1.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParamTimerA1);
    /*** END Init TIMER_A1 in continuous mode sourced by ACLK ***/

    /*** BEGIN Init COMPARE MODE by TIMER_A1 - ONE SECOND ***/
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0
        );

    Timer_A_initCompareModeParam initCompParamOneSec = {0};
    initCompParamOneSec.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParamOneSec.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParamOneSec.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    // Define CLK cycles for CCR0 Interrupt
    initCompParamOneSec.compareValue = COMPARE_VALUE_ONE_SEC;
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParamOneSec);

    Timer_A_startCounter( TIMER_A1_BASE,
    		TIMER_A_CONTINUOUS_MODE
                );
    /*** END Initialize compare mode ONE SECOND ***/

    /*** BEGIN Init TIMER_A2 in continuous mode sourced by ACLK ***/
    Timer_A_initContinuousModeParam initContParamTimerA2 = {0};
    initContParamTimerA2.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    initContParamTimerA2.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initContParamTimerA2.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParamTimerA2.timerClear = TIMER_A_DO_CLEAR;
    initContParamTimerA2.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A2_BASE, &initContParamTimerA2);
    /*** END Init TIMER_A2 in continuous mode sourced by ACLK ***/

    /*** BEGIN Init COMPARE MODE by TIMER_A2 - 10 MS ***/
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0
        );

    Timer_A_initCompareModeParam initCompParam10ms = {0};
    initCompParam10ms.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParam10ms.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParam10ms.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    // Define CLK cycles for CCR0 Interrupt
    initCompParam10ms.compareValue = COMPARE_VALUE_10_MS;
    Timer_A_initCompareMode(TIMER_A2_BASE, &initCompParam10ms);

    Timer_A_startCounter( TIMER_A2_BASE,
    		TIMER_A_CONTINUOUS_MODE
                );
    /*** END Init COMPARE MODE by TIMER_A2 - 10 MS ***/
}

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
    		+ COMPARE_VALUE_ONE_SEC;

    /*** BEGIN will be executed every 1 second ***/
    GPIO_toggleOutputOnPin(GPIO_PORT_P4,
                                   GPIO_PIN7);
//    UART_Timer_One_Sec();
    UART_Upper_T();
    UART_Lower_T();
//    Test_ADC();
    /*** END will be executed every 1 second ***/

    // Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0,
        compVal
        );
}

//******************************************************************************
//
//This is the TIMER2_A3 interrupt vector service routine.
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
    uint16_t compVal = Timer_A_getCaptureCompareCount(TIMER_A2_BASE,
    		TIMER_A_CAPTURECOMPARE_REGISTER_0)
    		+ COMPARE_VALUE_10_MS;

    /*** BEGIN will be executed every 10 milliseconds ***/
    GPIO_toggleOutputOnPin(GPIO_PORT_P1,
                                   GPIO_PIN0);
//    UART_Timer_One_Sec();
    Test_ADC();
//    UART_Dreieck();
    /*** END will be executed every 10 milliseconds ***/

    // Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A2_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0,
        compVal
        );
}
