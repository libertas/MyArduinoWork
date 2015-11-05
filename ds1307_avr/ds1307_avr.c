// Running on an Atmega 48V at 1 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdio.h>

#define START 0x08
#define MT_SLA_ACK 0x18
#define MT_DATA_ACK 0x28

#define DS1307_R 0xd1
#define DS1307_W 0xd0

static int usart_putchar(char data, FILE *stream)
{
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);

void initUSART()
{
	UCSR0A |= _BV(U2X0);
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	UBRR0 = 12;  // 9600 at 1 MHz
	stdout = &mystdout;
}

ISR(USART_RX_vect)
{
    unsigned char tmp;
    tmp = UDR0;
    UDR0 = tmp;
}

uint8_t TWI_Start()
{
	TWBR = 0x00;

	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

	while(!(TWCR & _BV(TWINT)));

	return TWSR;

}

uint8_t TWI_Write(uint8_t data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN);

	while(!(TWCR & _BV(TWINT)));

	return TWSR;
}

uint8_t TWI_Stop()
{
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
	return TWSR;
}

void setupDS1307()
{
	if((TWI_Start() & 0xf8) != START)
	{
		printf("Start Error!\n");
		return;
	}

	if((TWI_Write(DS1307_W) & 0xf8) != MT_SLA_ACK)
	{
		printf("Slave Addr Error!\n");
		return;
	}

	if((TWI_Write(0x00) & 0xf8) != MT_DATA_ACK)
	{
		printf("Data Error!\n");
		return;
	}

	TWI_Stop();

	printf("DS1307 Setup!\n");
}

int main()
{
	initUSART();

	sei();

	setupDS1307();

	while(1)
	{
	}

	return 0;
}

