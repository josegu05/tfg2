#include <avr/io.h>
// definiciones de registros especificos del controlador de io
#include <avr/interrupt.h>
// gestion de IRQs
#include <stdint.h>
// definiciones de tipos estandar

// while testing PB0, in board PB7
#define PIN_OUT PINB7
#define PIN_TEST PIND7
#define WAIT_TIME 100000UL
#define DELAY 10000UL

//uint8_t volatile count = 0;
//uint8_t volatile lock_button = 0;
//uint8_t volatile code = 0;
/* code legend binary:
 * XX00 0000 = Not_valid
 * 1111 0000 = pd5
 * 1111 1100 = pd6
 * 1111 1111 = pd4
*/

//---- nuevas vars --
//enum code_t {
//   CODE0 = 0,
//   CODE1 = 1,
//   CODE2 = 2,
//   CODE3 = 3
//}
uint8_t volatile lock_button = 0;
uint8_t volatile timer_complete = 0;
uint16_t volatile code = 0;
#define TEST 1
#define CODE0 TEST
#define CODE1 2
#define CODE2 3
#define CODE3 4

//#define DEBUG
void delay(void)
{
      for(uint16_t i=0; i<DELAY; i++);
}
void wait_time(void)
{
      for(int i=0; i<WAIT_TIME; i++);
}
#ifdef DEBUG
#define COUNT 100000UL
void debug_toggle_led(void)
{
   //toggle pb0
      PORTB ^= (1 << PORTB0);
      for(int i=0; i<COUNT; i++);
      PORTB ^= (1 << PORTB0);
      for(int i=0; i<COUNT; i++);
}
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

void enable_timer2(void)
{
	 // TIMER2 interrupt COMP_A interrupt clear by writing a logic 1 if a 1 is written
	 TIFR2 |= (TIFR2 & (1 << OCIE2A));
	 //restart the count
	 TCNT2 = 0;

	 //enable timer2 interrupt mask
	 TIMSK2 = (1 << OCIE2A);
}
void transmit_sync(void)
{
	 // first transmit sync sequence
	 PORTB &= ~(1 << PIN_OUT);
	 wait_time();
	 PORTB |= (1 << PIN_OUT);

	 ////clear possible timer2 interrupt to sync timer2
	 //// TIMER2 interrupt COMP_A interrupt clear by writing a logic 1 if a 1 is written
	 //TIFR2 |= (TIFR2 & (1 << OCIE2A));
	 ////restart the count
	 //TCNT2 = 0;

	 ////enable timer2 interrupt mask
	 //TIMSK2 = (1 << OCIE2A);
}

void setup_timer_2_gate(void)
{
   // set CTC mode and COM on toggle with compare match
   TCCR2A = (1 << COM2A0) | (1 << WGM21);
   //TCCR2A = (0 << COM2A0) | (1 << WGM21);
   OCR2A = 220;

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
   timer_complete = 1;
   /*
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
      // write 1 in output pin
      PORTB |= (1 << PIN_OUT);
      // disable timer2 interrupts
      TIMSK2 &= ~(1 << OCIE2A);
   }
   else
   {
      // continue with lock button until a 0 is processed
      lock_button = 1;
      // write 0 in output pin
      PORTB &= ~(1 << PIN_OUT);
   }
*/
}

ISR(PCINT2_vect)
{
   delay();
   // auto clear the interrupt
   // read which pin invoked the interrupt line
   uint8_t irq = 0;
   irq = PIND;

   //------nueva implement ----
   if (lock_button == 0)
   {
      lock_button = 1;

      // set code value depending on which button has been pressed
      // PIND reg has state value of the PINx -> 0 is button pressed
      if ( (irq & (1 << PIND5)) == 0 )
	 code = CODE1;
      if ( (irq & (1 << PIND6)) == 0 )
	 code = CODE2;
      if ( (irq & (1 << PIN_TEST)) == 0 )
	 code = TEST;

      //debug_toggle_led();
   }

   /*
   if (!lock_button)
   {
      if ( !(irq & (1 << PIND5)) )
      {
	 //lock button
	 lock_button = 1;
	 // set appropiate code variable
	 code = 0b1111;

	 //transmit sync and set timer interrupts
	 transmit_sync();

	 // with interrupts enabled, code will be transmitted
      }

      // check if pd6 set interrupt flag and if lock button is in progress
      if ( !(irq & (1 << PIND6)) )
      {
	 //lock button
	 lock_button = 1;
	 // set appropiate code variable
	 code = 0b111111;

	 //transmit sync and set timer interrupts
	 transmit_sync();
      }

      if ( !(irq & (1 << PIND7)) )
      {
	 //lock button
	 lock_button = 1;
	 // set appropiate code variable
	 code = 0b11111111;

	 //transmit sync and set timer interrupts
	 transmit_sync();
      }
   }
   */
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
   //set init high level pnp transistor
   PORTB |= (1 << PIN_OUT);
   //PORTB &= ~(1 << PIN_OUT);
}


int main(void)
{
   uint16_t i = 0;
   uint8_t irq = 0;

   setup_timer_2_gate();
   setup_gpio_pins();

#ifdef DEBUG
   DDRB |= (1 << PORTB0);
#endif

   //enable global interrupts
   sei();

   while(1)
   {
      // code transmit algorithm
      // if code > CODE0, means code set with CODEn value
      if (code > CODE0 && lock_button == 1)
      {
	 //wait for realised 
	 //while (button-pressed)
	 irq = PIND;
	 irq = ~irq & ((1 << PIND5) | (1 << PIND6));
	 while ( irq != 0 )
	 {
	    irq = PIND;
	    irq = ~irq & ((1 << PIND5) | (1 << PIND6));
	 }
	 //disable PC_INT
	 PCICR &= ~(1 << PCIE2);

	 transmit_sync();
	 enable_timer2();
	 //transmit code sequence
	 for (i=0; i<code; i++)
	 {
	    // set low level pnp transistor (activate RF)
	    PORTB &= ~(1 << PIN_OUT);
	    // wait for timer interrupt 
	    while(!timer_complete);
	    timer_complete = 0;
	 }
	 PORTB |= (1 << PIN_OUT);

	 // disable timer2 interrupts
	 TIMSK2 &= ~(1 << OCIE2A);

	 code = 0;
	 lock_button = 0;
	 timer_complete = 0;
	 //enable PC_INT
	 PCICR |= (1 << PCIE2);
      }

      if (code == TEST && lock_button == 1)
      {
	 //wait for realised 
	 //while (button-pressed)
	 irq = PIND;
	 while ( (irq & (1 << PIN_TEST)) == 0 )
	    irq = PIND;

	 PORTB ^= (1 << PIN_OUT);

	 code = 0;
	 lock_button = 0;
	 timer_complete = 0;
      }
   }
}
