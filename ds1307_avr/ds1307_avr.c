// Running on an Atmega 48V at 1 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

static int usart_putchar(char data, FILE *stream)
{
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);

void initUSART()
{
	UCSR0A |= _BV(U2X0);
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	UBRR0 = 12;  // 9600 at 1 MHz
	stdout = &mystdout;
}

ISR(USART_RX_vect)
{
    unsigned char tmp;
    tmp = UDR0;
    UDR0 = tmp;
}

int main()
{
	initUSART();

	sei();

	while(1)
	{
		printf("Hello,World\n");
	}

	return 0;
}

