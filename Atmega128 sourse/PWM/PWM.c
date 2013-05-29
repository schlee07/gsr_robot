
/*

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main()

{
	int count;

	DDRB = 0X10; // 0C0(PB4) 출력

	TCCR0 = 0x7E;
	TCNT0 = 0;



	while(1)
	{
		if(count <= 50)
		{
		count++;
		OCR0 = count;
		_delay_ms(25);
		}
		else 
		{
		count = 0;
		}
	}

 return 0 ;
}


*/



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>




int main()

{
	int adc_buffer;

	DDRB = 0X10; // 0C0(PB4) 출력

	TCCR0 = 0x7E;
	TCNT0 = 0;
	OCR0  = 0;

	ADMUX = 1;
	ADMUX |= 0x40;
	ADCSR = 0x86;




	while(1)
	{
		ADCSR |= 0x40;
		while( !(ADCSR & (1 << ADIF)) );
		adc_buffer = ADCL;
		adc_buffer |= (ADCH << 8); // 서로 다른 레지스터를 16비트로 정렬 하여 사용하기위해서 8bit를 shift 한다.  	
		OCR0 = adc_buffer /4;  // 1024 -> 256  
	}

 return 0 ;
}

