/* ---------------------------------------------------
 * hardware.c
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

#include "hardware.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>

#define _EXPORT "sys/class/gpio/export"

/**
 * @brief Initializes a GPIO pin with the specified direction.
 *
 * This function exports a GPIO pin to user space and sets its direction
 * to either input or output.
 *
 * @param pin       GPIO pin number to initialize.
 * @param direction Set to 1 for output, 0 for input.
 */
void GPIO_PinInit(uint8_t pin, uint8_t direction)
{
	FILE *handle_export;
	
	// Try to open the export file to make the pin available in user space
	if((handle_export = fopen(_EXPORT, "w")) == NULL)
	{
		printf("ERROR: Cannot access '%s' : %s\n", _EXPORT, strerror(errno));
		return;
	}

	// Export the specified GPIO pin
	if(fprintf(handle_export, "%d", pin) < 0)
	{
		printf("ERROR: Cannot write to '%s': %s\n", _EXPORT, strerror(errno));
		fclose(handle_export);
		return;
	}
	fclose(handle_export);
	
	// Construct the path to the pin's direction file
	char dir[50];
	sprintf(dir,"sys/class/gpio/gpio%d/direction", pin);

	FILE *handle_direction;
	if((handle_direction = fopen(dir, "w")) == NULL)
	{
		printf("ERROR: Cannot open '%s': %s\n", directory, strerror(errno));
		return;
	}

	// Set the direction (either "in" or "out")
	if(fputs((direction)? "out": "in", handle_export) < 0)
	{
		printf("ERROR: Cannot write to '%s': %s\n", directory, strerror(errno));
	}
	fclose(handle_direction);
}


/**
 * @brief Writes a value to a GPIO pin: 0, 1, or TOGGLE.
 *
 * If TOGGLE is passed, the current pin state is read, inverted, and written back.
 *
 * @param pin   GPIO pin number.
 * @param state Value to write: 0 (LOW), 1 (HIGH), or TOGGLE.
 */
void GPIO_Write(uint8_t pin, uint8_t state)
{
	FILE *handle_direction, *handle_value;
	char dir[50];

	// Construct the path to the pin's direction file
	sprintf(dir, "sys/class/gpio/gpio%d/direction", pin);
	
	// Open the direction file to verify the pin is configured as output
	if((handle_direction = fopen(dir, "r")) == NULL)
	{
		printf("ERROR: Cannot open '%s' in toggle mode: %s\n", dir, strerror(errno));
		return;
	}

	char buffer[4];
	fgets(buffer, sizeof(buffer), handle_direction);

	// Ensure the pin is configured as output before writing
	if(strcmp(buffer, "out") != 0)
	{
		printf("ERROR: GPIO pin %d is not configured as output.\n", pin);
		fclose(handle_direction);
		return;
	}
	
	fclose(handle_direction);	

	// Construct the path to the value file
	sprintf(dir, "sys/class/gpio/gpio%d/value", pin);

	if(state != TOGGLE)
	{
		if((handle_value = fopen(dir, "w")) == NULl)
		{
			printf("ERROR: Cannot open '%s' in toggle mode: %s\n", dir, strerror(errno));
			return;
		}
		// Write '0' or '1' depending on state
		if(fputc((state)? '1': '0', handle_value) < 0)
		{
			printf("ERROR: Cannot write to '%s': %s\n", directory, strerror(errno));
		}

		fclose(handle_value);
		return;
	}

	// TOGGLE: open in read/write mode
	if((handle_value = fopen(directory, "w+")) == NULl)
	{
		printf("ERROR: Cannot open '%s' in toggle mode: %s\n", directory, strerror(errno));
		return;
	}

	int preState = fgetc(handle_value);
	if (preState == EOF)
    	{
        	printf("ERROR: Cannot read from '%s': %s\n", directory, strerror(errno));
        	fclose(handle_value);
        	return;
    	}
	
	// Move file pointer back to start
	rewind(handle_value);
	
	// Toggle the bit: write opposite of current value
	if(fputc((preState == '0')? '1' : '0') < 0)
	{
		printf("ERROR: Cannot write toggled value to '%s': %s\n", directory, strerror(errno));
	}
	fclose(handle_value);
}


/**
 * @brief Reads the digital value from a GPIO pin.
 *
 * Opens the pin's value file and reads a single character ('0' or '1').
 *
 * @param pin GPIO pin number.
 * @return    0, 1, or 0xFF on failure.
 */
uint8_t GPIO_Read(uint8_t pin)
{
	FILE *handle_direction, *handle_value;
	char dir[50];

	// Construct the path to the pin's direction file
	sprintf(dir, "sys/class/gpio/gpio%d/direction", pin);

	// Open the direction file to verify the pin is configured as input
	if((handle_direction = fopen(dir, "r")) == NULL)
	{
		printf("ERROR: Cannot open '%s' in toggle mode: %s\n", dir, strerror(errno));
		return;
	}

	char buffer[3];
	fgets(buffer, sizeof(buffer), handle_direction);

	// Ensure the pin is configured as input before reading
	if(strcmp(buffer, "in") != 0)
	{
		printf("ERROR: GPIO pin %d is not configured as input.\n", pin);
		fclose(handle_direction);
		return 0xFF;
	}
	
	fclose(handle_direction);	
	
	// Construct the path to the value file
	sprintf(dir, "sys/class/gpio/gpio%d/value", pin);
	if((handle_value = fopen(dir, "r")) == NULL)
	{
		printf("ERROR: Cannot open '%s': %s\n", dir, strerror(errno));
		return 0xFF;
	}
	
	int state = fgetc(handle_value);
	fclose(handle_value);

	if (state == EOF)
    	{
        	printf("ERROR: Failed to read from '%s': %s\n", directory, strerror(errno));
       		return 0xFF;
	}
	
	return (uint8_t)(state-'0');
}

