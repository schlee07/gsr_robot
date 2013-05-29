#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



#define LCD_RS   PG0//0
#define LCD_RW   PG1
#define LCD_E    PG2
#define LCD_DATA PORTB
#define LCD_IN   PINB


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


void lcdGotoXY(char x, char y)
{
	lcdWriteCommand(0x80 | x | (y * 0x40));
} 

void lcdNumber(unsigned int num)
{
	lcdWriteData((num / 1000) + 0x30);				// 천의 자리 
	lcdWriteData(((num % 1000) / 100) + 0x30);		// 백의 자리
	lcdWriteData(((num % 100) / 10) + 0x30);		// 십의 자리 
	lcdWriteData((num % 10) + 0x30);				// 일의 자리 
}



 int main(void)
{	
	int adc_buffer, vol;
	double voltage; 

	lcdInit();
//	ADMUX  = 0; 		//입력채널 설정
	ADMUX |= 0X40;	   // 기준전압을 AVCC로 , 좌측정렬
	ADCSR = 0X86;		//ADC 인에이블, 64분주 

 

	while(1){
	
	
	
	
		ADMUX ^= 0x01;
	
		ADCSR |= 0X40;
		while( !(ADCSR & (1 << ADIF)) );
		adc_buffer = ADCL;
		adc_buffer |= (ADCH << 8); // 서로 다른 레지스터를 16비트로 정렬 하여 사용하기위해서 8bit를 shift 한다.  
			
		voltage = 5.0*(adc_buffer)/1023;


		if( ADMUX & 0x01)
			{
			lcdGotoXY(0,1);
			lcdString("ADC1: ");
			lcdNumber(adc_buffer);
			}

		else 
			{
			lcdGotoXY(0,0);
			lcdString("ADC0: ");
			lcdNumber(adc_buffer);
			}
	

		voltage  = (5.0* (adc_buffer / 1023.0));
		vol = (int)(voltage *100);

		lcdWriteData(' ');
		lcdWriteData((vol / 100) /10 + 0x30);
		lcdWriteData('.');
		lcdWriteData((vol % 100) /10 + 0x30);
		lcdWriteData((vol % 10)+ 0x30);
		lcdWriteData('v');


	}




	return 0 ;



}
