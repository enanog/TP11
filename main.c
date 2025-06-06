#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "hardware.h"

#define D0 17
#define D1 4
#define D2 18
#define D3 23
#define D4 24
#define D5 25
#define D6 22
#define D7 27

enum
{
	STOP = 0,
	RUNNING,
};

enum
{
	TOGGLE_LEDS = 8,
	OFF_ALL,
	ON_ALL,
	FINISH,
};

void menu(void);
uint8_t readMenuOption(void);

int main(void)
{
	uint8_t pinList[] = {D0, D1, D2, D3, D4, D5, D6, D7};
	for(uint8_t i = 0; i < 8; i++)
	{
		GPIO_PinInit(pinList[i], OUTPUT);	
	}
	for(uint8_t i = 0; i < 8; i++)
	{
		GPIO_Write(pinList[i], LOW);
	}
	
	uint8_t prog = RUNNING;
	
	while(prog)
	{
		menu();
		printf("\n\n\t\t\t");
		uint8_t aux = readMenuOption();
		if(aux <  8)
		{
			GPIO_Write(pinList[aux], HIGH);	
			continue;
		}
		switch(aux)
		{
			case TOGGLE_LEDS:
				for(uint8_t i = 0; i < 8; i++)
				{
					GPIO_Write(pinList[i], TOGGLE);
				}
				break;
		
			case OFF_ALL:
				for(uint8_t i = 0; i < 8; i++)
				{
					GPIO_Write(pinList[i], LOW);
				}
				break;
				
			case ON_ALL:
				for(uint8_t i = 0; i < 8; i++)
				{
					GPIO_Write(pinList[i], HIGH);
				}
				break;
				
			case FINISH:
				prog = STOP;
				break;
				
			default:
				printf("\t\t\tERROR: Invalid command. Try again.\n");
				break;
		}
		printf("\n\n\n\n\n\n");
	}
	for(uint8_t i = 0; i < 8; i++)
	{
		GPIO_Unexport(i);
	}

	return 0;
}

void menu(void)
{
	printf("\t\t\t=========== LED SIMULATOR MENU ===========\n");
    printf("\t\t\t [0-7]  Turn ON a specific LED\n");
    printf("\t\t\t [t]    Toggle all LEDs\n");
    printf("\t\t\t [c]    Clear (turn off) all LEDs\n");
    printf("\t\t\t [s]    Set (turn on) all LEDs\n");
    printf("\t\t\t [q]    Quit program\n");
    printf("\t\t\t==========================================\n");
}

uint8_t readMenuOption(void)
{
	char ch = getchar();
	while(getchar() != '\n');
	switch(tolower(ch))
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			return ch - '0';
		case 't':
			return TOGGLE_LEDS;
		case 'c':
			return OFF_ALL;
		case 's':
			return ON_ALL;
		case 'q':
			return FINISH;
		default:
			return 0xFF;
	}
}

