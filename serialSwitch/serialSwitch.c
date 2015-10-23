// Running on an Atmega 16a

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define EEPROM_SIZE 512
#define MAXCMDLEN 100

uint16_t timerMinutes = 0;
uint16_t stackP = EEPROM_SIZE - 1;

void writeEEPROM(unsigned int addr, unsigned char data)
{
    while(EECR & (1 << EEWE));
    EEAR = addr;
    EEDR = data;
    EECR |= 1 << EEMWE;
    EECR |= 1 << EEWE;
}

unsigned char readEEPROM(unsigned int addr)
{
    while(EECR & (1 << EEWE));
    EEAR = addr;
    EECR |= 1 << EERE;
    return EEDR;
}

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

void print(char *s)
{
    while(*s)
    {
        sendUSART(*s);
        s++;
    }
}

void initUSART()
{
    UBRRH = 0;
    UBRRL = 103; // 9600Hz on 16MHz F_CPU
    UCSRB = (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << URSEL) | (3 << UCSZ0);
}

ISR(TIMER0_OVF_vect)
{
    static uint16_t t;
    static uint16_t seconds;
    t++;
    if(t > (16000000UL / 1024))
    {
        t = 0;
        seconds++;
    }
    if(seconds > 59)
    {
        seconds = 0;
        timerMinutes++;
    }
}

void initTimer0()
{
    TCCR0 = (1 << CS02) | (1 << CS00);  // CLKio / 1024
    TCNT0 = 0;
    TIMSK = _BV(TOIE0);
    sei();
}

void runCmd(char code[])
{
    uint16_t i;
    uint8_t addr;
    uint16_t time;
    addr = code[1] - '0';
    switch(code[0])
    {
        case 'A':  // control port a
            if(code[2] - '0')
            {
                writeEEPROM(addr, '1');
                PORTA |= 1 << addr;
            }
            else
            {
                writeEEPROM(addr, '0');
                PORTA &= ~(1 << addr);
            }
            break;
        case 'C':  // control port c
            if(code[2] - '0')
            {
                writeEEPROM(8 + addr, '1');
                PORTC |= 1 << addr;
            }
            else
            {
                writeEEPROM(8 + addr, '0');
                PORTC &= ~(1 << addr);
            }
            break;
        case 'E':  // set eeprom data
            if(code[2] == 'i')
                for(i = 0; i < 16; i++)
                    writeEEPROM(i, code[1]);
            else if(code[2] == 'f')
                for(i = 0; i < 16; i++)
                    writeEEPROM(i, 0xff);
            else if(code[2] == 'r')
                PORTA = ~readEEPROM(code[1]);
            else
                writeEEPROM(code[1], code[2]);
                break;
        case 'T':
            if(sscanf(code, "T%d%s", &time, code) == 2)
            {
                writeEEPROM(stackP--, (uint8_t)(time >> 8));
                writeEEPROM(stackP--, (uint8_t)time);
                for(i = 0; i < strlen(code); i++)
                {
                    writeEEPROM(stackP--, code[i]);
                }
                writeEEPROM(stackP--, '\n');
            }
            else
            {
                print("Wrong code!\n");
            }
            break;
        default:
            break;
    }
}

int main()
{
    char codeUSART[MAXCMDLEN];
    unsigned char i, j;

    DDRA = 0xff;
    DDRC = 0xff;
    PORTA = 0x00;
    PORTC = 0x00;

    for(i = 0; i < 8; i++)
    {
        PORTA = PORTA >> 1;
        PORTA |= ((readEEPROM(i) - '0') & 0x01) << 7;
    }

    for(i = 8; i < 16; i++)
    {
        PORTC = PORTC >> 1;
        PORTC |= ((readEEPROM(i) - '0') & 0x01) << 7;
    }

    initUSART();
    initTimer0();

    while(1)
    {
        for(i = 0; i < MAXCMDLEN; i++)
        {
            codeUSART[i] = receiveUSART();
            if(codeUSART[i] == 19 || codeUSART[i] == '\n')
            {
                for(j = 0; j < 3; j++)
                    codeUSART[i] = 0x00;
                break;
            }
        }

        if(codeUSART[0])
        {
            print("\nReceived:\n");
            for(i = 0; i < 3; i++)
                sendUSART(codeUSART[i]);
            print("\n");
        }

        runCmd(codeUSART);
    }
    return 0;
}

