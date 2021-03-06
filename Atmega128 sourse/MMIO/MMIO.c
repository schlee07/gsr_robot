#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdio.h>


#define FND				(*(volatile unsigned char *)0x8000)

#define DOTMATRIX		(*(volatile unsigned char *)0x9000)

#define LED_SEL1		(*(volatile unsigned char *)0xA000)
#define LED_SEL2		(*(volatile unsigned char *)0xB000)

#define LCD_CMD_WRITE	(*(volatile unsigned char *)0xC000)
#define LCD_CMD_READ	(*(volatile unsigned char *)0xC001)
#define LCD_DATA_WRITE	(*(volatile unsigned char *)0xC002)
#define LCD_DATA_READ	(*(volatile unsigned char *)0xC003)

#define GLCD_CMD		(*(volatile unsigned char *)0xD000)
#define GLCD_DATA		(*(volatile unsigned char *)0xD001)


volatile unsigned char sw = 0;
volatile unsigned int tick0;
volatile char keyflag = 0;
unsigned char sign[10] = {
//  좌우직진   횡단보도적색 노란색      좌회전      노란색
	0xEE, 0xE5, 0xEE, 0xD9, 0xDD, 0xDA, 0x3B, 0xDB, 0xDD, 0xDA
};
volatile char count = 0;
volatile unsigned char segment[6], digit;
volatile char year, month, day, hour, min, sec, tick3;
char monthTable[12] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
volatile unsigned char redBuf[8] = {
	0x00, 0x18, 0x3C, 0x7E, 0x7E, 0x3C, 0x18, 0x00
};
volatile unsigned char greenBuf[8] = {
	11, 12, 13, 14, 15, 16, 17, 18
};
volatile unsigned char greenBuf1[8] = {
	0x00, 0x20, 0x7E, 0xFE, 0x7E, 0x2E, 0x0E, 0x0E
};
volatile unsigned char greenBuf2[8] = {
	0x00, 0x04, 0x7E, 0x7F, 0x7E, 0x74, 0x70, 0x70
};

volatile unsigned char english[128][8]={  {0x00,0x1f,0x64,0x84,0x84,0x64,0x1f,0x00},{0x00,0xff,0x99,0x99,0x99,0x99,0x66,0x00}

};


volatile unsigned char num[9][16]={ 


};


volatile char dig, temp_timer;








void dotMatrix(char line, unsigned char red, unsigned char green);


unsigned char lcdReadCommand(void)
{
	return LCD_CMD_READ;
}

void lcdWriteCommand(unsigned char cmd)
{
//	while( lcdReadCommand() & 0x80 );	// 1000 0000 => 0x80 Busy Check
	_delay_ms(1);

	LCD_CMD_WRITE = cmd;
}

unsigned char lcdReadData(void)
{
	return LCD_DATA_READ;
}

void lcdWriteData(unsigned char data)
{
//	while( lcdReadCommand() & 0x80 );	// 1000 0000 => 0x80 Busy Check
	_delay_ms(1);

	LCD_DATA_WRITE = data;
}

void lcdBacklight(char mode)
{
	if( mode == 0 ) {
		// Backlight Off
		PORTE |= 0x04;
	}
	else {
		// Backlight On
		PORTE &= ~0x04;
	}
}

void lcdInit(void)
{
	DDRE |= 0x04;

	_delay_ms(15);
	lcdWriteCommand(0x38);
	lcdWriteCommand(0x38);
	_delay_ms(5);
	lcdWriteCommand(0x38);
	_delay_us(100);
	lcdWriteCommand(0x08);
	lcdWriteCommand(0x01);
	lcdWriteCommand(0x06);
	lcdWriteCommand(0x0C);
}

void lcdString(char *str)
{
	while( *str )	lcdWriteData(*str++);
}

void lcdGotoXY(char x, char y)
{
	lcdWriteCommand(0x80 | x | (y * 0x40));
}

void lcdNumber(unsigned char num)
{
	lcdWriteData((num / 100) + 0x30);		// 백의자리
	lcdWriteData(((num % 100) / 10) + 0x30);	// 십의자리
	lcdWriteData((num % 10) + 0x30);		// 일의자리
}

ISR(INT4_vect)
{
	if( count == 0 ) {
		count = 4;
		segment[4] = 0;
		segment[5] = 0;
	}
	else	count = (count + 2) % 10;

	tick0 = 0;
}

// Timer0 Overflow Interrupt Handler
// 1ms Timer
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 256 - 125;

	tick0++;

	LED_SEL1 = sign[count];
	LED_SEL2 = sign[count + 1];

	segment[0] = tick0 / 10000;
	segment[1] = tick0 % 10000 / 1000;
	segment[2] = 0x0F;
	segment[3] = 0x0F;
	if( count == 0 ) {
		segment[4] = (25 - (tick0 / 1000)) / 10;
		segment[5] = (25 - (tick0 / 1000)) % 10;
	}
	FND = (segment[digit] << 3) | digit;
	digit = ++digit % 6;

	if( count == 0 ) {
		if( tick0 == 25000 )	count += 2;
	}
	else if( count == 2 ) {
		if( tick0 == 45000 ) {
			count += 2;
			tick0 = 0;
		}
	}
	else if( (count == 4) || (count == 8) ) {
		if( tick0 == 3000 ) {
			count = (count + 2) % 10;
			tick0 = 0;
		}
	}
	else if( count == 6 ) {
		if( tick0 == 45000 ) {
			count += 2;
			tick0 = 0;
		}
	}

	if( count == 0 )	dotMatrix(dig, redBuf[dig], 0x00);
	else if( count == 2 || count == 4 )	dotMatrix(dig, 0x00, greenBuf2[dig]);
	else if( count == 6 || count == 8 )	dotMatrix(dig, 0x00, greenBuf1[dig]);
	dig = ++dig % 8;
}

// Timer3 Overflow Interrupt Handler
// 0,2s Timer
ISR(TIMER3_OVF_vect)
{
	TCNT3 = 65536 - 50000;

	temp_timer++;
	// year, month, day, hour, min, sec, tick3
	if( ++tick3 == 5 ) {
		tick3 = 0;
		if( ++sec == 60 ) {
			sec = 0;
			if( ++min == 60 ) {
				min = 0;
				if( ++hour == 24 ) {
					hour = 0;
					if( ++day == monthTable[month] ) {
						day = 0;
						if( ++month == 12 ) {
							month = 0;
							year++;
						}
					}
				}
			}
		}
	}
}

unsigned char getkey(unsigned char keyin)
{
	unsigned char key, ret = 0;

	keyflag = 0;

	// 1열 검사
	PORTD |= 0xF0;
	PORTD &= 0xEF;
	_delay_ms(1);
	key = ~PINF & 0xF0;
	if( key == 0x10 )		ret = 1;
	else if( key == 0x20 )	ret = 2;
	else if( key == 0x40 )	ret = 3;
	else if( key == 0x80 )	ret = 4;

	// 2열 검사
	PORTD |= 0xF0;
	PORTD &= 0xDF;
	_delay_ms(1);
	key = ~PINF & 0xF0;
	if( key == 0x10 )		ret = 5;
	else if( key == 0x20 )	ret = 6;
	else if( key == 0x40 )	ret = 7;
	else if( key == 0x80 )	ret = 8;

	// 3열 검사
	PORTD |= 0xF0;
	PORTD &= 0xBF;
	_delay_ms(1);
	key = ~PINF & 0xF0;
	if( key == 0x10 )		ret = 9;
	else if( key == 0x20 )	ret = 10;
	else if( key == 0x40 )	ret = 11;
	else if( key == 0x80 )	ret = 12;

	// 4열 검사
	PORTD |= 0xF0;
	PORTD &= 0x7F;
	_delay_ms(1);
	key = ~PINF & 0xF0;
	if( key == 0x10 )		ret = 13;
	else if( key == 0x20 )	ret = 14;
	else if( key == 0x40 )	ret = 15;
	else if( key == 0x80 )	ret = 16;

	if( ret && (ret != keyin) )	keyflag = 1;

	return ret;
}

void displayWatch(void)
{
	// 시계 출력
	lcdGotoXY(0, 0);
	printf("   20%02d/%02d/%02d   ", year, month + 1, day + 1);
	lcdGotoXY(0, 1);
	if( hour >= 1 && hour <= 12 ) {
		printf("   AM %02d:%02d:%02d  ", hour, min, sec);
	}
	else {
		char hour1 = hour % 12;
		if( hour1 == 0 )	hour1 = 12;
		printf("   PM %02d:%02d:%02d", hour1, min, sec);
	}
}

// 8개중 1개의 Line에 적색 및 녹색 8비트 데이타 출력
void dotMatrix(char line, unsigned char red, unsigned char green)
{
	unsigned char data;

	DOTMATRIX = 0;	// 도트매트릭스의 모든 핀 disable

	for( int i = 0; i < 8; i++ ) {
		data = line << 4;
		// 적색
		if( red & 0x80 )	data |= 0x01;
		// 녹색
		if( green & 0x80 )	data |= 0x02;

		DOTMATRIX = data;
		data |= 0x04;
		DOTMATRIX = data;
		data &= ~0x04;
		DOTMATRIX = data;

		red <<= 1;
		green <<= 1;
	}
	data |= 0x88;
	DOTMATRIX = data;
	data &= ~0x08;
	DOTMATRIX = data;
}

unsigned char glcdReadCommand(void)
{
	return GLCD_CMD;
}

unsigned char glcdReadData(void)
{
	return GLCD_DATA;
}

void glcdWriteCommand(unsigned char cmd)
{
	while( glcdReadCommand() & 0x80 );

	GLCD_CMD = cmd;
}

void glcdWriteData(unsigned char data)
{
	while( glcdReadCommand() & 0x80 );

	GLCD_DATA = data;
}

void glcdBacklight(char mode)
{
	if( mode == 0 ) {
		// Backlight Off
		PORTE |= 0x08;
	}
	else {
		// Backlight On
		PORTE &= ~0x08;
	}
}

void glcdGotoXY(unsigned char x, unsigned char y)
{
	glcdWriteCommand(0xB0 | (7 - y));
	glcdWriteCommand(0x10 | x >> 4);
	glcdWriteCommand(0x00 | (x & 0x0f));
}

void glcdClear(void)
{

	unsigned char x, y, i;

	for( y = 0; y < 8; y++){
		glcdGotoXY(0, y);

		for(x = 0; x < 128; x++){
			glcdWriteData(0x00);
		}
	}
	
}

void glcdInit(void)
{
	DDRE |= 0x08;

	glcdBacklight(1);	// Back Light ON

	_delay_ms(500);
	glcdWriteCommand(0xAB);		//New
	glcdWriteCommand(0xAF);		//Display on
	glcdWriteCommand(0x40);		//Start line 0
	glcdWriteCommand(0xB0);		//Page address set 0
	glcdWriteCommand(0x10);		//Column address H 0
	glcdWriteCommand(0x00);		//Column address L 0
	glcdWriteCommand(0xC0);		//Common output mode select = Normal
	glcdWriteCommand(0xA6);		//Normal display
	glcdWriteCommand(0xA0);		//ADC set = Normal
	glcdWriteCommand(0xA4);		//Display all point off
	glcdWriteCommand(0xA3);		//LCD bias 1/9
	glcdWriteCommand(0x2F);		//Power control all on
	glcdWriteCommand(0x21);		//Rab ratio 26
	glcdWriteCommand(0x81);		//E-Vol setting
	glcdWriteCommand(0x1f);		//Contrast Level
}



int main(void)
{
	unsigned char key, dayflag = 0, watchflag = 0, tempflag = 0;
	unsigned int adc_buffer;

	// 외부메모리 설정
	MCUCR = 0x80;
	XMCRA = 0x4A;

	// 외부인터럽트 4 폴링엣지
	EICRA = 0x00;
	EICRB = 0x02;
	EIMSK = 0x10;

	// Timer0 초기화
	TCCR0 = 0x05;		// 128분주 : 16MHz / 128 = 125KHz(8us)
	TCNT0 = 256 - 125;	// 8us * 125 = 1ms, 타이머인터럽트 발생 주기
	TIMSK = 0x01;		// Timer0 오버플로우 인터럽트 인에이블

	// OC2(PB7) output
	DDRB |= 0x60;  //0x80

	// Timer2 : Fast PWM mode, 256분주
	TCCR2 = 0x7C;
	TCNT2 = 0;
	OCR2 = 0;

	// Timer3 초기화
	TCCR3B = 0x03;			// 64분주 : 16MHz / 64 = 250KHz(4us)
	TCNT3 = 65536 - 50000;	// 4us * 50000 = 0.2sec
	ETIMSK = 0x04;			// 오버플로우 인터럽트 인에이블

	sei();

	// ADC 초기화
	ADMUX = 2;			// 2번 채널
	ADMUX |= 0x40;		// 기준 전압을 AVCC로, 좌측정렬
	ADCSR = 0x86;		// ADC 인에이블, 64분주

	DDRD |= 0xF0;
	DDRF &= 0x0F;

	lcdInit();
	glcdInit();
	glcdClear();
	
	for(int i =0 ; i<8 ; i++)
	{
		for(int j=0 ; j<8 ; j++)
		{
		glcdGotoXY(j, 0);
		glcdWriteData(english[0][j+1]);
		}
	}
	/*
		

			//B
			glcdGotoXY(9, 0);
			glcdWriteData(0x00);
			glcdGotoXY(10, 0);
			glcdWriteData(0xff);
			glcdGotoXY(11, 0);
			glcdWriteData(0x99);
			glcdGotoXY(12, 0);
			glcdWriteData(0x99);
			glcdGotoXY(13, 0);
			glcdWriteData(0x99);
			glcdGotoXY(14, 0);
			glcdWriteData(0x99);
			glcdGotoXY(15, 0);
			glcdWriteData(0x66);
			glcdGotoXY(16, 0);
			glcdWriteData(0x00);
			
			//C
			glcdGotoXY(17, 0);
			glcdWriteData(0x00);
			glcdGotoXY(18, 0);
			glcdWriteData(0x3C);
			glcdGotoXY(19, 0);
			glcdWriteData(0x42);
			glcdGotoXY(20, 0);
			glcdWriteData(0x81);
			glcdGotoXY(21, 0);
			glcdWriteData(0x81);
			glcdGotoXY(22, 0);
			glcdWriteData(0x81);
			glcdGotoXY(23, 0);
			glcdWriteData(0x81);
			glcdGotoXY(24, 0);
			glcdWriteData(0x00);
	
			//D
			glcdGotoXY(17, 0);
			glcdWriteData(0x00);
			glcdGotoXY(18, 0);
			glcdWriteData(0x3C);
			glcdGotoXY(19, 0);
			glcdWriteData(0x42);
			glcdGotoXY(20, 0);
			glcdWriteData(0x81);
			glcdGotoXY(21, 0);
			glcdWriteData(0x81);
			glcdGotoXY(22, 0);
			glcdWriteData(0x81);
			glcdGotoXY(23, 0);
			glcdWriteData(0x81);
			glcdGotoXY(24, 0);
			glcdWriteData(0x00);
*/
	




	// printf 사용을 위한 선언(LCD)
	fdevopen((void *)lcdWriteData, 0);

	lcdBacklight(1);	// Back Light On

	while( 1 ) {
		key = getkey(key);
		if( keyflag ) {
			switch( key ) {
				case 1 :
					if( !watchflag )	dayflag ^= 0x01;
					break;

				case 2:
					if( !dayflag )		watchflag ^= 0x01;
					break;

				case 5:
					if( dayflag ) {
						if( year )	year--;
						else		year = 99;
					}
					if( watchflag ) {
						if( hour )	hour--;
						else		hour = 23;
					}
					break;

				case 6:
					if( dayflag ) {
						if( year < 99 )	year++;
						else			year = 0;
					}
					if( watchflag ) {
						if( hour < 23 )	hour++;
						else			hour = 0;
					}
					break;

				case 9:
					if( dayflag ) {
						if( month )	month--;
						else		month = 11;
					}
					if( watchflag ) {
						if( min )	min--;
						else		min = 59;
					}
					break;

				case 10:
					if( dayflag ) {
						if( month < 11 )	month++;
						else				month = 0;
					}
					if( watchflag ) {
						if( min < 59 )	min++;
						else			min = 0;
					}
					break;

				case 13:
					if( dayflag ) {
						if( day )	day--;
						else		day = monthTable[month] - 1;
					}
					break;

				case 14:
					if( dayflag ) {
						if( day < monthTable[month] - 1 )	day++;
						else								day = 0;
					}
					break;

				case 4 :
					tempflag ^= 0x01;
					temp_timer = 0;
					break;
			}
		}

		if( tempflag ) {
			if( temp_timer > 25 )	tempflag = 0;
			lcdGotoXY(0, 0);
			printf("  Temperature   ");
			lcdGotoXY(0, 1);
			printf("     %05.1f      ", 100.0 * (adc_buffer / 1023.0));
		}
		else	displayWatch();

		ADMUX = 2;
		ADCSR |= 0x40;	// ADC 변환 시작
		while( !(ADCSR & (1 << ADIF)) );
		adc_buffer = ADC;

		ADMUX = 1;
		ADCSR |= 0x40;	// ADC 변환 시작
		while( !(ADCSR & (1 << ADIF)) );
		OCR2 = ADC >> 2;
	}

	return 0;
}
