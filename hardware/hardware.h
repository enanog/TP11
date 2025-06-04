/* ---------------------------------------------------
 * hardware.h
 * ---------------------------------------------------
 * GRUPO 1:
 * 	CASTRO, Tomás
 *	FRIGERIO, Dylan
 * 	VALENZUELA, Agustín
 * 	YAGGI, Lucca
 *
 * Profesores:
 * 	MAGLIIOLA, Nicolas
 * 	JACOBY, Daniel
 * 	VACATELLO, Pablo
 *
 * fecha: 21/05/2025
 * ---------------------------------------------------*/

#include <stdint.h>

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

// Pin direction modes.
enum
{
	INPUT = 0,
	OUTPUT,
};

//Output states for a pin or group of pins.
enum
{
	LOW = 0,
	HIGH,
	TOGGLE,
};

/**
 * @brief Initializes the mode (input or output) of a specific pin on a given port.
 *
 * This function validates the port and pin, and then sets the corresponding bit
 * in the direction register to configure the pin's mode.
 *
 * @param port   Port to which the pin belongs (PORTA, PORTB, PORTD)
 * @param pin    Pin number within the port (0–7 for PORTA && PORTB, 0-15 for PORTD).
 * @param state  Desired mode for the pin: INPUT (0) or OUTPUT (1).
 */
void GPIO_PinInit(uint8_t pin, uint8_t state);


///**
// * @brief Initializes the mode (input or output) of multiple pins on a given port using a bitmask.
// *
// * This function allows configuring a group of pins simultaneously. It shifts the mask
// * to the proper location based on the port and updates the direction register accordingly.
// *
// * @param port   Port to which the pins belong (PORTA, PORTB, PORTD).
// * @param mask   Bitmask representing the pins to initialize.
// * @param state  Desired mode: INPUT (0) or OUTPUT (1).
// */
//void GPIO_MaskInit(uint8_t port, uint16_t mask, uint8_t state);
//
//
/**
 * @brief Sets the output state of a specific pin on a given port.
 *
 * This function writes a logic value (HIGH/LOW) to the selected pin or toggles it
 * if TOGGLE is selected. Only works if the pin is configured as an output.
 *
 * @param port   Port to which the pin belongs (PORTA, PORTB, PORTD).
 * @param pin    Pin number within the port (0–7 for PORTA and PORTB, 0–15 for PORTD).
 * @param state  Output state: LOW (0), HIGH (1), or TOGGLE (2).
 */
void GPIO_Write(uint8_t pin, uint8_t state);

#endif /* _HARDWARE_H_ */
