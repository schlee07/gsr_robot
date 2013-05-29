// FND
/*

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{	
	while(1)

	{       // common anode  ���̳��� ���
	       
	 //C3	           
	 PORTA = 0xF9;

	 PORTE = 0x08;
	          
	 PORTF = 0x00;   
	 
	 _delay_ms(5);


	 //C2
	 PORTA = segTable[2];
	 
	 PORTE = 0x04;
	   
     PORTF = 0x00;

	 _delay_ms(5);


	 //C1
	 PORTA = 0xB0;
	 
	 PORTE = 0x00;
	   
     PORTF = 0x08;
	 
	 _delay_ms(5);
	 
	 //C0
     PORTA = 0x99;
	 
	 PORTE = 0x00;
	   
     PORTF = 0x04;

	 _delay_ms(5);

	}



   return 0 ;
}



*/



#include <avr/io.h>
#include <util/delay.h>

volatile unsigned char segTable[10] = {
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
}; // �������� 


void fndDisplay(int num) // �������� : �Լ��� ȣ��Ǿ� ����ɶ����� �ʱ�ȭ�ȴ�.
{
	static char count = 0; //static ��������  / ���������� ���� �ٲ��� �ʴ´�. ����

	PORTE = 0x00; 
	PORTF = 0x00;
	
	if (count == 0 ) 
	{
		PORTA = sec % 10; 
		PORTF = 0x04;
	}
    else if (count == 1)
	{
	 	PORTA =sec % 10 ;
		PORTF =0x08;
	}
	else if (count == 2)
	{
	 	PORTA =min % 10 ;
		PORTE =0x04;
	}
	else if (count == 3)
	{
	 	PORTA =min % 10 ;
		PORTE =0x08;
	}

	count =++count % 4;
	_delay_ms(1);


}


int main(void)
{				      // 7 6 5 4 3 2 1 0
	DDRA = 0xFF;     // 7 6 5 4 3 2 1 0  
	DDRE = 0x0C;	 // 0 0 0 0 1 1	0 0
	DDRF = 0x0C;	 // 0 0 0 0 1 1	0 0

	while(1)
	{
      fndDisplay(219);
	}



   return 0 ;
}
