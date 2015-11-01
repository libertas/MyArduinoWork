// Running on an Atmega 48V at 1 MHz

#include <avr/io.h>
#include <avr/interrupt.h>

void initUSART()
{
	UCSR0A |= _BV(U2X0);
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	UBRR0 = 12;  // 9600 at 1 MHz
}

ISR(USART_RX_vect)
{
    unsigned char tmp;
    tmp = UDR0;
    UDR0 = tmp;
}

void sendUSART(unsigned char data)
{
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

char receiveUSART()
{
    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

int main()
{
	initUSART();

	sei();

	while(1)
	{
	}

	return 0;
}
