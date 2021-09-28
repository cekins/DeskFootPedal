/* Name: main.c
 * Project: 4-Key-Keyboard
 * Author: Flip van den Berg - www.flipwork.nl
 * Creation Date: February 2010
 * Based on V-USB drivers from Objective Developments - http://www.obdev.at/products/vusb/index.html
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include <stdint.h>


#define PRESS_WINDOW 10

uint8_t time_count = 0;
uint8_t press_count = 0;

void press_button(uint8_t button) {
	int pin;
	switch (button) {
		case 1:
			pin = PB0;
			break;
		case 2:
			pin = PB1;
			break;
		case 3:
			pin = PB3;
			break;
		case 4:
			pin = PB4;
			break;
		default:
			return;
	}
	
	PINB |= 1<<pin;
}
	
ISR(TIMER0_COMPA_vect) {

	if (time_count > PRESS_WINDOW)
		return;
	if (++time_count == PRESS_WINDOW && press_count > 0 && press_count <= 4) {
		press_button(press_count);
		press_count = 0;
	}
}

ISR(INT0_vect) {
	++press_count;
	time_count = 0;
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

	// Set output compare value to trigger every ~0.1 seconds
	OCR0A = 98;
	
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
	init_timer0_interrupt();
	init_external_interrupt();

	//Enable all interrupts
	sei();

	while(1);;

   	return 0;
}
