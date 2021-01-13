#include <includes/config/timer_cfg.h>

#define CLOCK_DIVIDER           64
#define ONE_KHZ                 1000
#define COMPARE_VALUE_ONE_SEC_ACLK      32768
#define COMPARE_VALUE_ONE_SEC   11993088
#define COMPARE_VALUE_10_MS     COMPARE_VALUE_ONE_SEC / 100
#define COMPARE_VALUE_1KHZ      COMPARE_VALUE_ONE_SEC / (CLOCK_DIVIDER * ONE_KHZ)

uint16_t compare_value_one_khz = 187*10;

void Init_Timers() {
    Init_Timer_A();
    Init_Timer_B();
}

void Init_Timer_A() {

    /*** BEGIN Init TIMER_A1 in continuous mode sourced by ACLK ***/
    Timer_A_initContinuousModeParam initContParamTimerA1 = {0};
    initContParamTimerA1.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initContParamTimerA1.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64;
    initContParamTimerA1.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParamTimerA1.timerClear = TIMER_A_DO_CLEAR;
    initContParamTimerA1.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParamTimerA1);
    /*** END Init TIMER_A1 in continuous mode sourced by ACLK ***/

    /*** BEGIN Init COMPARE MODE by TIMER_A1 - ONE KHZ ***/
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0
        );

    Timer_A_initCompareModeParam initCompParamOneKHz = {0};
    initCompParamOneKHz.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParamOneKHz.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParamOneKHz.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    // Define CLK cycles for CCR0 Interrupt
    initCompParamOneKHz.compareValue = compare_value_one_khz;
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParamOneKHz);

    Timer_A_startCounter( TIMER_A1_BASE,
            TIMER_A_CONTINUOUS_MODE
                );
    /*** END Initialize compare mode ONE KHZ ***/

    /*** BEGIN Init TIMER_A2 in continuous mode sourced by ACLK ***/
    Timer_A_initContinuousModeParam initContParamTimerA2 = {0};
    initContParamTimerA2.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
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

void Init_Timer_B() {
    /*** BEGIN Init TIMER_B0 in continuous mode sourced by ACLK ***/
    Timer_B_initContinuousModeParam initContParamTimerB0 = {0};
    initContParamTimerB0.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
    initContParamTimerB0.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    initContParamTimerB0.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    initContParamTimerB0.timerClear = TIMER_B_DO_CLEAR;
    initContParamTimerB0.startTimer = false;
    Timer_A_initContinuousMode(TIMER_B0_BASE, &initContParamTimerB0);
    /*** END Init TIMER_B0 in continuous mode sourced by ACLK ***/

    /*** BEGIN Init COMPARE MODE by TIMER_B0 - ONE SEC ***/
    Timer_B_clearCaptureCompareInterrupt(TIMER_B0_BASE,
        TIMER_B_CAPTURECOMPARE_REGISTER_0
        );

    Timer_B_initCompareModeParam initCompParamOneSec = {0};
    initCompParamOneSec.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_0;
    initCompParamOneSec.compareInterruptEnable = TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParamOneSec.compareOutputMode = TIMER_B_OUTPUTMODE_OUTBITVALUE;
    // Define CLK cycles for CCR0 Interrupt
    initCompParamOneSec.compareValue = COMPARE_VALUE_ONE_SEC_ACLK;
    Timer_B_initCompareMode(TIMER_B0_BASE, &initCompParamOneSec);

    Timer_B_startCounter( TIMER_B0_BASE,
            TIMER_B_CONTINUOUS_MODE
                );
    /*** END Initialize compare mode ONE SEC ***/
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
            + compare_value_one_khz;

    /*** BEGIN will be executed every 1 KHz ***/

//    GPIO_toggleOutputOnPin(GPIO_PORT_P4,
//                                   GPIO_PIN7);

    timer_1hz_flag = 1;


//    Test_ADC();

    /*** END will be executed every 1 KHz ***/

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

//    timer_start_stop = 1;

//    Test_ADC();                         /***********/

    /*** END will be executed every 10 milliseconds ***/

    // Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A2_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0,
        compVal
        );
}

//******************************************************************************
//
//This is the TIMER0_B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER0_B0_VECTOR)))
#endif
void TIMER0_B0_ISR (void)
{
    uint16_t compVal = Timer_B_getCaptureCompareCount(TIMER_B0_BASE,
            TIMER_B_CAPTURECOMPARE_REGISTER_0)
            + COMPARE_VALUE_ONE_SEC_ACLK;

    /*** BEGIN will be executed every 1 SEC ***/
    GPIO_toggleOutputOnPin(GPIO_PORT_P4,
                                   GPIO_PIN7);
//    timer_start_stop = 1;
    timer_1sek_flag = 1;

//    Test_ADC();
//    UART_Timer_One_Sec();
//    UART_Timer_Two_Sec();
//    UART_Timer_Three_Sec();

    /*** END will be executed every 1 SEC ***/
    // Add Offset to CCR0
    Timer_B_setCompareValue(TIMER_B0_BASE,
        TIMER_B_CAPTURECOMPARE_REGISTER_0,
        compVal
        );
}
