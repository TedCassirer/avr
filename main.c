/* port_test.c
 * The following program just  keeps toggling pin 0 of port B
 */

#define F_CPU 1000000UL
#include <avr/io.h> //standard include for ATMega16
#include <util/delay.h>
#define sbi(x,y) x |= _BV(y) //set bit - using bitwise OR operator 
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator
#define is_high(x,y) (x & _BV(y) == _BV(y)) //check if the y'th bit of register 'x' is high ... test if its AND with 1 is 1

/* _BV(a) is a macro which returns the value corresponding to 2 to the power 'a'. Thus _BV(PX3) would be 0x08 or 0b00001000 */


void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);
 
    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch)
{
  // select the corresponding channel 0~7
  // ANDing with ’7′ will always keep the value
  // of ‘ch’ between 0 and 7
  ch &= 0b00000111;  // AND operation with 7
  ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
 
  // start single convertion
  // write ’1′ to ADSC
  ADCSRA |= (1<<ADSC);
 
  // wait for conversion to complete
  // ADSC becomes ’0′ again
  // till then, run loop continuously
  while(ADCSRA & (1<<ADSC));
 
  return (ADC);
}

void main(void)
{
    uint16_t adc_result;


    adc_init();
    DDRA=0x00;
    DDRB=0xFF; //PORTB as OUTPUT
    PORTA=0xFF;
    PORTB=0xFF; //All pins of PORTB LOW


    while(1) //Infinite loop
    {
        // _delay_ms(1000);

        adc_result = adc_read(0) >> 3;


    	for (uint8_t i = 7; i >= 0; i--) {
    		if (adc_result >= 1 << i) {
    			PORTB = ~(1 << i);
    			break;
    		}
    	}




     	// PORTB = ~(adc_read(0) >> 3);
        // tbi(PORTB,PB7);   //here the toggling takes place
    }
}
