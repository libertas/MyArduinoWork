#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER2_OVF_vect)
{
    static unsigned char t;
    t++;
    if(t == 128)
    {
        t = 0;
        PORTD = ~PORTD;
    }
}

void setupTimer()
{
    // Atmega 48
    ASSR |= _BV(AS2);
    TCCR2B = _BV(CS20);
    TCNT2 = 0;
    TIMSK2 = _BV(TOIE2);
    sei();
}

int main()
{
    DDRD = 0xff;

    setupTimer();

    while(1)
    {
        
    }
    return 0;
}

