
#include "main.h"
#include "stm32f10x.h"
#include "hw_config.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "motor_step.h"
#include "motor_step_table.h"


u16 g_step_motor_count = 0;
u16 g_isr_step_motor_10ms_cnt = 0;
static step_motor_act_type s_motor_action = STEP_MOTOR_Start;

// gsr-20130522-ivanjin: Timer interrupt 기본적으로 10ms 한번씩 뜨도록 설정됨.
void isr_motor_event(void) // gsr-20130521-ivanjin: ISR is called in every 10ms.
{
    u16 motor_table_max = sizeof(g_step_motor_intrv_time) / 2;
    static u16 array_cnt = 0;
    
	if( g_isr_step_motor_10ms_cnt++ > g_step_motor_intrv_time[array_cnt] )
	{
		g_isr_step_motor_10ms_cnt = 0;

		
        // 1 pulse
		bsp_led_off(ledUser);
		// wait minimum 0.5us
		wait_1ms(10000);
		bsp_led_on(ledUser);
		
        if(s_motor_action == STEP_MOTOR_Start)
        {
            g_step_motor_count++;
            
    		if(array_cnt < motor_table_max)
    		{
                array_cnt++;
    		}
        }
        else if(s_motor_action == STEP_MOTOR_Stop)
        {
            g_step_motor_count = array_cnt;
            if(array_cnt > 0)
            {
                array_cnt--;
            }
            
        }
        else
        {
            array_cnt = 0;
        }
	}
}

void step_motor_direction(step_motor_dir_type l_motor_dir, step_motor_dir_type r_motor_dir)
{
    
	if((l_motor_dir >= MOTOR_DIR_MAX) || (r_motor_dir >= MOTOR_DIR_MAX) )
		return;

    // Left motor direction
    if(l_motor_dir == MOTOR_DIR_CCW)
    	GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_LEFT_DIR);
    else
       	GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_LEFT_DIR);            

    // Right motor direction
    if(r_motor_dir == MOTOR_DIR_CCW)
    	GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_RIGHT_DIR);
    else
        GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_RIGHT_DIR);
}

void step_motor_enable(FunctionalState chip_sel)
{
    if(chip_sel == ENABLE)
    	GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_ENABLE);
    else
        GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_ENABLE);
}

void step_motor_start(step_motor_robot_dir_type robot_direction)
{
    if(robot_direction >= ROBOT_GO_MAX)
        return;

    // Motor Direction
    switch(robot_direction)
    {
        case ROBOT_GO_FORWARD:
            step_motor_direction(MOTOR_DIR_CCW, MOTOR_DIR_CW);
            break;
            
        case ROBOT_GO_BACKWARD:
            step_motor_direction(MOTOR_DIR_CW, MOTOR_DIR_CCW);
            break;

        case ROBOT_TURN_LEFT:
            step_motor_direction(MOTOR_DIR_CCW, MOTOR_DIR_CCW);
            break;
            
        case ROBOT_TURN_RIGHT:
        default:
            step_motor_direction(MOTOR_DIR_CW, MOTOR_DIR_CW);        
            break;
    }
    
    s_motor_action = STEP_MOTOR_Start;
    g_step_motor_count = 0; // step counter init
    
    // Motor Enable, L297 enable
    step_motor_enable(ENABLE);
    // Start Timer Interrupt
    bsp_motor_timer_interrupt(ENABLE);
}

void step_motor_stop(void)
{
    s_motor_action = STEP_MOTOR_Start;
    while(g_step_motor_count > 0);

    wait_10ms(50);  // Wait 500ms
    step_motor_enable(DISABLE);    
    bsp_motor_timer_interrupt(DISABLE);
}


void service_step_motor_start(void)
{
    step_motor_start(ROBOT_GO_FORWARD);
    usart1_transmit_string("\r\nservice_step_motor_start()\r\n");
    
}

void service_step_motor_stop(void)
{
    step_motor_stop();
    usart1_transmit_string("\r\nservice_step_motor_stop()\r\n");
}

void service_step_motor_cruise_test(void)
{
    
    usart1_transmit_string("\r\nservice_step_motor_cruise_test()\r\n");
}

void service_step_motor_exit(void)
{
    service_exit_menu();
    usart1_transmit_string("\r\nservice_step_motor_exit()\r\n");
}


