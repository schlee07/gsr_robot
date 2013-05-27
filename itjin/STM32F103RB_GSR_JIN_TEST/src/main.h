/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : main.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Header for main.c module
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#define DEV_KIT_LED_TEST // gsr-20130515-ivanjin: Connect GPIO led from port c0 ~ c7.
#define DEV_KIT_STEP_MOTOR_TEST  // gsr-20130520-ivanjin: Timer 3 Test

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "hw_config.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void bsp_init_rcc(void);
void bsp_init_gpio(void);
void bsp_init_interrupt(void);
ErrorStatus Get_HSEStartUpStatus(void);


typedef enum
{
	MainService,
	Ledservice,
	TimerService,
	RtcService,	
#ifdef DEV_KIT_STEP_MOTOR_TEST
    StepMotorService,	
#endif /* DEV_KIT_STEP_MOTOR_TEST */
	serviceMAX
} service_define_type;


typedef enum
{
	ledUserOnService,
	ledUserOffService,
	ledExitService,
	ledServiceMAX
} service_led_type;

typedef enum
{
	timerTIM2Service,
	timerTIM2Stop,		
	timerExitService,
	timerServiceMAX
} service_timer_type;

typedef enum
{
	rtcStartService,
	rtcStopService,
	rtcExitService,	
	rtcServiceMAX
} service_rtc_type;

typedef enum
{
	usbStartService,
	usbStopService,
	usbExitService,	
	usbServiceMAX
} service_usb_type;

#ifdef DEV_KIT_STEP_MOTOR_TEST
typedef enum
{
	stepMotorStartService,
	stepMotorStopService,
    stepMotorCruiseService,
	stepMotorExitService,	
	stepMotorServiceMAX
} service_step_motor_type;
#endif /* DEV_KIT_STEP_MOTOR_TEST */

typedef            void     (*service_function)(void);


typedef struct _service_type
{
	int service;
	char* service_string;
	service_function run;
	s8 cmd;	
} service_type;

void service_led(void);
void service_timer(void);
void service_rtc(void);
void service_usb(void);
#ifdef DEV_KIT_STEP_MOTOR_TEST
void service_step_motor(void);
#endif /* DEV_KIT_STEP_MOTOR_TEST */
void service_user_led_on(void);
void service_user_led_off(void);
void service_led_exit(void);

void service_tim2_ticktime(void);
void service_tim2_stop(void);
void service_timer_exit(void);

void service_rtc_start(void);
void service_rtc_stop(void);
void service_rtc_exit(void);
void wait_10ms(s16 ms_10);


// gsr-20130522-ivanjin: added for additional sevice menu
void service_exit_menu(void);

void tim_display(u32 TimeVar);
void time_show(void);


#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
