/* Name: main.c
 * Project: 4-Key-Keyboard
 * Author: Flip van den Berg - www.flipwork.nl
 * Creation Date: February 2010
 * Based on V-USB drivers from Objective Developments - http://www.obdev.at/products/vusb/index.html
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

#define DEBOUNCE_DELAY 1000L*50

ISR(INT0_vect) {
	PINB |= 1<<PB0;
}


int main(void)
{
	//Set Pin as Output
	DDRB |= 1<<PB0;

	//Set Pin as INput pullup
	DDRB &= ~(1<<PB2);
	PORTB &= ~(1<<PB2);


	//Set up external interrupt
	GIMSK |= (1<<INT0);
	
	// falling edge of INT0
	MCUCR |= 1<<ISC01;
	MCUCR &= ~(1<<ISC00);


	sei();
	

	while(1) {
		

	}

   	return 0;
}
