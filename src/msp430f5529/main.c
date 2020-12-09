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
    Init_Timer();
    // TODO Timers
    Init_UART();
    Init_ADC();
    EnableGlobalInterrupt();
    /* Init MSP430 END */

	while(1) {
//	    Test_UART();
	    //Test_ADC();
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








