// Running on an Atmega 16a
// 16 MHz

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
    uint16_t addr;
    uint16_t time, time1;
    char port, pin;
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
        case 'T':  // Timer
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
                return;
            }
            break;
        case 'U':  // Duty of each pin
            if(sscanf(code, "U%c%c%d/%d", &port, &pin, &time, &time1) == 4)
            {
                if(port == 'A')
                    addr = 16;
                else if(port == 'C')
                    addr = 48;
                else
                {
                    printf("Wrong port\n");
                    return;
                }
                if(pin - '0' < 10 && pin - '0' >= 0)
                    addr += (pin - '0') * 4;
                writeEEPROM(addr, time >> 8);
                writeEEPROM(addr + 1, time);
                writeEEPROM(addr + 2, time1 >> 8);
                writeEEPROM(addr + 3, time1);
            }
            break;
        default:
            break;
    }
}

ISR(TIMER0_OVF_vect)
{
    static uint16_t t;
    static uint8_t seconds;
    static uint16_t p;
    uint16_t time, i;
    char buf[100];

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

    time = ((uint16_t)readEEPROM(EEPROM_SIZE - 1 - p)) << 8 | readEEPROM(EEPROM_SIZE - 2 - p);

    if(time != 0xffff)
    {
        p -= 2;
        if(timerMinutes >= time)
        {
            for(i = 0; '\n' != (buf[i] = readEEPROM(p)); i++, p--);
            runCmd(buf);
            timerMinutes = 0;
        }
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

