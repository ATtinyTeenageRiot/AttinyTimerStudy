/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile uint16_t count;
volatile uint16_t count2;

void disableTimer0()
{
    GTCCR |= _BV(TSM) | _BV(PSR0);
}

void enableTimer0()
{
    GTCCR &= ~_BV(TSM);
}

int main(void)
{
    count = 0;
    count2 = 0;

    //disable timer 0
    disableTimer0();

    TCCR0B |= _BV(CS00); // no prescale

    //TIMSK |= _BV(OCIE0A); //timer compare A
    //TIMSK |= _BV(OCIE0B); //timer compare B

    //set CTC mode 1
    TCCR0A &= ~_BV(WGM00);
    TCCR0A |= _BV(WGM01);
    TCCR0A &= ~_BV(WGM02);

    //timer0 compare A
    OCR0A = 100;

    DDRB |= 1<<PB1; //set PB1 as output
    PORTB &= ~(1 << PB1); //set PB1 output 1

    //enable timer 0
    enableTimer0();

    sei(); //enable global interrupt

    /* endless loop */
    for(;;){
        if(TIFR & _BV(OCF0A)) //same as TIMER0_COMPA_vect
        {
            count++;
            if (count >= 10000) {
                PORTB ^= (1 << PB1);
                count2++;
                count = 0;

            }

            TIFR = _BV(OCF0A); // will need this if no timer interrupt not activated
            //OCF0A is cleared by writing a logic one to the flag.
            //OCF0A is cleared by hardware when executing the corresponding interrupt handling vector
        }

    }

    return 0;   /* never reached */
}



ISR(TIMER0_COMPA_vect)
{

}
