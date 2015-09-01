#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER2_OVF_vect)
{
    static unsigned t;
    t++;
    if(t == F_CPU/256/1024)
    {
        PORTD = ~PORTD;
        t = 0;
    }
}

void setupTimer()
{
    TCCR2 = 0x07;
    TCNT2 = 0;
    TIMSK = _BV(TOIE2);
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
