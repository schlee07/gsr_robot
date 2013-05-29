#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // ���ͷ�Ʈ ����� ���� �������

volatile unsigned int tick0;
volatile char min,sec;
volatile unsigned char segTable[10] = {
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 
	};
volatile char count = 0;










int main(void)
{
	DDRA = 0xFF;     // 7 6 5 4 3 2 1 0  
	DDRE = 0x0C;	 // 0 0 0 0 1 1	0 0
	DDRF = 0x0C;	 // 0 0 0 0 1 1	0 0


	TCCR0 = 0x05; 		 // 0 0 0 0 0 1 0 1  128����  -> 8 us
	TCNT0 = 256- 125 ;  // �߻��ֱ� 8 us * 125 = 1ms
	TIMSK = 0x01;		 //	overflow interrupt ���
	sei();				// avr�� ��ü interrupt Ȱ��ȭ





	while(1)
	{
		_delay_ms(100);

	}


	return 0;
}



// Timer0 Overflow interrupt Handler
// 1ms
ISR(TIMER0_OVF_vect) // ��� ���ͷ�Ʈ �̸��� ISR�̶�� ��Ī�� ���´�. () <- �ȿ� ���ͷ�Ʈ ���͹�ȣ�� ����.
{
	TCNT0 = 256- 125 ; // �ݵ�� �ʱ�ȭ ���ش�.
	tick0++;

	if ( tick0 == 100)
	{
		tick0 = 0;
		sec++;
		if( sec == 60)
		{
			sec=0; 
			min++;
		}
		if(min == 60)
		{
			min = 0;
		}


	}
	
	PORTE = 0x00; 
	PORTF = 0x00;
	
	if  (count == 0 ) 
	{
		PORTA = segTable[sec % 10]; 
		PORTF = 0x04;
	}
    else if (count == 1)
	{
	 	PORTA =segTable[sec / 10 ] ;
		PORTF =0x08;
	}
	else if (count == 2)
	{
	 	PORTA =segTable[min % 10 ] ;
		PORTE =0x04;
	}
	else if (count == 3)
	{
	 	PORTA =segTable[min / 10 ];
		PORTE =0x08;
	}

	count = ++count % 4;

}

