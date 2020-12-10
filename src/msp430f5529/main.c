#include "main.h"

/* Function declarations */
void Init_Watchdog(void);
void EnableGlobalInterrupt(void);

/* Function definitions */
void main(void){
    /* Init MSP430 BEGIN */
    Init_Watchdog();
    Init_GPIO();
    Init_CLK();
//  Init_Timer();
    Init_UART();
    Init_ADC();
    EnableGlobalInterrupt();
    /* Init MSP430 END */

    /* IIR Test */
    iir_filter_init();

    /* FIR Test */
    fir_filter_init();

    /* Filter Test VARs */
    uint16_t filter_val = 50;
    uint16_t adc_after_iir_test;
    uint16_t adc_after_fir_test;
    int i = 0;

	while(1) {
//	    Test_UART();
//	    Test_ADC();
//	    UART_Upper_T();
//	    UART_Lower_T();

//        for(i = 0; i <= 50; i++){
//            filter_val += 1;
//
//            // adc_after_iir_test = iir_filter(filter_val);
//            // Test_UART(adc_after_iir_test);
//
//            adc_after_fir_test = fir_filter(filter_val);
//            Test_UART(adc_after_fir_test);
//
//            if(filter_val == 100){
//                filter_val=50;
//            }
//            _delay_cycles(3000);
//        }

	    /* Test FIR Filter */
//	    uint16_t adc_after_fir_test = fir_filter(filter_val);
//	    Test_UART(adc_after_fir_test);

	    /* Test IIR Filter */
//	    uint16_t adc_after_iir_test = iir_filter(filter_val);
//	    Test_UART(filter_val);
	}
}

/*** Configure Watchdog Timer ***/
void Init_Watchdog(void) {
	WDT_A_hold(WDT_A_BASE);
}

/*** Global Interrupt Enable ***/
void EnableGlobalInterrupt() {
    // Enter interrupt
    __bis_SR_register(GIE);
    // For debugger
    __no_operation();
}








