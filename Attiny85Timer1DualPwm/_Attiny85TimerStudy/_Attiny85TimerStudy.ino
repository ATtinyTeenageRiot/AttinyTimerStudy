/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

//Timer 1 as pwm

//ref http://www.re-innovation.co.uk/web12/index.php/en/blog-75/305-fast-pwm-on-attiny85
//ref http://matt16060936.blogspot.com/2012/04/attiny-pwm.html

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/iotn85.h>


//volatile uint16_t count;
//volatile uint16_t count2;

unsigned int interval = 0;


int main(void)
{
    uint8_t pwm_fade = 0;
    //count = 0;
    //count2 = 0;

    // Enable PLL and async PCK for high-speed PWM
    PLLCSR |= (1 << PLLE) | (1 << PCKE);

    //set ctc mode
    TCCR1 |= _BV(CTC1); //pwm not affected if you disable this

    //http://www.technoblogy.com/show?LE0
    //no prescale
    TCCR1 |= _BV(CS10);

    //PWM1A
    TCCR1 |= _BV(PWM1A);

    //bug fix
    //http://electronics.stackexchange.com/questions/97596/attiny85-pwm-why-does-com1a0-need-to-be-set-before-pwm-b-will-work
    TCCR1 |= 3<<COM1A0;

    //prescale 1024
    //TCCR1 |= _BV(CS13) | _BV(CS11) | _BV(CS10);
    //TCCR1 &=  ~_BV(CS12);

    //OCR1A –Timer/Counter1 Output Compare RegisterA
    //OCR1A = 250;

    //OCR1B –Timer/Counter1 Output Compare RegisterB
    //OCR1B = 250;

    //OCIE1A: Timer/Counter1 Output Compare Interrupt Enable
    //TIMSK |= _BV(OCIE1A);
    //OCR1A = 255;

    //OCIE1B: Timer/Counter1 Output Compare Interrupt Enable
    //TIMSK |= _BV(OCIE1B);

    //2<<COM1B0: sets bit COM1B1 and leaves COM1B0 clear, which (when in PWM mode) clears OC1B on compare-match, and sets at BOTTOM
    //PWM1B connected to port PB4[OC1B]

    GTCCR = 1<<PWM1B | 1<<COM1B0; //COM1B0 = important
    GTCCR |= _BV(FOC1B);

    //led output

    DDRB |= 1<<DDB4; //set PB4 as output
    PORTB &= ~(1 << PB4); //set PB4 output 0

    DDRB |= 1<<DDB1; //set PB1 as output
    PORTB &= ~(1 << PB1); //set PB1 output 0

    sei(); //enable global interrupt

    /* endless loop */
    for(;;)
        {

            for (pwm_fade =0; pwm_fade<255; ++pwm_fade)
                {
                    OCR1B = pwm_fade;
                    OCR1A = 255-pwm_fade;
                    _delay_ms(2);
                }

            _delay_ms(10);

            for (pwm_fade =255; pwm_fade>0; --pwm_fade)
                {
                    OCR1B = pwm_fade;
                    OCR1A = 255-pwm_fade;

                    _delay_ms(2);
                }

            _delay_ms(100);

        }

    return 0;   /* never reached */
}


