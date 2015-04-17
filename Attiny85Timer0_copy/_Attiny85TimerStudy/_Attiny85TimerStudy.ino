/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define STEP_COUNT 8

const uint8_t NUMBER_OF_STEPS = STEP_COUNT - 1;
volatile uint8_t CURRENT_STEP_INDEX = 0;

uint8_t stepFreqs[]    = {  0,  0, 20,250,  0,  0, 20,250};
uint8_t stepSustains[] = {100,100,100,100,100,100,100,100};

volatile uint16_t timer0_tempo_interval = 0;
volatile uint16_t timer0_sustain_interval = 0;
volatile uint16_t timer0_duration_millis = 0;

uint8_t first_start = 1;


uint8_t pwm_fade = 0;

void disableTimer0()
{
    //Writing the TSM bit to one activates the Timer/Counter Synchronization Mode. In this mode,
    //the value written to PSR0 is kept, hence keeping the Prescaler Reset signal asserted
    GTCCR |= _BV(TSM) | _BV(PSR0);
}

void enableTimer0()
{
    //When the TSM bit is written to zero, the PSR0 bit is cleared by hardware, and the timer/counter start counting.
    GTCCR &= ~_BV(TSM);
}

void setupTimer0()
{

    //prescale 1024
    TCCR0B |=   (1<<CS00);
    TCCR0B &=   ~(1<<CS01);
    TCCR0B |=   (1<<CS02);

    //TCCR0B |= _BV(CS00); // no prescale

    TIMSK |= _BV(OCIE0A); //timer compare A interrupt enable
    //TIMSK |= _BV(OCIE0B); //timer compare B interrupt enable

    //set CTC mode 1
    TCCR0A &= ~_BV(WGM00);
    TCCR0A |= _BV(WGM01);
    TCCR0A &= ~_BV(WGM02);

    //timer0 compare A
    OCR0A = 255;
    //timer0 compare B
    //OCR0B = 255;

}

void setupOutputPin()
{
    DDRB |= 1<<DDB4; //set PB4 as output
    PORTB &= ~(1 << PB4); //set PB4 output 0

    DDRB |= 1<<DDB1; //set PB1 as output
    PORTB &= ~(1 << PB1); //set PB1 output 0

}

void setupTimer1PWM()
{
    //set ctc mode
    TCCR1 |= _BV(CTC1); //pwm not affected if you disable this
    //http://www.technoblogy.com/show?LE0
    //no prescale
    TCCR1 |= _BV(CS10);
    //PWM1A
    TCCR1 |= _BV(PWM1A);

    //bug fix
    //http://electronics.stackexchange.com/questions/97596/attiny85-pwm-why-does-com1a0-need-to-be-set-before-pwm-b-will-work
    TCCR1 |= 3<<COM1A0; //this will disable PB1

    //2<<COM1B0: sets bit COM1B1 and leaves COM1B0 clear, which (when in PWM mode) clears OC1B on compare-match, and sets at BOTTOM
    //PWM1B connected to port PB4[OC1B]
    //GTCCR = 1<<PWM1B | 1<<COM1B0; //COM1B0 = important
}

int main(void)
{
    //disable timer 0
    disableTimer0();
    //setup timer 0
    setupTimer0();
    //set PB1 output 1
    setupOutputPin();
    //enable timer 0
    enableTimer0();
    //timer 1 pwm
    setupTimer1PWM();

    sei(); //enable global interrupt

    /* endless loop */
    for(;;)
        {

//        for (pwm_fade =0; pwm_fade<255; ++pwm_fade)
//            {
//                OCR1A = 255 - pwm_fade;
//                _delay_ms(2);
//            }
//        _delay_ms(10);
//        for (pwm_fade =255; pwm_fade>0; --pwm_fade)
//            {
//                OCR1A = 255 - pwm_fade;
//                _delay_ms(2);
//            }
//        _delay_ms(100);
//        PORTB ^= (1 << PB4);


//            if(TIFR & _BV(OCF0A)) //TIMER0_COMPA_vect
//                {
//                    timer0_tempo_interval++;

//                    if(CURRENT_STEP_INDEX == 0)
//                        {
//                            OCR1A = 255 - stepFreqs[CURRENT_STEP_INDEX];
//                        }

//                    //on end of bar
//                    if (timer0_tempo_interval >= 10000)
//                        {
//                            timer0_tempo_interval = 0;
//                            timer0_duration_millis = 0;
//                            CURRENT_STEP_INDEX++;
//                            if(CURRENT_STEP_INDEX > NUMBER_OF_STEPS) CURRENT_STEP_INDEX = 0;
//                        }


//                    TIFR = _BV(OCF0A); // will need this if no timer interrupt not activated
//                    //OCF0A is cleared by writing a logic one to the flag.
//                    //OCF0A is cleared by hardware when executing the corresponding interrupt handling vector
//                }





        }

    return 0;   /* never reached */
}


ISR(TIMER0_COMPA_vect)
{
    timer0_tempo_interval++;

//    if(first_start)
//    {
//        OCR1A = 255 - stepFreqs[CURRENT_STEP_INDEX];
//    }


//    if(CURRENT_STEP_INDEX == 0)
//        {
//        }

    //on end of bar
    if (timer0_tempo_interval >= 61)
        {
            PORTB ^= (1 << PB4);
            timer0_tempo_interval = 0;
//            timer0_duration_millis = 0;
//            CURRENT_STEP_INDEX++;
//            if(CURRENT_STEP_INDEX > NUMBER_OF_STEPS) CURRENT_STEP_INDEX = 0;
        }

    if(first_start) first_start = 0;
}


ISR(TIMER0_COMPB_vect)
{

}
