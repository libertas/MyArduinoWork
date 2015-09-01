#include <avr/io.h>
#include <util/delay.h>

/* Use PORTC and PORTD as ports of leds
 * Use PORTB0 as input pin
 * Use PORTB1 as output pin
 */

void blink()
{
    if(!(PINB & 0x01))
    {
        // Blink the leds
        PORTC = ~PORTC;
        PORTD = ~PORTD;
        //_delay_ms(1000);
    }
    else
    {
        // Turn off the leds
        PORTC = 0xff;
        PORTD = 0xff;
    }
}

void play()
{
    unsigned char i;
    if(!(PINB & 0x01))
    {
        for(i=0; i<100; i++)
        {
            PORTB |= _BV(1);
            _delay_ms(3);
            PORTB ^= _BV(1);
            _delay_ms(3);
        }
        _delay_ms(100);
        for(i=0; i<100; i++)
        {
            PORTB |= _BV(1);
            _delay_ms(2);
            PORTB ^= _BV(1);
            _delay_ms(2);
        }
        _delay_ms(100);
        for(i=0; i<100; i++)
        {
            PORTB |= _BV(1);
            _delay_ms(2);
            PORTB ^= _BV(1);
            _delay_ms(2);
        }
        _delay_ms(100);
        for(i=0; i<100; i++)
        {
            PORTB |= _BV(1);
            _delay_ms(3);
            PORTB ^= _BV(1);
            _delay_ms(3);
        }
    }
}

int main()
{
    DDRC = 0xff;
    PORTC = 0xff;
    DDRD = 0xff;
    PORTD = 0xff;
    DDRB = 0xfe;
    while(1)
    {
        blink();
        play();
    }
    return 0;
}

