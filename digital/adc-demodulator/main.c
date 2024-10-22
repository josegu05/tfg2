#include<avr/io>

int setup(void);

int main(void)
{
	if (setup())
	{
		//mostrar error en puerto b(n)
		PORTB << 1;
	}

	while(1)
	{
		//procedure: wait for ADC interrupt, then handle it
		//ADCH result register, 8-bit precision
	}
	return 0;
}




