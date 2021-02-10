/*
 * std_types.h
 *
 *  Created on: 6 Nov 2020
 *      Author: ivan
 */

#ifndef INCLUDES_STD_TYPES_H_
#define INCLUDES_STD_TYPES_H_

#define TRUE    1
#define FALSE   0

#define DELAY500K  _delay_cycles(500000)

/**
 * enum for standard return type
 */
typedef enum {
    E_OK        = 0,    /*!< ok     */
    E_NOT_OK    = 1     /*!< not ok */
} STD_RETURN_TYPE_e;

/**
 * enum for UART ports
 */
typedef enum {
    E_UART_A0        = 0,    /*!< UART A0 */
    E_UART_A1        = 1     /*!< UART A1 */
} STD_UART_BASE_e;

#endif /* INCLUDES_STD_TYPES_H_ */
