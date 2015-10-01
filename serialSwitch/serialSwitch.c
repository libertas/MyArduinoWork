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
    unsigned char codeUSART[3];
    unsigned char i, j;
    DDRA = 0xff;
    DDRC = 0xff;
    initUSART(9600);
    while(1)
    {
        for(i = 0; i < 3; i++)
        {
            codeUSART[i] = receiveUSART();
            if(codeUSART[i] == 19)
            {
                for(j = 0; j < 3; j++)
                    codeUSART[i] = 0x00;
                break;
            }
        }
        switch(codeUSART[0])
        {
            case 'A':
                if(codeUSART[2] - '0')
                    PORTA |= 1 << (codeUSART[1] - '0');
                else
                    PORTA &= ~(1 << (codeUSART[1] - '0'));
                break;
            case 'C':
                if(codeUSART[2] - '0')
                    PORTC |= 1 << (codeUSART[1] - '0');
                else
                    PORTC &= ~(1 << (codeUSART[1] - '0'));
                break;
        }
    }
    return 0;
}

