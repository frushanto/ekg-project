/*
 * clk_cfg.c
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#include <clk_cfg.h>

/*
 * Setup Clocking
 * ACLK = aux clock, slow & low-power;
 * REFO = 32768 Hz, trimmed to 3,5%;
 * */
void Init_CLK() {
    UCS_initClockSignal (
        UCS_ACLK,               // Configure ACLK
        UCS_REFOCLK_SELECT,     // Set to REFO source
        UCS_CLOCK_DIVIDER_1     // Set clock divider to 1
    );
}
