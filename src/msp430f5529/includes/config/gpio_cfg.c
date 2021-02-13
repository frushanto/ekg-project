/*
 * gpio_cfg.c
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#include <includes/config/gpio_cfg.h>

// Configure GPIO ports/pins
void Init_GPIO(void)
{
    // Configure LED1 on PCB
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);

    // Configure buzzer
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN1);

    // Configure LED2 on PCB
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);

    // Configure 5V DC/DC Enaeble
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);

    // Configure button on PCB
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN0);

    // Configure Pin attached to button DONT USE
    GPIO_setAsInputPin(GPIO_PORT_P6, GPIO_PIN2);
    // GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN2);
}

void Buzzer_active(void)
{

}

/* Interrupt Service Routines */
#pragma vector = PORT1_VECTOR
__interrupt void pushbutton_ISR(void)
{
    switch (__even_in_range(P1IV, 0x10))
    {
    case 0x00:
        break;   // None
    case 0x02:          // Pin 0

        __delay_cycles(4000000);   // 20000 = 1ms
        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN0))
        {
            GPIO_toggleOutputOnPin(GPIO_PORT_P6, GPIO_PIN6);
            GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN4);

            g_5v_flag++;
            if (g_5v_flag == 1)
            {
                // energy saving
                g_sys_state = ENERGY_SAVING_MODE;
            }
            else
            {
                // init UART and FAT new
                g_sys_state = SYS_DIRTY_START;  // create new case
            }
        }

        break;
    case 0x04:
        break;   // Pin 1
    case 0x06:
        break;   // Pin 2
    case 0x08:
        break;   // Pin 3
    case 0x0A:
        break;   // Pin 4
    case 0x0C:
        break;   // Pin 5
    case 0x0E:
        break;   // Pin 6
    case 0x10:
        break;   // Pin 7
    default:
        _never_executed();
    }
}
