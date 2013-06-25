/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"
#include "platform_config.h"

#include "led.h"
#include "main.h"           // gsr-20130515-itjin: include feature
#include "motor_step.h"     // DEV_KIT_STEP_MOTOR_TEST
#include "stm32f10x_dma.h"  // DEV_KIT_ADC_CONV_TEST
#include "sensor.h"         // DEV_KIT_ADC_CONV_TEST    

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void bsp_init_rcc(void)
{  
	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

#if defined( STM32F10X_CL )
		/* Configure PLLs *********************************************************/
		/* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
		RCC_PREDIV2Config(RCC_PREDIV2_Div5);
		RCC_PLL2Config(RCC_PLL2Mul_8);

		/* Enable PLL2 */
		RCC_PLL2Cmd(ENABLE);

		/* Wait till PLL2 is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
		{}

	#if defined (HSE_8MHZ)
		/* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
		RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);		
		RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
	#endif
#else

	#if defined (HSE_12MHZ)
		/* PLLCLK = 12MHz * 6 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
	#elif defined( HSE_16MHZ )
		/* PLLCLK = 16MHz /2 * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);
	#else	 // 8MHZ   
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	#endif
#endif

		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
		}
		else
		{ /* If HSE fails to start-up, the application will have wrong clock configuration.
		   User can add here some code to deal with this error */    

		/* Go to infinite loop */
		while (1)
		{
		}
	}
  
	/* enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE and AFIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

	/* USART1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* TIM1 Periph clock enable */
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	

#ifdef DEV_KIT_STEP_MOTOR_TEST
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* TIM4 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
#endif /* DEV_KIT_STEP_MOTOR_TEST */

#ifdef DEV_KIT_ADC_CONV_TEST
    /* Configure the ADC clock */
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    /* Enable ADC1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Enable DMA1 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);     // DMA1
#endif /* DEV_KIT_ADC_CONV_TEST */

}



void bsp_init_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure the GPIO ports( leds ) */
#ifdef DEV_KIT_LED_TEST
    GPIO_InitStructure.GPIO_Pin = LED_USER_PIN | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
#else
	GPIO_InitStructure.GPIO_Pin =  LED_USER_PIN;
#endif /* DEV_KIT_LED_TEST */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_USER_PORT, &GPIO_InitStructure);	

	/* GPIOA Configuration:TIM2 Channel1, 2, 3 and 4 in Output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

#ifdef DEV_KIT_STEP_MOTOR_TEST
	GPIO_InitStructure.GPIO_Pin =  STEP_MOTOR_ENABLE | STEP_MOTOR_HALF_FULL | STEP_MOTOR_LEFT_DIR | STEP_MOTOR_LEFT_PULSE | STEP_MOTOR_RIGHT_DIR | STEP_MOTOR_RIGHT_PULSE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(STEP_MOTOR_PORT, &GPIO_InitStructure);	

    GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_ENABLE);     // Toque off
#endif /* DEV_KIT_STEP_MOTOR_TEST */

#ifdef DEV_KIT_ADC_CONV_TEST
    /* Port A0. Configure ADC Channel0 as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif /* DEV_KIT_ADC_CONV_TEST */

}

#ifdef DEV_KIT_ADC_CONV_TEST
void bsp_init_adc(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    /* Configure ADC1 to convert continously channel14 */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 3;
    ADC_Init(ADC1, &ADC_InitStructure);
    /* Port A0. ADC1 regular channel0 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);    

#if 1 // RE_CHECK
    /* Enable ADC1's DMA interface */
    ADC_DMACmd(ADC1, ENABLE);
#endif

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 reset calibration register */ 
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));

}

void DMA_Initial(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  /* DMA1 channel1 configuration ----------------------------------------------*/  
  DMA_DeInit(DMA1_Channel1);   
  DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)0x4001244C);  //원래 DR_ADDRESS로 정의 되어 있으나 컴파일 오류가 떠서 다시 정의 해주려다가 그냥 직접 적어줌;;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC1ConvertedValue;   
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;   // ADC의 데이터를 memory로 이동하므로 source로 설정
  DMA_InitStructure.DMA_BufferSize = 3;   // channel 3개 데이터를 저장해야 하므로
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   // DR_ADDRESS는 고정이므로
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;   // ADC1ConvertedValue[0]에서 [2]까지 증가
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   // DR_ADDRESS 크기가 16bit(2byte)이므로
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   // 저장할 변수 크기 역시 동일 16bit(2byte)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;   
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);     
  /* Enable DMA1 channel1 */  
  DMA_Cmd(DMA1_Channel1, ENABLE);
 
   
}

#endif /* DEV_KIT_ADC_CONV_TEST */

void bsp_init_interrupt(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Set the Vector Table base address at 0x08000000 */
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);

	/* 1 bit for pre-emption priority, 3 bits for subpriority */
	/* 101111110100000000000000000 & */
	/* 				   11000000000	 */	
	/* SCB->AIRCR = 101111110100000011000000000 */

#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

	/* 2 bit for pre-emption priority, 2 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);


	/* Enable the USB interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USB Wake-up interrupt */
	// NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_Init(&NVIC_InitStructure);   


	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	


}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
