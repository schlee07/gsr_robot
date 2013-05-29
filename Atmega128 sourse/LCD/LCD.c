#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // 인터럽트 사용을 위한 헤더파일

#define LCD_RS   PG0//0
#define LCD_RW   PG1
#define LCD_E    PG2
#define LCD_DATA PORTB
#define LCD_IN   PINB

volatile unsigned int tick0;
volatile char min,sec;
volatile char count = 0;
volatile unsigned char segTable[10] = {
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 
	};

void Timer0(void)
{
	DDRA = 0xFF;     // 7 6 5 4 3 2 1 0  
	DDRE = 0x0C;	 // 0 0 0 0 1 1	0 0
	DDRF = 0x0C;	 // 0 0 0 0 1 1	0 0


	TCCR0 = 0x05; 		 // 0 0 0 0 0 1 0 1  128분주  -> 8 us
	TCNT0 = 256- 125 ;  // 발생주기 8 us * 125 = 1ms
	TIMSK = 0x01;		 //	overflow interrupt 사용
	sei();				// avr의 전체 interrupt 활성화
}


void Time (void)
{
	


}

void Fnd (void)
{
	tick0++;

	if ( tick0 == 1000)
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





char lcdReadCommand(void)
{
 char data; 

 DDRB = 0x00;

 PORTG &= ~(1<<LCD_RS);
 PORTG |=  (1<<LCD_RW);
 PORTG |=  (1<<LCD_E);
 
 data = LCD_IN;

 PORTG &= ~(1<<LCD_E);

 return data;

}
void lcdWriteCommand(unsigned char cmd)
{
// while(lcdReadCommand() & 0x80);
 
 _delay_ms(1);

 DDRB = 0xFF;

 PORTG &= ~(1<<LCD_RS);
 PORTG &= ~(1<<LCD_RW);
 LCD_DATA = cmd;

 PORTG |= (1<<LCD_E);
 PORTG &= ~(1<<LCD_E);
}


char lcdReadData(void)
{
 char data; 

 DDRB = 0x00;

 PORTG |= (1<<LCD_RS);
 PORTG |= (1<<LCD_RW);
 PORTG |= (1<<LCD_E);
 
 data = LCD_IN;

 PORTG &= ~(1<<LCD_E);

 return data;

}
void lcdWriteData(unsigned char data)
{
// while(lcdReadCommand() & 0x80);

 _delay_ms(5);

 DDRB = 0xFF;

 PORTG |= (1<<LCD_RS);
 PORTG &= ~(1<<LCD_RW);
 LCD_DATA = data;
 PORTG |= (1<<LCD_E);
 PORTG &= ~(1<<LCD_E);
}
void lcdInit(void)
{
 DDRB = 0xFF;
 DDRG = 0x07;

 _delay_ms(15);
 lcdWriteCommand(0x38);
 _delay_ms(5);
 lcdWriteCommand(0x38);
 _delay_ms(100);
 lcdWriteCommand(0x38);
 lcdWriteCommand(0x08);
 lcdWriteCommand(0x01);
 lcdWriteCommand(0x06);
 lcdWriteCommand(0x0C);

}

void lcdString(char *str)
{
	while(*str)		lcdWriteData(*str++);
	
}

void lcdString2(char *num0)
{
	while(*str)		lcdWriteData(*str++);
	
}

void lcdGotoXY(char x, char y)
{
	lcdWriteCommand(0x80 | x | (y * 0x40));
} 

void lcdNumber(unsigned char num)
{
	lcdWriteData(num / 100 + 0x30);				// 백의 자리
	lcdWriteData(((num % 100) / 10) + 0x30);	// 십의 자리
	lcdWriteData((num % 10) + 0x30);			// 일의 자리 
}

void lcdTimer(void)
{
	


}




int main(void)
{  
  Timer0();
  lcdInit();
  lcdGotoXY(0, 0);
  char num0;

//  lcdString("num=");

//  unsigned char num = 250;

//  lcdNumber(num);

  while(1)
  {	lcdString2();
	Timer0();
	_delay_ms(10);

  }
	
 return 0;
} 



ISR(TIMER0_OVF_vect) // 모든 인터럽트 이름은 ISR이라는 명칭을 갖는다. () <- 안에 인터럽트 백터번호가 들어간다.
{
	TCNT0 = 256- 125 ; // 반드시 초기화 해준다.
	tick0++;

	Fnd();
	

}



