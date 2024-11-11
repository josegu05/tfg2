#include <avr/io.h>
// definiciones de registros especificos del controlador de io
#include <avr/interrupt.h>
// gestion de IRQs
#include <stdint.h>
// definiciones de tipos estandar

#define WAIT_TIME 1000
#define COUNT_MAX 109
#define COUNT_MIN 109

uint16_t volatile decoding = 0;
uint16_t volatile count = 0;

// AJUSTAR TIEMPOS DE LOS TIMERS CON EL OSCILOSCOPIO Y 2 SONDAS

//void set_sys_clk_prescaler_1(void)
//{
//   // setting clk_sys prescaler to 1 sets clk_sys = 8 MHz
//   CLKPR = (1 <<  CLKPCE);
//   CLKPR = 0;
//}
void setup_timer_0_counter(void)
{
   // set configuration registers
   // set CTC mode and COM on toggle with compare match
   TCCR0A = (1 << COM0A0) | (1 << WGM01);
   //TCCR0A = (0 << COM0A0) | (1 << WGM01);
   // 4 count limit frecuency for F_in = 60 KHz and F_CPU = 1 MHz
   OCR0A = 3;

//   select clock source as external source falling edge no prescaler
   TCCR0B = (1 << FOC2A) | (1 << FOC2B) | (1 << CS12) | (1 << CS11);

   // set PD6 as output wich is TIMER0 COMP_A OUTPUT
   DDRD |= (1 << DDD6);
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
   TCCR2B = (1 << CS21) | (1 << CS20);

// PB3 as output compare match A timer 2
	DDRB |= (1 << DDB3);

// set TIMER2 interrupt COMP_A mask enable 
   TIMSK2 = (1 << OCIE2A);

}

ISR(TIMER2_COMPA_vect)
{
   // gate

   if(count > COUNT_MAX)
   {
      // pinout PB0
      // digital_out = 0;
      decoding = (decoding + 1) % sizeof(uint16_t);
      PORTB &= ~(1 << PORTB0);
   }

   else
   //if(count < COUNT_MIN)
   {
	 PORTB |= (1 << PORTB0);
      if(decoding)
      {
	 // digital_out = 1;
	 switch (decoding)
	 {
	    case 4:
	       // pd5 = 1;
	       PORTD |= (1 << PORTD5);
	       break;
	    case 6:
	       // pd6 = 1;
	       PORTD |= (1 << PORTD6);
	       break;
	    case 8:
	       // pd7 = 1;
	       PORTD |= (1 << PORTD7);
	       break;
	    default:
	       break;
	 }

	 decoding = 0;
      }
   }

   count = 0;
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

void wait_init(void)
{
      for(int i=0; i<WAIT_TIME; i++);
}
int main(void)
{
  // set_sys_clk_prescaler_1();
  // wait_init();
   setup_timer_0_counter();
   setup_timer_2_gate();
   setup_gpios();

   //enable global interrupts
   sei();

   while(1);
}
