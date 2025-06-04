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

#define _EXPORT "/sys/class/gpio/export"

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
	sprintf(dir,"/sys/class/gpio/gpio%d/direction", pin);

	FILE *handle_direction;
	if((handle_direction = fopen(dir, "w")) == NULL)
	{
		printf("ERROR: Cannot open '%s': %s\n", dir, strerror(errno));
		return;
	}

	// Set the direction (either "in" or "out")
	if(fputs((direction)? "out": "in", handle_export) < 0)
	{
		printf("ERROR: Cannot write to '%s': %s\n", dir, strerror(errno));
	}
	fclose(handle_direction);
}

void GPIO_Write(uint8_t pin, uint8_t state)
{
	FILE *handle_direction, *handle_value;
	char dir[50];

	// Construct the path to the pin's direction file
	sprintf(dir, "/sys/class/gpio/gpio%d/direction", pin);
	
	// Open the direction file to verify the pin is configured as output
	if((handle_direction = fopen(dir, "r")) == NULL)
	{
		printf("ERROR: Cannot open '%s': %s\n", dir, strerror(errno));
		return;
	}

	char buffer[4];
	if(fgets(buffer, sizeof(buffer), handle_direction) == NULL)
	{
		printf("ERROR: Failed to read from '%s': %s\n", dir, strerror(errno));
		fclose(handle_direction);
		return;
	}

	// Ensure the pin is configured as output before writing
	if(strcmp(buffer, "out") != 0)
	{
		printf("ERROR: GPIO pin %d is not configured as output.\n", pin);
		fclose(handle_direction);
		return;
	}
	
	fclose(handle_direction);	

	// Construct the path to the value file
	sprintf(dir, "/sys/class/gpio/gpio%d/value", pin);

	if(state != TOGGLE)
	{
		if((handle_value = fopen(dir, "w")) == NULL)
		{
			printf("ERROR: Cannot open '%s': %s\n", dir, strerror(errno));
			return;
		}
		// Write '0' or '1' depending on state
		if(fputc((state)? '1': '0', handle_value) < 0)
		{
			printf("ERROR: Cannot write to '%s': %s\n", dir, strerror(errno));
		}

		fclose(handle_value);
		return;
	}

	// Try to open the GPIO value file in read mode to read the current state
	if((handle_value = fopen(dir, "r")) == NULL)
	{
		printf("ERROR: Cannot open '%s': %s\n", dir, strerror(errno));
		return;
	}

	// Read the current value of the GPIO pin
	int preState = fgetc(handle_value);
	if (preState == EOF)
	{
    	printf("ERROR: Cannot read from '%s': %s\n", dir, strerror(errno));
    	fclose(handle_value);
    	return;
	}
	
	fclose(handle_value);
	
	// Open the GPIO value file again, this time in write mode, to update the pin state
	if((handle_value = fopen(dir, "w")) == NULL)
	{
		printf("ERROR: Cannot open '%s': %s\n", dir, strerror(errno));
		return;
	}
	
	// Toggle the bit: write opposite of current value
	if(fputc((preState == '0')? '1' : '0', handle_value) < 0)
	{
		printf("ERROR: Cannot write toggled value to '%s': %s\n", dir, strerror(errno));
	}
	fclose(handle_value);
}

uint8_t GPIO_Read(uint8_t pin)
{
	FILE *handle_direction, *handle_value;
	char dir[50];

	// Construct the path to the pin's direction file
	sprintf(dir, "/sys/class/gpio/gpio%d/direction", pin);

	// Open the direction file to verify the pin is configured as input
	if((handle_direction = fopen(dir, "r")) == NULL)
	{
		printf("ERROR: Cannot open '%s': %s\n", dir, strerror(errno));
		return 0xFF;
	}

	char buffer[3];
	if(fgets(buffer, sizeof(buffer), handle_direction) == NULL)
	{
		printf("ERROR: Failed to read from '%s': %s\n", dir, strerror(errno));
		fclose(handle_direction);
		return 0xFF;
	}

	// Ensure the pin is configured as input before reading
	if(strcmp(buffer, "in") != 0)
	{
		printf("ERROR: GPIO pin %d is not configured as input.\n", pin);
		fclose(handle_direction);
		return 0xFF;
	}
	
	fclose(handle_direction);	
	
	// Construct the path to the value file
	sprintf(dir, "/sys/class/gpio/gpio%d/value", pin);
	if((handle_value = fopen(dir, "r")) == NULL)
	{
		printf("ERROR: Cannot open '%s': %s\n", dir, strerror(errno));
		return 0xFF;
	}
	
	int state = fgetc(handle_value);
	fclose(handle_value);

	if (state == EOF)
    	{
        	printf("ERROR: Failed to read from '%s': %s\n", dir, strerror(errno));
       		return 0xFF;
	}
	
	return (uint8_t)(state-'0');
}

