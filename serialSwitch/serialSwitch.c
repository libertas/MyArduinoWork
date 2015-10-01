// Running on an Atmega 16a

#include <avr/io.h>
#include <util/delay.h>

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
    UCSRB = (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
}

int main()
{
    unsigned char tmp;
    DDRB = 0xff;
    initUSART(9600);
    DDRA = 0xff;
    while(1)
    {
        PORTB = ~PORTB;
        tmp = receiveUSART();
        if(tmp != 19)
        {
            PORTA = ~tmp;
            sendUSART(tmp);
        }
    }
    return 0;
}

