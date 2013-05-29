/*
#include <avr/io.h>
#include <util/delay.h>



int main(void)
{
	DDRA = 0xFF ; 
	DDRB = 0xFF ; 
	



	while(1)
	{
		//1
		PORTA = 0x00;
		PORTB = 0xFE;
		_delay_ms(1);	
		//2
		PORTA = 0x00;
		PORTB = 0xFD;
		_delay_ms(1);
		//3
		PORTA = 0x00;
		PORTB = 0xFB;
		_delay_ms(1);
		//4
		PORTA = 0x82;
		PORTB = 0xF7;
		_delay_ms(1);
		//5
		PORTA = 0xFF;
		PORTB = 0xEF;
		_delay_ms(1);
		//6
		PORTA = 0x80;
		PORTB = 0xDF;
		_delay_ms(1);
		//7
		PORTA = 0x00;
		PORTB = 0xBF;
		_delay_ms(1);
		//8
		PORTA = 0x00;
		PORTB = 0x7F;
		_delay_ms(1);

		
	}


 return 0;

}

*/

/*

#include <avr/io.h>
#include <util/delay.h>

volatile unsigned char font[5][8] = {
										{ 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00 },
										{ 0x00, 0x6C, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00 },
										{ 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0x00 },
										{ 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00 },
										{ 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00 }
									};
volatile unsigned char digit[8] =   { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

void dotDisplay(int num)
{
	static char count = 0;
	
	PORTA = 0x00;

	PORTB = ~font[num][count];
	PORTA = digit[count];

	count = ++count % 8;
	_delay_ms(1);
}

	
int main(void)
{
	DDRA = 0xFF ; 
	DDRB = 0xFF ; 
	



	while(1)
	{
		for( int i=0 ; i<5 ; i++){
			for(int j=0 ; j<500 ; j++){
					dotDisplay(i);
			}

		}
	}


 return 0;

}
*/


/*
#include <avr/io.h>
#include <util/delay.h>

volatile unsigned char font[7] = {
	
int main(void)
{
	DDRA = 0xFF ; 
	DDRB = 0xFF ; 
	



	while(1)
	{
	  PORTA= 0x7C;
	  PORTB= ~0x01;
	  _delay_ms(30);
	 
	  PORTA= 0x7E;
	  PORTB= ~0x01;
	  _delay_ms(30);
	  
	  PORTA= 0x13;
	  PORTB= ~0x01;
	  _delay_ms(30); 
	  PORTA= 0x11;
	  PORTB= ~0x01;
	  _delay_ms(30);
	  PORTA= 0x11;
	  PORTB= ~0x01;
	  _delay_ms(30);
	  PORTA= 0x13;
	  PORTB= ~0x01;
	  _delay_ms(30);
	  PORTA= 0x7C;
	  PORTB= ~0x01;
	  _delay_ms(30);



	}


 return 0;

}

*/
