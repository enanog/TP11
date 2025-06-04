#include <stdio.h>
#include <time.h>

#define LED 24
int main(void)
{
	GPIO_PinInit(LED, OUTPUT);
	GPIO_Write(LED, LOW);
	for(int i= 0; i < 100000; i++);
	GPIO_Write(LED, HIGH);

	return 0;
}
