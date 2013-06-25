
#include "main.h"
#include "stm32f10x.h"
#include "hw_config.h"
#include "timer.h"
#include "motor_step.h" // DEV_KIT_STEP_MOTOR_TEST

timer_service_function_type gbl_ar_timer_service[timerServiceFunctionMAX] = 
{
	{timer1ServiceFunction, NULL},
	{timer2ServiceFunction, NULL},
	{timer3ServiceFunction, NULL},	
    {timer4ServiceFunction, NULL}

};
 
/* ------------------------------------------------------------------------------------------------- */
/* BSP Timer */
/* ------------------------------------------------------------------------------------------------- */

void register_timer_function(timer_register_function_type timer_fn_type, timer_register_function fn)
{
	gbl_ar_timer_service[timer_fn_type].run = fn;
}

void TIM2_IRQHandler(void)
{
	// Also cleared the wrong interrupt flag in the ISR
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	// TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // Clear the interrupt flag

	if( gbl_ar_timer_service[timer2ServiceFunction].run != NULL )
	{
		gbl_ar_timer_service[timer2ServiceFunction].run();
	}
}

void bsp_init_timer2(void/*isr_function timer2_isr*/)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


	/* Enable the TIM4 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		

	// TIM4CLK = 72 MHz( APB1은 원래 36MHz 인데 분주를 해서 사용하므로 36*2 = 72MHz 가 된다. )
	// 시간의  기본단위 :S(초)-->nS.uS.mS.S.
	// 1Mhz / 72Mhz = 0.0138uS, 
	// 72/(71+1) = 1(uS) * 10000 = 10mS 마다 인터럽트 발생	

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10000;	// ARR(Auto reload register)
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);	

	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE);


	
}


u16 bsp_get_timer2_cnt(void)
{
	return TIM2->CNT;
}

void bsp_set_timer2_cnt( u16 cnt)
{
	TIM2->CNT = cnt;
}

#ifdef DEV_KIT_STEP_MOTOR_TEST
void TIM3_IRQHandler(void)
{
	// Also cleared the wrong interrupt flag in the ISR
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	// TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // Clear the interrupt flag

	if( gbl_ar_timer_service[timer3ServiceFunction].run != NULL )
	{
		gbl_ar_timer_service[timer3ServiceFunction].run();
	}
}

void bsp_init_timer3(FunctionalState TimerStart)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


	/* Enable the TIM4 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// TIM4CLK = 72 MHz( APB1은 원래 36MHz 인데 분주를 해서 사용하므로 36*2 = 72MHz 가 된다. )
	// 시간의  기본단위 :S(초)-->nS.uS.mS.S.
	// 1Mhz / 72Mhz = 0.0138uS, 
	// 72/(71+1) = 1(uS) * 10000 = 10mS 마다 인터럽트 발생	

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1000;// 10000;	// ARR(Auto reload register)
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

    if(TimerStart == ENABLE)
    {
    	TIM_Cmd(TIM3, ENABLE);
    	/* TIM IT enable */
    	TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE);
	}
}

void TIM4_IRQHandler(void)
{
	// Also cleared the wrong interrupt flag in the ISR
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	// TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // Clear the interrupt flag

	if( gbl_ar_timer_service[timer4ServiceFunction].run != NULL )
	{
		gbl_ar_timer_service[timer4ServiceFunction].run();
	}
}

void bsp_init_timer4(FunctionalState TimerStart)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


	/* Enable the TIM4 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// TIM4CLK = 72 MHz( APB1은 원래 36MHz 인데 분주를 해서 사용하므로 36*2 = 72MHz 가 된다. )
	// 시간의  기본단위 :S(초)-->nS.uS.mS.S.
	// 1Mhz / 72Mhz = 0.0138uS, 
	// 72/(71+1) = 1(uS) * 10000 = 10mS 마다 인터럽트 발생	

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1000;// 10000;	// ARR(Auto reload register)
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM4, ENABLE);

    if(TimerStart == ENABLE)
    {
    	TIM_Cmd(TIM4, ENABLE);
    	/* TIM IT enable */
    	TIM_ITConfig(TIM4, TIM_IT_Update , ENABLE);
	}
}

void bsp_motor_timer_interrupt(FunctionalState TimerStart)
{
    TIM_Cmd(TIM3, TimerStart);  // Left motor timer start
    TIM_Cmd(TIM4, TimerStart);  // Right motor timer start  
    /* TIM IT enable */
    TIM_ITConfig(TIM3, TIM_IT_Update , TimerStart);
    TIM_ITConfig(TIM4, TIM_IT_Update , TimerStart);    
}

#endif /* DEV_KIT_STEP_MOTOR_TEST */


