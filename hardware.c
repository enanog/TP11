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

#define _EXPORT "sys/class/gpio/export"

void GPIO_PinInit(uint8_t pin, uint8_t direction)
{
	FILE *handle_export;
	if((handle_export = fopen(_EXPORT, "w")) == NULL)
	{
		printf("ERRROR: Cannot access '%s' : %s", _EXPORT, strerror(errno));
		return;
	}
	if(fprintf(handle_export, "%d", pin) < 0)
	{
		printf("ERROR WRITE EXPORT\n");
		fclose(handle_export);
		return;
	}
	fclose(handle_export);

	char directory[50];
	sprintf(directory,"sys/class/gpio/gpio%d/direction", pin);

	FILE *handle_direction;
	if((handle_direction = fopen(directory, "w")) == NULL)
	{
		printf("ERROR ACCESS TO DIRECTION\n");
		return;
	}
	if(fputs((direction)? "out": "in", handle_export) < 0)
	{
		printf("ERROR WRITE DIRECTION\n");
		fclose(handle_direction);
		return;
	}
	fclose(handle_direction);
}

void GPIO_Write(uint8_t pin, uint8_t state)
{
	FILE *handle_value;
	char directory[50];
	sprintf(directory, "sys/class/gpio/gpio%d/value", pin);
	if(state != TOGGLE)
	{
		if((handle_value = fopen(directory, "w")) == NULl)
		{
			return;
		}
		
		if(fputc((state)? '1': '0', handle_value) < 0)
		{
			fclose(handle_value);
			return;
		}
		fclose(handle_value);
		return;
	}
	if((handle_value = fopen(directory, "w+")) == NULl)
	{
		return;
	}
	char preState = fgetc(handle_value);
	if(fputc((preState == '0')? '1' : '0') < 0)
	{
		fclose(handle_value);
		return;
	}
	fclose(handle_value);
}

uint8_t GPIO_Read(uint8_t pin)
{
	FILE *handle_value;
	char directory[50];
	sprintf(directory, "sys/class/gpio/gpio%d/value", pin);
	if((handle_value = fopen(directory, "r")) == NULL)
	{
		return 0xFF;
	}
	
	uint8_t state = fgetc(handle_value) - '0';
	fclose(handle_value);
	return state;
}

