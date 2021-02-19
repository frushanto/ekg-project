/*
 * gpio_cfg.c
 *
 *  Created on: 8 Nov 2020
 *      Author: ivan
 */

#include <includes/config/gpio_cfg.h>

uint8_t g_sd_state_flag = 2;
uint8_t g_bt_state_flag = 2;
uint8_t test = 4;
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

    // Configure Bluetooth State Pin
//    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
    test = (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1));

    // Configure Card Detect for SD Card
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P2,
            GPIO_PIN0)) {
        g_sd_card_inserted = TRUE;
        GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0, GPIO_HIGH_TO_LOW_TRANSITION);
    } else {
        g_sd_card_inserted = FALSE;
        GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0, GPIO_LOW_TO_HIGH_TRANSITION);
    }
}

void Buzzer_active(void)    // Use only for 5V DCDC ON/OFF DONT USE FOR AKKU
{
    while (g_buzzer_on_flag && g_buzzer_cnt)
    { 
        // __delay_cycles(2000000);
        if (g_timer_250Hz_Buzzer)
        {
            g_timer_250Hz_Buzzer = 0;
            GPIO_toggleOutputOnPin(GPIO_PORT_P6, GPIO_PIN1);
        }
        if (g_buzzer_cnt == 50)
        {
            g_timer_250Hz_Buzzer = 0;
            __delay_cycles(2000000);
        }
        if (g_buzzer_cnt == 100)
        {
            g_timer_250Hz_Buzzer = 0;
            __delay_cycles(2000000);
        }
        if (g_buzzer_cnt == 150) // 250 ca. 1sec
        {
            g_buzzer_on_flag = 0;
            g_timer_250Hz_Buzzer = 0;
            g_buzzer_cnt = 0;
            GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN1);
        }
    }  
}

void State_sys_Energy_Saving_Mode(void)
{
    g_buzzer_cnt = 1;
    // Buzzer turn ON
    Buzzer_active();
    // LED2 on PCB turn OFF
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
    // 5V DC/DC turn OFF
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6); 
    // Check if SD was inserted while turning off
    if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0)) {
        g_sd_state_flag = 1;
    } else {
        g_sd_state_flag = 0;
    }
}

void State_sys_Wakeup_Mode(void)
{
    // LED2 on PCB turn ON
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
    // 5V DC/DC turn ON
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);
    // Init SD Card
    // Init_FAT();
    Check_SD_Card_Connection();
    // Buzzer turn ON
    g_buzzer_on_flag = 1;
    g_buzzer_cnt = 1;
    Buzzer_active();
    // Change Sys_State
    g_sys_state = IDLE_STATE;
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
        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN0)) //TODO: When Button pressed, do nothing
        {
            if (g_5v_flag == 0)
            {
                g_5v_flag = 1;
                g_buzzer_on_flag = 1;
                g_sys_state = ENERGY_SAVING_MODE;
                // State_sys_Energy_Saving_Mode();
            }
            else if (g_5v_flag == 1)
            {
                g_5v_flag = 0;
                // g_buzzer_on_flag = 1;
                g_sys_state = SYS_WAKEUP;
                // State_sys_Wakeup_Mode();
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

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    switch (__even_in_range(P2IV, 0x10))
    {
    case 0x00:
        break;   // None
    case 0x02:   // Pin 0   // SD CARD
//        __delay_cycles(2000000);
        if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0)) {
            g_sd_state_flag = 1;
        } else {
            g_sd_state_flag = 0;
        }

        break;
    case 0x04:              // BLUETOOTH
//        __delay_cycles(2000000);
        if (test = GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1)) {
            g_bt_state_flag = 1;
        } else {
            g_bt_state_flag = 0;
        }
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
