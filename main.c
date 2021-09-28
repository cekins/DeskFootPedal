/* Name: main.c
 * Project: 4-Key-Keyboard
 * Author: Flip van den Berg - www.flipwork.nl
 * Creation Date: February 2010
 * Based on V-USB drivers from Objective Developments - http://www.obdev.at/products/vusb/index.html
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

ISR(INT0_vect) {
	PINB |= 1<<PB0;
}

void init_external_interrupt() {
	// Enable INT0 interrupt
	GIMSK |= (1<<INT0); 
	
	// Trigger interrupt on falling edge of INT0
	MCUCR |= 1<<ISC01;
	MCUCR &= ~(1<<ISC00);
}

void init_timer0_interrupt() {
	// Normal port operation
	TCCR0A &= ~(1<<COM0A1) & ~(1<<COM0A0);

	// CTC mode
	TCCR0A &= ~(1<<WGM00);
	TCCR0A |= (1<<WGM01);
	TCCR0B &= ~(1<<WGM02);

	// Set clock divider
	TCCR0B |= (1<<CS00) | (1<<CS02);
	TCCR0B &= ~(1<<CS01);

	// Set output compare value
	OCR0A = 0xFF;
	
	// Enable interrupt for OCR0A
	TIMSK |= 1<<OCIE0A;
}

void init_pins() {

	//Set PB0, PB1, PB2, PB4 to output mode
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB3) | (1<<PB4);

	//Set PB2 to input mode with no pullup resistor
	DDRB &= ~(1<<PB2);
	PORTB &= ~(1<<PB2);
}


int main(void)
{
	init_pins();
	init_external_interrupt();

	//Enable all interrupts
	sei();

	while(1);;

   	return 0;
}
