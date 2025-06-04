/*
 * gpio.c
 *
 *  Created on: Jun 4, 2025
 *      Author: dyfrigerio
 */

#include "gpio.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define _EXPORT "/sys/class/gpio/export"

void GPIO_PinInit(uint8_t pin, uint8_t direction)
{
	FILE *handle_export;
	if((handle_export = fopen(_EXPORT, "w")) == NULL)
	{
		printf("ERROR EXPORT\n");
		return;
	}
	if(fprintf(handle_export, "%d", pin) < 0)
	{
		printf("");
		fclose(handle_export);
		return;
	}
	fclose(handle_export);

	char directory[50];
	sprintf(directory,"/sys/class/gpio/gpio%d/direction", pin);

	FILE *handle_direction;
	if((handle_direction = fopen(directory, "w")) == NULL)
	{
		printf("");
		return;
	}
	if(fputs((direction)? "out": "in", handle_export) < 0)
	{
		printf("");
		fclose(handle_direction);
		return;
	}
	fclose(handle_direction);
}

void GPIO_Write(uint8_t pin, uint8_t state)
{
	FILE *handle_value;
	char directory[50];
	sprintf(directory, "/sys/class/gpio/gpio%d/value", pin);
	if((handle_value = fopen(directory, handle_value)) == NULL)
	{
		return;
	}
	if(fputc((state)? '1': '0', handle_value) < 0)
	{
		fclose(handle_value);
		return;
	}
	fclose(handle_value);
}

