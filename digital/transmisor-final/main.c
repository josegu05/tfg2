#include <avr/io.h>
// definiciones de registros especificos del controlador de io
#include <avr/interrupt.h>
// gestion de IRQs
#include <stdint.h>
// definiciones de tipos estandar

#define PIN_OUT PINB0

uint8_t volatile lock_button = 0;
uint8_t volatile code = 0;
/* code legend binary:
 * 0000 = Not_valid
 * 1000 = pd5
 * 1100 = pd6
 * 1110 = pd4
*/

#ifdef DEBUG
#define COUNT 100000UL
void debug_stop_exec(void)
{
   //toggle pb0
      PORTB ^= (1 << PORTB0);
      for(int i=0; i<COUNT; i++);
      PORTB ^= (1 << PORTB0);
      for(int i=0; i<COUNT; i++);
      while(1);
}
#endif

void setup_timer_2_gate(void)
{
   // set CTC mode and COM on toggle with compare match
   TCCR2A = (1 << COM2A0) | (1 << WGM21);
   //TCCR2A = (0 << COM2A0) | (1 << WGM21);
   OCR2A = 200;

//   select clock source as internal prescaler 32
   TCCR2B = (1 << CS21) | (1 << CS20);

//   PB3 as output compare match A timer 2
	DDRB |= (1 << DDB3);

// set TIMER2 interrupt COMP_A mask enable 
  // TIMSK2 = (1 << OCIE2A);

   // enable timer2 in power reduction
   //PRR &= ~(1 << PRTIM2);

}

ISR(TIMER2_COMPA_vect)
{
   // gate
   uint8_t current_code = 0;

   // check if LSB of variable code is 0 or 1
   current_code = (0b1 & code);
   // LSR variable code to load next LSB
   code = (code >> 1);
   
   // detectado 0 -> desbloquear boton y desactivar mask
   if(!current_code)
   {
      // unlock button press
      lock_button = 0;
      // write 0 in output pin
      PORTB &= ~(1 << PIN_OUT);
      // disable timer2 interrupts
      TIMSK2 &= ~(1 << OCIE2A);
   }
   else
   {
      // continue with lock button until a 0 is processed
      lock_button = 1;
      // write 1 in output pin
      PORTB |= (1 << PIN_OUT);
   }

}

ISR(PCINT2_vect)
{
   // auto clear the interrupt
   // read which pin invoked the interrupt line
   uint8_t irq = 0;
   irq = PIND;

//   if (button_pressed & irq)
   if (!lock_button)
   {
      // check if pd6 set interrupt flag and if lock button is in progress
      if ( !(irq & (1 << PIND6)) )
      {
	 //lock button
	 lock_button = 1;

	 // set appropiate code variable
	 code = 0b11;

	 //clear possible timer2 interrupt to sync timer2
	 // TIMER2 interrupt COMP_A interrupt clear by writing a logic 1 if a 1 is written
	 //TIFR2 |= (TIFR2 & (1 << OCIE2A));
	 TCNT2 = 0;

	 //enable timer2 interrupt mask
	 TIMSK2 = (1 << OCIE2A);
      }

      if ( !(irq & (1 << PIND5)) )
      {
	 //lock button
	 lock_button = 1;

	 // set appropiate code variable
	 code = 0b01;

	 //clear possible timer2 interrupt to sync timer2
	 // TIMER2 interrupt COMP_A interrupt clear by writing a logic 1 if a 1 is written
	 //TIFR2 |= (TIFR2 & (1 << OCIE2A));
	 TCNT2 = 0;

	 //enable timer2 interrupt mask
	 TIMSK2 = (1 << OCIE2A);
      }

      if ( !(irq & (1 << PIND7)) )
      {
	 //lock button
	 lock_button = 1;

	 // set appropiate code variable
	 code = 0b111;

	 //clear possible timer2 interrupt to sync timer2
	 // TIMER2 interrupt COMP_A interrupt clear by writing a logic 1 if a 1 is written
	 //TIFR2 |= (TIFR2 & (1 << OCIE2A));
	 TCNT2 = 0;

	 //enable timer2 interrupt mask
	 TIMSK2 = (1 << OCIE2A);
      }
   }
}
void setup_gpio_pins(void)
{

   //configurar gpios pd5, pd6, pd7 as input
   DDRD &= ~(1 << DDD5);
   DDRD &= ~(1 << DDD6);
   DDRD &= ~(1 << DDD7);

   // activate pull-ups
   PORTD |= (1 << PORTD5);
   PORTD |= (1 << PORTD6);
   PORTD |= (1 << PORTD7);

   // activate the pin change interrupts
   // first activate PC2vector
   PCICR |= (1 << PCIE2);
   // second activate pd5 -> PC21, pd6 -> PC22, pd7 -> PC23 to trigger interrupt 
   PCMSK2 |=  ((1 << PCINT21) |(1 << PCINT22) |(1 << PCINT23));

   //config pb7 as output 
   DDRB |= (1 << PIN_OUT);
   //set init low level 
   //set init high level 
   PORTB &= ~(1 << PIN_OUT);
}

int main(void)
{
   setup_timer_2_gate();
   setup_gpio_pins();

   //enable global interrupts
   sei();

   while(1);
}
