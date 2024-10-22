#include <avr/io.h>
#include <stdint.h>

#define COUNT 1000000UL
#define PIN 0

int main(void)
{
   DDRB = (1 << PIN);

   while(1)
   {
      PORTB = (1 << PIN);
      for(int i=0; i<COUNT; i++);
      PORTB = (0 << PIN);
      for(int i=0; i<COUNT; i++);
   }
}
