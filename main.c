/*
Coder:          Maxime Beauchemin
Coded on:       July
Description:    The goal here is to calibrate an ATTINY internal oscilator. The OSCCAL register is used to do just that. Goal is to be as close as 8MHZ as possible.
                So. This program sends a tick (lifts a pin) to a clocked Arduino. It listens on another pin to know when it went over 8MHZ.
                When it knows it went over, it goes back to the previous value of the OSCCAL and writes that value to the EEPROM.
                Why EEPROM? Well cause the value of OSCCAL is reset at every reset of the chip. So the actual program would check for a non 0 value in the eeprom and load it
                in the OSCCAL
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int nbOVF;

//PB4 is write
//PB5 is read

int main(void)
{
    DDRB = 0b00010000;

    TCCR0A &=  0b11110111;//ctc normal
    TCCR0A |=  0b00000001;//no prescaler
    nbOVF = 0;
    for(;;)
    {
        if(nbOVF==122)
        {
            //122 overflows (8MHZ / 65536 ) should be every second
            PORTB |= 0b00010000;
            nbOVF = 0;
            _delay_ms(5);
            PORTB |= 0b11101111;
        }
    }
    return 0;
}

SIGNAL(TIMER1_OVF_vect ){
    //16bit, overflows at every 65536 cycles, counting overflows
    nbOVF +=1;
}
