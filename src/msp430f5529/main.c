#include "main.h"

/* GLOBAL VARs */
uint8_t g_timer_1khz_flag = 0;
uint8_t g_timer_1sec_flag = 0;
uint16_t g_adc_result = 0;
uint8_t g_short_ECG_flag = 0;
uint8_t g_long_ECG_flag = 0;
uint8_t flag = 0;

STATE_MACHINE_e g_sys_state = SYS_INIT;
/* END GLOBAL VARs */

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);

/* Function definitions */
void main(void) {
    
    Init_Watchdog();
    Init_GPIO();
    Init_CLK();
    Init_Timers();
    Init_UART();
    Init_ADC();
    EnableGlobalInterrupt();


//    GPIO_setOutputHighOnPin(GPIO_PORT_P6,
//    							GPIO_PIN6);
    while(1) {

        if(g_timer_1khz_flag)
        {
            g_timer_1khz_flag = 0;
//            flag++;
//
            Test_ADC();
//
//            if(flag == 10)
//            {
//                flag = 0;
//                UART_serialplot(g_adc_result, 0);
//            }
        }

    }
}

/*** Configure Watchdog Timer ***/
void Init_Watchdog(void)
{
    WDT_A_hold(WDT_A_BASE);
}

/*** Global Interrupt Enable ***/
void EnableGlobalInterrupt()
{
    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    // Enter interrupt
    __bis_SR_register(GIE);
    // For debugger
    __no_operation();
}
