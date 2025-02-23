#include <avr/io.h>
// definiciones de registros especificos del controlador de io
#include <avr/interrupt.h>
// gestion de IRQs
#include <stdint.h>
// definiciones de tipos estandar
#include "codes_table.h"

//#define WAIT_TIME 1000
#define THRESHOLD 6
#define GATE_COUNT_LIMIT 6
#define TIMES_STABLE_BOOT 500
uint8_t header = 0;
uint16_t volatile decoding = 0;
int32_t volatile count = 0;
int32_t last_count = 0;
float mean = 0;
float last_mean = 0;
uint16_t volatile gate_times = 0;

void setup_timer_0_counter(void)
{
   // set configuration registers
   // set CTC mode and COM on toggle with compare match
   //TCCR0A = (1 << COM0A0) | (1 << WGM01);
   // 
   TCCR0A = (0 << COM0A0) | (1 << WGM01);
   // 4 count limit frecuency for F_in = 60 KHz and F_CPU = 1 MHz
   OCR0A = 3;

//   select clock source as external source falling edge no prescaler
//   PD4 is the Alternate function to External T0 clk
   TCCR0B = (1 << FOC2A) | (1 << FOC2B) | (1 << CS12) | (1 << CS11);

   // set PD6 as output wich is TIMER0 COMP_A OUTPUT
   //DDRD |= (1 << DDD6);
// set TIMER0 interrupt COMP_A mask enable 
   TIMSK0 = (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
   //counter
     count++;
}

void setup_timer_2_gate(void)
{
   // set CTC mode and COM on toggle with compare match
   TCCR2A = (1 << COM2A0) | (1 << WGM21);
   //TCCR2A = (0 << COM2A0) | (1 << WGM21);
   OCR2A = 220;

//   select clock source as internal prescaler 32
//   TCCR2B = (1 << CS21) | (1 << CS20);
//   select clock source as internal prescaler 32*8 at 8MHz
   TCCR2B = (1 << CS22) | (1 << CS21) | (0 << CS20);

// PB3 as output compare match A timer 2
	DDRB |= (1 << DDB3);

// set TIMER2 interrupt COMP_A mask enable 
   TIMSK2 = (1 << OCIE2A);

}

ISR(TIMER2_COMPA_vect)
{
   // gate
   gate_times++;
}

void setup_gpios(void)
{
   // as output gpios pb0=general 
   DDRB |= (1 << DDB0);
   // as output decoded outputs and set to 0
   DDRD |= ((1<< DDD5) | (1<< DDD6) | (1<< DDD7));
   PORTD &= ~(1 << PORTD5);
   PORTD &= ~(1 << PORTD6);
   PORTD &= ~(1 << PORTD7);
}

//void wait_init(void)
//{
//      for(int i=0; i<WAIT_TIME; i++);
//}
void startup (uint16_t* stable_boot)
{      
   float diff;

   if (gate_times > GATE_COUNT_LIMIT)
   {
      // disable global interrupts
      cli();

      // calculate mean 
      mean = (float) count / gate_times;

      // evaluate if signal is stable
      diff = mean - last_mean; 
      if ( (diff < THRESHOLD) )
      {
	 // one time stable
	 *stable_boot = *stable_boot + 1;
      }
      else
      {
	 // continue booting
	 *stable_boot = 0;
      }

      last_mean = mean;
      gate_times = 0;
      count = 0;
      // clear past interrupts
      //TIFR2 |= (1 << OCF2B) |(1 << OCF2A) |(1 << TOV2); 
      TIFR2 |=  (1 << OCF2A); 
      TIFR0 |=  (1 << OCF0A); 
      //reset timers
      TCNT0 = 0;
      TCNT2 = 0;
      // enable interrupts
      sei();
   }
}

int32_t module(int32_t value)
{
   if (value < 0)
   {
      return -value;
   }
   else
   {
      return value;
   }
}

int main(void)
{
   uint16_t stable_boot = 0;
   int32_t dif = 0;

   setup_timer_0_counter();
   setup_timer_2_gate();
   setup_gpios();

   //enable global interrupts
   sei();

   // while booting turn on led
      PORTB |= (1 << PORTB0);
   //startup function
   while (stable_boot < TIMES_STABLE_BOOT)
   {
      startup(&stable_boot);
   }
   // turn off led
      PORTB &= ~(1 << PORTB0);

   while(1)
   {
      if (gate_times > GATE_COUNT_LIMIT)
      {
	 // disable global interrupts
	 cli();

	 // calculate mean 
	 //mean = (float) count / gate_times;

	 // evaluate if signal has been detected
	 dif = module(count - last_count);
	 if ( dif < THRESHOLD)
	 {
	    // '1' received
	    decoding++;
	 }
	 else if(decoding > 0)
	 {
	    // '0' received
	    if (header == 1)
	    {
		  switch (decoding)
		  {
		     case CODE0:
			// pd5 = 1;
			PORTD ^= (1 << PORTD5);
			break;
		     case CODE1:
			// pd6 = 1;
			PORTD ^= (1 << PORTD6);
			break;
		     case CODE2:
			// pd7 = 1;
			PORTD ^= (1 << PORTD7);
			break;
		     default:
			break;
		  }
	       header = 0;
	    }
	    else 
	    {
	       header = 1;
	    }

	    decoding = 0;
	 }

	 last_count = count;
	 gate_times = 0;
	 count = 0;
	 // clear past interrupts
	 // clear past interrupts
	 //TIFR2 |= (1 << OCF2B) |(1 << OCF2A) |(1 << TOV2); 
	 TIFR2 |=  (1 << OCF2A); 
	 TIFR0 |=  (1 << OCF0A); 
	 //reset timers
	 TCNT0 = 0;
	 TCNT2 = 0;
	 // enable interrupts
	 sei();
      }
   }
}
