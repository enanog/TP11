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


/**
 * @brief Writes a value to a GPIO pin: 0, 1, or TOGGLE.
 *
 * If TOGGLE is passed, the current pin state is read, inverted, and written back.
 *
 * @param pin   GPIO pin number.
 * @param state Value to write: 0 (LOW), 1 (HIGH), or TOGGLE.
 */
void GPIO_Write(uint8_t pin, uint8_t state);


/**
 * @brief Reads the digital value from a GPIO pin.
 *
 * Opens the pin's value file and reads a single character ('0' or '1').
 *
 * @param pin GPIO pin number.
 * @return    0, 1, or 0xFF on failure.
 */
uint8_t GPIO_Read(uint8_t pin);

#endif /* _HARDWARE_H_ */
