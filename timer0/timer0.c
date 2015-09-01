#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect)
{
    PORTC = ~PORTC;
    PORTD = ~PORTD;
}

void setupTimer0()
{
    TCCR0 = 0x05;
    TCNT0 = 0;
    TIMSK = _BV(TOIE0);
    sei();
}

int main()
{
    DDRC = 0xff;
    DDRD = 0xff;

    setupTimer0();

    while(1)
    {
        
    }
    return 0;
}
