#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_OVF_vect)
{
    static unsigned int t;

    TCNT1H = (65536-F_CPU/1000)%256;
    TCNT1L = (65536-F_CPU/1000)%256;

    t++;
    if(t == F_CPU/65536)
    {
        t = 0;
        PORTC = ~PORTC;
        PORTD = ~PORTD;
    }
}

void setupTimer1()
{
    TCNT1H = (65536-F_CPU/1000)%256;
    TCNT1L = (65536-F_CPU/1000)%256;
    TCCR1A = 0;
    TCCR1B = 0x01;
    // Atmega 48V
    TIMSK1 = _BV(TOIE1);
    // Atmega 16a
    // TIMSK = _BV(TOIE1);
    sei();
}

int main()
{
    DDRC = 0xff;
    DDRD = 0xff;

    setupTimer1();

    while(1)
    {
        
    }
    return 0;
}
