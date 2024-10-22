#include <avr/io.h>
// definiciones de registros especificos del controlador de io
#include <avr/interrupt.h>
// gestion de IRQs
#include <stdint.h>
// definiciones de tipos estandar

#define WAIT_TIME 10000UL

uint16_t volatile count = 0;
uint16_t volatile count_before = 0;
uint16_t volatile mean = 0;

void setup_timer_0_counter(void)
{
   // set configuration registers
   // set CTC mode and COM on toggle with compare match
   TCCR0A = (1 << COM0A0) | (1 << WGM01);
   OCR0A = 0X0;

//   select clock source as external source falling edge no prescaler
   TCCR0B = (1 << FOC2A) | (1 << FOC2B) | (1 << CS12) | (1 << CS11);

   // set PD6 as output wich is TIMER0 COMP_A OUTPUT
   DDRD = (1 << DDD6);
// set TIMER0 interrupt COMP_A mask enable 
   TIMSK0 = (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
   //counter
     count++;
   // count = 0;
}

void setup_timer_2_gate(void)
{
   // set CTC mode and COM on toggle with compare match
   TCCR2A = (1 << COM2A0) | (1 << WGM21);
   OCR2A = 20;

//   select clock source as internal prescaler 32
   TCCR2B = (1 << CS21) | (1 << CS20);

// PB3 as output compare match A timer 2
	DDRB = (1 << DDB3);

// set TIMER2 interrupt COMP_A mask enable 
   TIMSK2 = (1 << OCIE2A);

}

ISR(TIMER2_COMPA_vect)
{
   // gate
   mean = (count + count_before) >> 1;

   if(count >= mean)
      // pinout PB0
      // digital_out = 0;
      PORTB = (0 << PORTB0);
   else
      // digital_out = 1;
      PORTB = (1 << PORTB0);

   count_before = count;
   count = 0;
}

void wait_init(void)
{
      for(int i=0; i<WAIT_TIME; i++);
}
int main(void)
{
//   PRTIM0 = 0 to enable in power reduction and to enable timer2 IMPORTANTE
   setup_timer_0_counter();
   setup_timer_2_gate();

   DDRB |= (1 << DDB0);

   //enable global interrupts
   sei();

   wait_init();

   while(1);
}
