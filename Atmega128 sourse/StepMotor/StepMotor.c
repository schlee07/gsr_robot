/*

//1상 여자방식


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void delay_us(unsigned int us)
{
	register unsigned int i;

	for ( i = 0; i < us; i++){ //4cycle
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle		   
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle
	}
}

void delay_ms(unsigned int ms)
{
	register unsigned int i;

	for (i = 0; i < ms ; i++)
	{
		delay_us(250);
		delay_us(250);
		delay_us(250);
		delay_us(250);

	}

}


int main(void)
{
	int delay;

	DDRC = 0x0F;			//방향설정
	

	delay = 20;
	while(1)
	{

		for ( int i =0; i< 100; i++){
		PORTC = 0x01; 		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 0 0 1

		PORTC = 0x02;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 0 1 0

		PORTC = 0x04;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 1 0 0

		PORTC = 0x08;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 1 0 0 0
		}

		if( delay > 5 ) delay--;	
		
	}


	return 0;


}

*/


/*
 //2상 여자방식

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void delay_us(unsigned int us)
{
	register unsigned int i;

	for ( i = 0; i < us; i++){ //4cycle
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle		   
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle
	}
}

void delay_ms(unsigned int ms)
{
	register unsigned int i;

	for (i = 0; i < ms ; i++)
	{
		delay_us(250);
		delay_us(250);
		delay_us(250);
		delay_us(250);

	}

}


int main(void)
{
	int delay;

	DDRC = 0x0F;			//방향설정
	

	delay = 20;
	while(1)
	{

		for ( int i =0; i< 100; i++){
		PORTC = 0x03; 		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 0 0 1

		PORTC = 0x06;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 0 1 0

		PORTC = 0x0C;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 1 0 0

		PORTC = 0x09;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 1 0 0 0
		}

		if( delay > 5 ) delay--;	
		
	}


	return 0;


}

*/
/*
 //1-2상 여자방식 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void delay_us(unsigned int us)
{
	register unsigned int i;

	for ( i = 0; i < us; i++){ //4cycle
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle		   
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle
		asm("PUSH R0");		   //2cycle
		asm("POP  R0");		   //2cycle
	}
}

void delay_ms(unsigned int ms)
{
	register unsigned int i;

	for (i = 0; i < ms ; i++)
	{
		delay_us(250);
		delay_us(250);
		delay_us(250);
		delay_us(250);

	}

}


int main(void)
{
	int delay;

	DDRC = 0x0F;			//방향설정
	

	delay = 20;
	while(1)
	{

		for ( int i =0; i< 100; i++){
		PORTC = 0x01; 		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 0 0 1

		PORTC = 0x03;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 0 1 0

		PORTC = 0x02;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 0 1 0 0

		PORTC = 0x06;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 1 0 0 0

		PORTC = 0x04;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 1 0 0 0

		PORTC = 0x0C;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 1 0 0 0

		PORTC = 0x08;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 1 0 0 0

		PORTC = 0x09;		//PC 7 6 5 4 3 2 1 0 
		delay_ms(delay);	//   0 0 0 0 1 0 0 0
		}

		if( delay > 5 ) delay--;	
		
	}


	return 0;


}
*/




#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned int tick0;
volatile unsigned char pulse[8] = { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09 };
volatile unsigned char count = 0;


int main(void)
{

	TCCR0 = 0x05; 		 // 0 0 0 0 0 1 0 1  128분주  -> 8 us
	TCNT0 = 256- 125 ;  // 발생주기 8 us * 125 = 1ms
	TIMSK = 0x01;		 //	overflow interrupt 사용
	sei();				// avr의 전체 interrupt 활성화
	DDRC = 0x0F;			//방향설정
	
	
	while(1)
	{
	
	}


	return 0;


}


ISR(TIMER0_OVF_vect)
{
	

	TCNT0 = 256- 125 ; // 반드시 초기화 해준다.


		PORTC = pulse[count];
		count = ++count % 8;
	




}
