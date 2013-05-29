#include <avr/io.h>
#include <util/delay.h>


volatile unsigned char segTable[10] = {
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
}; // 전역번수 


void fndDisplay(int num) // 지역변수 : 함수가 호출되어 실행될때마다 초기화된다.
{
	static char count = 0; //static 정적변수  / 지역변수라도 값이 바뀌지 않는다. 유지

	PORTE = 0x00; 
	PORTF = 0x00;
	
	if (count == 0 ) 
	{
		PORTA = segTable[num % 10]; 
		PORTF = 0x04;
	}
    else if (count == 1)
	{
	 	PORTA =segTable[(num % 100)  / 10 ] ;
		PORTF =0x08;
	}
	else if (count == 2)
	{
	 	PORTA =segTable[(num % 1000)  / 100 ] ;
		PORTE =0x04;
	}
	else if (count == 3)
	{
	 	PORTA =segTable[(num / 1000) ];
		PORTE =0x08;
	}

	count = ++count % 4;
	_delay_ms(1);


}





















int main (void)
{
	DDRD  =0x00;
	DDRA = 0xFF;     // 7 6 5 4 3 2 1 0  
	DDRE = 0x0C;	 // 0 0 0 0 1 1	0 0
	DDRF = 0x0C;	 // 0 0 0 0 1 1	0 0

	unsigned char key;


	while(1)
	{
		key = ~PIND & 0xF0;

		if( key == 0x20)
		{
	    	fndDisplay(1);
		
		}

		else if( key == 0x40)
		{
	    	fndDisplay(10);
		}

		else if( key == 0x80)
		{
	    	fndDisplay(100);
		}

		else if( key == 0x10)
		{
	    	fndDisplay(1000);
		}
		else  {
		
			fndDisplay(0000);
		}
	}


	return 0;




}
