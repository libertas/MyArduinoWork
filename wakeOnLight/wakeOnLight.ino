#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>

/* Use PORTC and PORTD as ports of leds
 * Use PORTB0 as input pin
 * Use PORTB1 as output pin
 */

void sleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
}

void play()
{
	unsigned char i;
	for (i = 0; i < 100; i++) {
		PORTB |= _BV(1);
		_delay_ms(3);
		PORTB ^= _BV(1);
		_delay_ms(3);
	}
	_delay_ms(100);
	for (i = 0; i < 100; i++) {
		PORTB |= _BV(1);
		_delay_ms(2);
		PORTB ^= _BV(1);
		_delay_ms(2);
	}
	_delay_ms(100);
	for (i = 0; i < 100; i++) {
		PORTB |= _BV(1);
		_delay_ms(2);
		PORTB ^= _BV(1);
		_delay_ms(2);
	}
	_delay_ms(100);
	for (i = 0; i < 100; i++) {
		PORTB |= _BV(1);
		_delay_ms(3);
		PORTB ^= _BV(1);
		_delay_ms(3);
	}
}

int main()
{
    wdt_enable(WDTO_8S);
	DDRB = 0xfe;
	while (1) {
        wdt_reset();
        if (!(PINB & 0x01))
            play();
        else
            sleep();
	}
	return 0;
}
