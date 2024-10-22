#include <avr/io.h>
// definiciones de registros especificos del controlador de io
#include <avr/interrupt.h>
// gestion de IRQs
#include <stdint.h>
// definiciones de tipos estandar

#define WAIT_TIME 10000UL
#define COUNT_MAX 79
#define COUNT_MIN 79

uint8_t volatile count = 0;
uint8_t volatile count_before = 0;
uint8_t volatile mean = 0;

// AJUSTAR TIEMPOS DE LOS TIMERS CON EL OSCILOSCOPIO Y 2 SONDAS

void setup_timer_0_counter(void)
{
   // set configuration registers
   // set CTC mode and COM on toggle with compare match
   TCCR0A = (1 << COM0A0) | (1 << WGM01);
   //TCCR0A = (0 << COM0A0) | (1 << WGM01);
   // 4 count limit frecuency for F_in = 60 KHz and F_CPU = 1 MHz
   OCR0A = 4;

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
}

void setup_timer_2_gate(void)
{
   // set CTC mode and COM on toggle with compare match
   TCCR2A = (1 << COM2A0) | (1 << WGM21);
   //TCCR2A = (0 << COM2A0) | (1 << WGM21);
   OCR2A = 200;

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
   //mean = (count + count_before) >> 1;

   if(count > COUNT_MAX)
      // pinout PB0
      // digital_out = 0;
      PORTB &= ~(1 << PORTB0);
   if(count < COUNT_MIN)
      // digital_out = 1;
      PORTB |= (1 << PORTB0);

//   count_before = count;
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
