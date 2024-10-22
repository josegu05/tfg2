#include <avr/io.h>
// definiciones de registros especificos del controlador de io
#include <avr/interrupt.h>
// gestion de IRQs
#include <stdint.h>
// definiciones de tipos estandar

#define COUNT 1000000UL

void setup_timer_0_gate(void)
{
   // set configuration registers
   // set CTC mode and COM on toggle with compare match
   TCCR0A = (1 << COM0A0) | (1 << WGM01);
   OCR0A = 0XFF;
//   select clock source as internal no prescaler
   // TCCR0B = (1 << FOC0A) | (1 << FOC0B) | (1 << CS00);
   TCCR0B = (1 << CS00);

   // set PD6 as output wich is TIMER0 COMP_A OUTPUT
   DDRD = (1 << DDD6);
// set TIMER0 interrupt COMP_A mask enable 
   TIMSK0 = (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
   //toggle pb0
      PORTB ^= (1 << PORTB0);
      for(int i=0; i<COUNT; i++);
      PORTB ^= (1 << PORTB0);
      for(int i=0; i<COUNT; i++);

}

// int setup_timer_1_count(void)
// {
//    // set CTC mode and COM on toggle with compare match
//    TCCR0A = (1 << COM0A0) | (1 << WGM01);
//    OCR0A = 0XFF;
// //   select clock source as internal no prescaler
//    TCCR0B = (1 << FOC0A) | (1 << FOC0B) | (1 << CS00);
// 
//    // set PD6 as output wich is TIMER0 COMP_A OUTPUT
//    DDRD = (1 << DDD6);
// // set TIMER0 interrupt COMP_A mask enable 
//    TIMSK0 = (1 << OCIE0A);
// 
// }

int main(void)
{
//   PRTIM0 = 0 to enable in power reduction
   // if needed output pin for counter/timer set DDR register as output
   setup_timer_0_gate();

   DDRB = (1 << DDB0);

   //enable global interrupts
   sei();

   while(1);
}
