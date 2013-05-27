/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Joystick Mouse demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/*
Predefine 설정 
64P
STM32F103RBT6(128KB, 32KB) :  USE_STDPERIPH_DRIVER, STM32F10X_MD
STM32F103R8T6(128KB, 20KB) :  USE_STDPERIPH_DRIVER, STM32F10X_MD
STM32F105RBT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107RBT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL

100p
STM32F103VCT6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_HD
STM32F105VCT6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VBT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VCT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VBT6(128KB, 32KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL

144p
STM32F103ZET6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_HD
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"
#include "main.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "rtc.h"
#include "motor_step.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


// setting menu tree.
static service_define_type s_menu_level = MainService;

#ifdef DEV_KIT_STEP_MOTOR_TEST
#define MAX_SUB_SERVICE	5
#else
#define MAX_SUB_SERVICE	4
#endif /* DEV_KIT_STEP_MOTOR_TEST */

service_type gbl_service[serviceMAX][MAX_SUB_SERVICE] = 
{
	{
		{Ledservice, "[1] LED testggg\r\n", service_led, '1'},
		{TimerService, "[2] TIMER test\r\n", service_timer, '2'},	
		{RtcService, "[3] RTC test\r\n", service_rtc, '3'},
#ifdef DEV_KIT_STEP_MOTOR_TEST
         {StepMotorService, "[4] Step Motr test\r\n", service_step_motor, '4'},
#endif /* DEV_KIT_STEP_MOTOR_TEST */
		{serviceMAX, 0x00, 0x00, 0x00}	
	},
	{
		{ledUserOnService, "[1] User led on\r\n", service_user_led_on, '1'},
		{ledUserOffService, "[2] User led off\r\n", service_user_led_off, '2'},
		{ledExitService, "[x] Exit led test\r\n", service_led_exit, 'x'},
		{ledServiceMAX, 0x00, 0x00, 0x00}
#ifdef DEV_KIT_STEP_MOTOR_TEST
        ,{ledServiceMAX, 0x00, 0x00, 0x00}
#endif /* DEV_KIT_STEP_MOTOR_TEST */		
	},	
	{
		{timerTIM2Service, "[1] TIM2 ticktime test\r\n", service_tim2_ticktime, '1'},
		{timerTIM2Stop, "[2] TIM2 blink stop\r\n", service_tim2_stop, '2'},
		{timerExitService, "[x] Exit timer test\r\n", service_timer_exit, 'x'},			
		{timerServiceMAX, 0x00, 0x00, 0x00}	
#ifdef DEV_KIT_STEP_MOTOR_TEST
        ,{timerServiceMAX, 0x00, 0x00, 0x00}
#endif /* DEV_KIT_STEP_MOTOR_TEST */		
	},	
	{
		{rtcStartService, "[1] RTC service start\r\n", service_rtc_start, '1'},
		{rtcStopService, "[2] RTC service stop\r\n", service_rtc_stop, '2'},
		{rtcExitService, "[x] Exit RTC test\r\n", service_rtc_exit, 'x'},					
		{rtcServiceMAX, 0x00, 0x00, 0x00}
#ifdef DEV_KIT_STEP_MOTOR_TEST
        ,{rtcServiceMAX, 0x00, 0x00, 0x00}
#endif /* DEV_KIT_STEP_MOTOR_TEST */		
	},
#ifdef DEV_KIT_STEP_MOTOR_TEST
	{
		{stepMotorStartService, "[1] Step Motor start\r\n", service_step_motor_start, '1'},
		{stepMotorStopService, "[2] Step Motor stop\r\n", service_step_motor_stop, '2'},
        {stepMotorCruiseService, "[3] Step Motor cruise test\r\n", service_step_motor_cruise_test, '3'},
		{stepMotorExitService, "[x] Exit Step Motor test\r\n", service_step_motor_exit, 'x'},					
		{stepMotorServiceMAX, 0x00, 0x00, 0x00}	
	},	
#endif /* DEV_KIT_STEP_MOTOR_TEST */
		
};


static volatile s8 s_blink_service = 0;
static volatile s8 s_rtc_service = 0;
static volatile s16 s_blink_cnt = 0;
static volatile u16 s_wait_cnt = 0;
vu32 time_display = 0;
#if 1 // test
static volatile u16 s_delay_cnt = 0;
#endif
#ifdef DEV_KIT_LED_TEST
static volatile u16 s_satus_led_flag = 0;
#endif /* DEV_KIT_LED_TEST */

void wait_10ms(s16 ms_10)
{
	s_wait_cnt = 0;
	while( s_wait_cnt < ms_10 );
}

#if 1 // test
void wait_1ms(s16 ms_10)
{
	s_delay_cnt = 100;
    do
    {
        s_delay_cnt = 100;
        while(--s_delay_cnt);
    }while(--ms_10);
}
#endif

void timer2_event(void)
{
	if( s_wait_cnt++ == 0xffff )
		s_wait_cnt = 0;

	if( s_blink_cnt++ > 10 )
	{
		s_blink_cnt = 0;		
		if( s_blink_service )
		{
			bsp_led_toggle(ledUser);
		}
	}

#ifdef DEV_KIT_LED_TEST
    if(s_satus_led_flag)
    {
        if((s_wait_cnt % 10) == 0)
        {
            led_status_test();
        }
    }
	
#endif /* DEV_KIT_LED_TEST */
}

void rtc_event(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear the RTC Second interrupt */
		RTC_ClearITPendingBit(RTC_IT_SEC);

		/* Toggle led connected to PC.06 pin each 1s */
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_6)));

		/* Enable time update */
		if( s_rtc_service )
		{
			time_show();
		}

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Reset RTC Counter when Time is 23:59:59 */
		if(RTC_GetCounter() == 0x00015180)
		{
			RTC_SetCounter(0x0);
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
		}
	}
}

void welcome(void)
{
	usart1_transmit_string("\r\n*****************************************************************************\r\n");
	usart1_transmit_string("GSR 2013.\r\n");

#ifdef STM32F10X_CL
	// STM32F105, 107 에서는 PLL설정이 약간 틀리다. 
	// 이후에 PLL설정 스트링을 수정해야 한다.
	usart1_transmit_string("HSE(High Speed External clock signal) Enable\r\n");
	usart1_transmit_string("HCLK(SYSCLK) = 8MHz, PCLK2 = HCLK, PCLK1 = HCLK/2, ADCCLK = PCLK2/2\r\n");
	usart1_transmit_string("PLLCLK = 8MHz / 1 * 9 = 72MHz, USBCLK = PLLCLK / 2 = 48MHz\r\n\r\n");
#else
	usart1_transmit_string("HSE(High Speed External clock signal) Enable\r\n");
	usart1_transmit_string("HCLK(SYSCLK) = 8MHz, PCLK2 = HCLK, PCLK1 = HCLK/2, ADCCLK = PCLK2/2\r\n");
	usart1_transmit_string("PLLCLK = 8MHz / 1 * 9 = 72MHz, USBCLK = PLLCLK / 2 = 48MHz\r\n");
#endif

}

void display_menu()
{
	int i;

	for(i=0;i<serviceMAX;i++)
	{
		if( gbl_service[s_menu_level][i].run == 0x00 )
			break;

		// wait(5);
		usart1_transmit_string(gbl_service[s_menu_level][i].service_string);
		//strcpy(gbl_zz, gbl_service[s_menu_level][i].service_string);
		//strcpy(zz, gbl_service[s_menu_level][i].service_string);
		
	}
	
	usart1_transmit_string("\r\n\r\nSelect menu ? ");
	
}


int run_menu_selection(void)
{
	int i;
	char* pdata;
	s8 data;

	pdata = (char*)usart1_get_data();

	if( pdata == 0x00 )
		return 0x00;

	data = pdata[0];


	for(i=0;i<MAX_SUB_SERVICE;i++)
	{
		if( gbl_service[s_menu_level][i].run == 0x00 )
			break;

		if( gbl_service[s_menu_level][i].cmd == data )
		{
			gbl_service[s_menu_level][i].run();
			return 1;
		}
	}

	return 0;
	
}

/*******************************************************************************
* Function Name  : main.
* Description    : main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{

	/* Initialize the Demo */
	bsp_init_rcc();

	bsp_init_gpio();
	bsp_init_interrupt();
#ifdef DEV_KIT_STEP_MOTOR_TEST
    step_motor_enable(DISABLE);
#endif /* DEV_KIT_STEP_MOTOR_TEST */

	register_timer_function(timer2ServiceFunction, timer2_event);
#ifdef DEV_KIT_STEP_MOTOR_TEST
    register_timer_function(timer3ServiceFunction, isr_motor_event);    
#endif /* DEV_KIT_STEP_MOTOR_TEST */
	register_rtc_function(rtcServiceFunction, rtc_event);

	bsp_init_timer2();
#ifdef DEV_KIT_STEP_MOTOR_TEST
    bsp_init_timer3(DISABLE);
#endif /* DEV_KIT_STEP_MOTOR_TEST */

	bsp_init_irq_usart1();
	//bsp_init_dma_usart1();

	// 10msec
	wait_10ms(1);

	// User LED ON
	bsp_led_on(ledUser);
#ifdef DEV_KIT_LED_TEST
    led_rotate_test();
    s_satus_led_flag = 1; // Go status led
#endif /* DEV_KIT_LED_TEST */
	welcome();
	
	usart1_transmit_string("\r\n*****************************************************************************\r\n");	
	usart1_transmit_string("User led ( on )\r\n");
	usart1_transmit_string("Initialize gpio service.\r\n");
	usart1_transmit_string("Start USART1 service on mode interrupt.\r\n");
	usart1_transmit_string_format("BaudRate = %d, Databit = %dbit, StopBits = %d, Parity = no, FlowControl = none\r\n", 115200, 8, 1);
	usart1_transmit_string("\r\n*****************************************************************************\r\n");		

	display_menu();

	usart1_tx_proc();


#if 0 //#ifdef DEV_KIT_LED_TEST
//    GPIO_SetBits(GPIOC, GPIO_Pin_0)
    GPIO_ResetBits(GPIOC, GPIO_Pin_0);
    GPIO_ResetBits(GPIOC, GPIO_Pin_1);
    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);    
    GPIO_ResetBits(GPIOC, GPIO_Pin_4);
    GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);
#endif /* DEV_KIT_LED_TEST */
	
	while( 1 )
	{
		if( run_menu_selection() != 0 )
			display_menu();

#ifdef DEV_KIT_STEP_MOTOR_TEST
        step_motor_start(ROBOT_GO_FORWARD);    
#endif /* DEV_KIT_STEP_MOTOR_TEST */
	}

}



void service_led(void)
{
	s_menu_level = Ledservice;
	usart1_transmit_string("\r\nservice_led()\r\n");
}

void service_timer(void)
{
	s_menu_level = TimerService;
	usart1_transmit_string("\r\nservice_timer()\r\n");	
}

void service_rtc(void)
{
	s_menu_level = RtcService;
	usart1_transmit_string("\r\nservice_rtc()\r\n");	
}

#ifdef DEV_KIT_STEP_MOTOR_TEST
void service_step_motor(void)
{
	s_menu_level = StepMotorService;
	usart1_transmit_string("\r\nservice_step_motor\r\n");	
}
#endif /* DEV_KIT_STEP_MOTOR_TEST */

void service_user_led_on(void)
{
	bsp_led_on(ledUser);
	usart1_transmit_string("\r\nservice_user_led_on()\r\n");
}

void service_user_led_off(void)
{
	bsp_led_off(ledUser);
	usart1_transmit_string("\r\nservice_user_led_off()\r\n");
}


void service_led_exit(void)
{
	s_menu_level = MainService;
	usart1_transmit_string("\r\nservice_led_exit()\r\n");
}


void service_tim2_ticktime(void)
{
	usart1_transmit_string("\r\nservice_tim2_ticktime()\r\n");	
	usart1_transmit_string("Start timer2 10msec period.\r\n");
	usart1_transmit_string("User led blink start 1 second period\r\n");

	s_blink_service = 1;
}

void service_tim2_stop(void)
{
	usart1_transmit_string("\r\nservice_tim2_stop()\r\n");	
	usart1_transmit_string("User led blink stop.\r\n");

	s_blink_service = 0;
}


void service_timer_exit(void)
{
	s_menu_level = MainService;
	usart1_transmit_string("\r\nsservice_timer_exit()\r\n");
}

void service_rtc_start(void)
{

	usart1_transmit_string("\r\nservice_rtc_start()\r\n");	

	bsp_init_rtc();
	
	usart1_transmit_string("Start RTC Service to uart.\r\n");

	time_show();

	s_rtc_service = 1;
}

void service_rtc_stop(void)
{
	usart1_transmit_string("\r\nservice_rtc_stop()\r\n");	
	usart1_transmit_string("Stop RTC Service to uart.\r\n");

	s_rtc_service = 0;
}

void service_rtc_exit(void)
{
	s_menu_level = MainService;
	usart1_transmit_string("\r\nservice_rtc_exit()\r\n");
}

// gsr-20130522-ivanjin: added for additional sevice menu
void service_exit_menu(void)
{
	s_menu_level = MainService;
}


/*******************************************************************************
* Function Name  : tim_display
* Description    : Displays the current time.
* Input          : - TimeVar: RTC counter value.
* Output         : None
* Return         : None
*******************************************************************************/
void tim_display(u32 TimeVar)
{ 
	u32 THH = 0, TMM = 0, TSS = 0;

	/* Compute  hours */
	THH = TimeVar/3600;
	/* Compute minutes */
	TMM = (TimeVar % 3600)/60;
	/* Compute seconds */
	TSS = (TimeVar % 3600)% 60;

	usart1_transmit_string_format("\r\nTime: %0.2d:%0.2d:%0.2d",THH, TMM, TSS);
}

/*******************************************************************************
* Function Name  : Time_Show
* Description    : Shows the current time (HH:MM:SS) on the Hyperterminal.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void time_show(void)
{
	// usart1_transmit_string("\n\r");
	tim_display(RTC_GetCounter());

#if 0	

	/* Infinite loop */ 
	while(1)
	{
		/* If 1s has paased */
		if(time_display == 1)
		{    
			/* Display current time */
			tim_display(RTC_GetCounter());
			time_display = 0;
		}
	}
#endif
}


#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
