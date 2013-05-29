/*
* LED 20회 on/off


#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
			 		// PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0
	DDRC = 0xFF ;	 //  1   1   1   1   1   1   1   1
	PORTC = 0x00 ; 	// ALL LED OFF
					// PE7 PE6 PE5 PE4 PE3 PE2 PE1 PE0
				
	DDRE = 0x10 ; 		//  0   0   0   1   0   0   0   0
	
	int KEY = 20;

	while(KEY)
	{				//PC7만 ON
		PORTC ^= 0x80;
		_delay_ms(100);
		KEY--;
	}

	return 0; 
}
*/
/*
* LED 차례대로 on/off


#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
			 		// PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0
	DDRC = 0xFF ;	 //  1   1   1   1   1   1   1   1
	PORTC = 0x00 ; 	// ALL LED OFF
					// PE7 PE6 PE5 PE4 PE3 PE2 PE1 PE0
				
	DDRE = 0x10 ; 		//  0   0   0   1   0   0   0   0
	
	PORTC= 0x01;

	while(1)
	{			
		_delay_ms(100);
		PORTC <<= 1;
		
		if(PORTC == 0x00) PORTC = 0X01;
		
	}

	return 0; 
}
*/



#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
			 		// PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0
	DDRC = 0xFF ;	 //  1   1   1   1   1   1   1   1
	PORTC = 0x00 ; 	// ALL LED OFF
					// PE7 PE6 PE5 PE4 PE3 PE2 PE1 PE0
				
	DDRE = 0x10 ; 		//  0   0   0   1   0   0   0   0
	

		while(1)
	{		
	  for( int i =0 ; i<15 ; i++){
	  PORTC <<= 1;
	  	if( i < 7) PORTC |= 0x01; 
		_delay_ms(100);
	  	}
	  for( int i = 0; i< 15; i++){
		PORTC >>=1; 
		if( i<7  ) PORTC |= 0x80;{
		 _delay_ms(100);
		}
	}
}
	return 0; 
}








