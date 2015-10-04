// Running on an Atmega 16a

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void sendUSART(unsigned char data)
{
    while(!(UCSRA & (1 << UDRE)));
    UDR = data;
}

unsigned char receiveUSART()
{
    while(!(UCSRA & (1 << RXC)));
    return UDR;
}

void initUSART()
{
    UBRRH = 0;
    UBRRL = 103; // 9600Hz on 16MHz F_CPU
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
    UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
}

ISR(USART_RXC_vect)
{
    unsigned char tmp;
    tmp = UDR;
    UDR = tmp;
}

int main()
{
    DDRA = 0xff;
    DDRC = 0xff;
    PORTA = 0x00;
    PORTC = 0x00;
    initUSART();
    sei();
    while(1)
    {
        PORTC = !(UCSRA & (1 << RXC));
    }
    return 0;
}

