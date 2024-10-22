int setup(void)
{
   //Enable global interrupts
   SREG |= 1 << 7;
   //ADCSRA -> general reg: enable, prescaler-bits, ADLAR = 1 -> 8bit
   ADCSRA = ADEN | ADLAR;

   //ADMUX -> select adport ADC0 mux bits, 
   return 0;
}
