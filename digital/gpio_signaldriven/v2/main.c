#include <avr/io.h>
// definiciones de registros especificos del controlador de io
#include <avr/interrupt.h>
// gestion de IRQs
#include <stdint.h>
// definiciones de tipos estandar

#define COUNT 1000000UL

uint8_t volatile count = 0;

void setup_timer_0_gate(void)
{
   // set configuration registers
   // set CTC mode and COM on toggle with compare match
   TCCR0A = (1 << COM0A0) | (1 << WGM01);
   OCR0A = 0X0;
//   select clock source as internal no prescaler
//   TCCR0B = (1 << FOC0A) | (1 << FOC0B) | (1 << CS00);
   // TCCR0B = (1 << CS00);

//   select clock source as external source falling edge no prescaler
   TCCR0B = (1 << FOC2A) | (1 << FOC2B) | (1 << CS12) | (1 << CS11);

   // set PD6 as output wich is TIMER0 COMP_A OUTPUT
   DDRD = (1 << DDD6);
// set TIMER0 interrupt COMP_A mask enable 
   TIMSK0 = (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
	count = 2;
}

void setup_timer_2_count(void)
{
   // set CTC mode and COM on toggle with compare match
   TCCR2A = (1 << COM2A0) | (1 << WGM21);
   OCR2A = 0;
//   select clock source as external source falling edge no prescaler
   //TCCR2B = (1 << FOC2A) | (1 << FOC2B) | (1 << CS12) | (1 << CS11);

   TCCR2B = (1 << CS00);

// PB3 as output compare match A timer 2
	DDRB = (1 << DDB3);

// set TIMER2 interrupt COMP_A mask enable 
   TIMSK2 = (1 << OCIE2A);

}

ISR(TIMER2_COMPA_vect)
{
	count = 1;
}

int main(void)
{
//   PRTIM0 = 0 to enable in power reduction and to enable timer2 IMPORTANTE
   setup_timer_0_gate();
   setup_timer_2_count();

   //enable global interrupts
   sei();

   while(1);
}
