
#include "main.h"
#include "stm32f10x.h"
#include "hw_config.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "motor_step.h"
#include "motor_step_table.h"
#include "sensor.h"

step_motor_set_type g_left_mot={STEP_MOTOR_DIR_CCW, STEP_MOTOR_ANGLE_FULL,0}, g_right_mot={STEP_MOTOR_DIR_CCW, STEP_MOTOR_ANGLE_FULL,0};

void isr_left_motor_event(void) // gsr-20130521-itjin: ISR is called in motor time table
{
    u16 array_count = 0, next_interrupt_time = 0;
    static u8 s_low_pulse_flag = 0;

    if(s_low_pulse_flag == 0)
    {
        s_low_pulse_flag = 1; 
        // LOW motor pulse
        GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_LEFT_PULSE);
        TIM_SetAutoreload(TIM3, STEP_MOTOR_Pulse_Low_Delay);  // 100us low, L297 need minimum 0.5us
    }
    else
    {
        // Change flag for next low signal.
        s_low_pulse_flag = 0;
        
        // HIGH motor pulse
        GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_LEFT_PULSE);

        if(g_left_mot.step_count <= g_left_mot.max_speed) // Acceleration
        {
            array_count = g_left_mot.step_count;
        }
        else if((g_left_mot.max_speed < g_left_mot.step_count) && (g_left_mot.step_count < g_left_mot.constant_distance)) // Constant velocity
        {
            if(g_left_mot.smooth_turn_flag == DISABLE)
            {
                array_count = g_left_mot.max_speed;
            }
            else    // TODO: Change more simple. smooth turn code
            {
                if(g_left_mot.smooth_speed_down_max_table < g_left_mot.max_speed - g_left_mot.smooth_step_count)
                {
                    array_count = g_left_mot.max_speed - g_left_mot.smooth_step_count;
                }
                else if(g_left_mot.smooth_step_count < g_left_mot.smooth_constant_dist)
                {
                    array_count = g_left_mot.smooth_speed_down_max_table;
                }
                else if(g_left_mot.smooth_step_count < g_left_mot.smooth_target_dist)
                {
                    array_count = g_left_mot.smooth_speed_down_max_table + (g_left_mot.smooth_step_count - g_left_mot.smooth_constant_dist);
                }
                g_left_mot.smooth_step_count++;
            }

        }
        else if(g_left_mot.step_count >= g_left_mot.constant_distance)   // Deceleration
        {
            if(g_left_mot.target_distance >= g_left_mot.step_count)
            {
                array_count = g_left_mot.target_distance - g_left_mot.step_count;
            }
            else
            {
                array_count = 0;
            }
        }

        // Except low puls time
        next_interrupt_time = g_motor_time_table[array_count] - STEP_MOTOR_Pulse_Low_Delay;
        // Add adjustment time for robot position.
        next_interrupt_time += g_left_mot.adjustment_time;
        
        TIM_SetAutoreload(TIM3, next_interrupt_time);

        // Increase step
        g_left_mot.step_count++;
    }
}

void isr_right_motor_event(void) // gsr-20130521-itjin: ISR is called in motor time table
{
    u16 array_count = 0, next_interrupt_time = 0;
    static u8 s_low_pulse_flag = 0;

    if(s_low_pulse_flag == 0)
    {
        s_low_pulse_flag = 1; 
        // LOW motor pulse
        GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_RIGHT_PULSE);
        TIM_SetAutoreload(TIM4, STEP_MOTOR_Pulse_Low_Delay);  // 100us low, L297 need minimum 0.5us
    }
    else
    {
        // Change flag for next low signal.
        s_low_pulse_flag = 0;
        
        // HIGH motor pulse
        GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_RIGHT_PULSE);

        if(g_right_mot.step_count <= g_right_mot.max_speed) // Acceleration
        {
            array_count = g_right_mot.step_count;
        }
        else if((g_right_mot.max_speed < g_right_mot.step_count) && (g_right_mot.step_count < g_right_mot.constant_distance)) // Constant velocity
        {
            if(g_right_mot.smooth_turn_flag == DISABLE)
            {
                array_count = g_right_mot.max_speed;
            }
            else    // TODO: Change more simple. smooth turn code
            {
                if(g_right_mot.smooth_speed_down_max_table < g_right_mot.max_speed - g_right_mot.smooth_step_count)
                {
                    array_count = g_right_mot.max_speed - g_right_mot.smooth_step_count;
                }
                else if(g_right_mot.smooth_step_count < g_right_mot.smooth_constant_dist)
                {
                    array_count = g_right_mot.smooth_speed_down_max_table;
                }
                else if(g_right_mot.smooth_step_count < g_right_mot.smooth_target_dist)
                {
                    array_count = g_right_mot.smooth_speed_down_max_table + (g_right_mot.smooth_step_count - g_right_mot.smooth_constant_dist);
                }
                g_right_mot.smooth_step_count++;
            }

        }
        else if(g_right_mot.step_count >= g_right_mot.constant_distance)   // Deceleration
        {
            if(g_right_mot.target_distance >= g_right_mot.step_count)
            {
                array_count = g_right_mot.target_distance - g_right_mot.step_count;
            }
            else
            {
                array_count = 0;
            }
        }

        // Except low puls time
        next_interrupt_time = g_motor_time_table[array_count] - STEP_MOTOR_Pulse_Low_Delay;
        // Add adjustment time for robot position.
        next_interrupt_time += g_right_mot.adjustment_time;
        
        TIM_SetAutoreload(TIM4, next_interrupt_time);

        // Increase step
        g_right_mot.step_count++;
    }
}


void step_motor_direction(step_motor_dir_type l_motor_dir, step_motor_dir_type r_motor_dir)
{
    
	if((l_motor_dir >= STEP_MOTOR_DIR_MAX) || (r_motor_dir >= STEP_MOTOR_DIR_MAX) )
		return;

    // Save dir status
    g_left_mot.dir = l_motor_dir;
    g_right_mot.dir = r_motor_dir;
    
    // Left motor direction
    if(l_motor_dir == STEP_MOTOR_DIR_CCW)
    {
    	GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_LEFT_DIR);
    }
    else
    {
       	GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_LEFT_DIR);
    }

    // Right motor direction
    if(r_motor_dir == STEP_MOTOR_DIR_CCW)
    {
    	GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_RIGHT_DIR);
    }
    else
    {
        GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_RIGHT_DIR);
    }
}

void step_motor_enable(FunctionalState chip_sel)
{

    if(chip_sel == ENABLE)
    {
    	GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_ENABLE);
    }
    else
    {
        GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_ENABLE);
    }
}

void step_motor_angle(step_motor_angle_type angle)
{
    // Save angle status
    g_left_mot.angle = angle;
    g_right_mot.angle = angle;
    
    if(angle == STEP_MOTOR_ANGLE_FULL)
    {
        GPIO_ResetBits(STEP_MOTOR_PORT, STEP_MOTOR_HALF_FULL);
    }
    else
    {
        GPIO_SetBits(STEP_MOTOR_PORT, STEP_MOTOR_HALF_FULL);
    }
}

void motor_all(u16 movement_millimeter, robot_action_type robot_action, robot_with_action_type with_what)
{
    u16 max_speed_table;
    u32 tmp_target_dist = 0;
    
    if(robot_action >= ACT_MAX)
        return;
    // Motor Angle. Half step: 0.9 degree, Full step: 1.8 degree
    step_motor_angle(STEP_MOTOR_ANGLE_HALF);

    // Motor action
    switch(robot_action)
    {
        case ACT_GO_FORWARD:
            max_speed_table = ROBOT_Max_Speed;
            step_motor_direction(STEP_MOTOR_DIR_CCW, STEP_MOTOR_DIR_CW);
            break;
            
        case ACT_GO_BACKWARD:
            max_speed_table = ROBOT_Max_Speed;
            step_motor_direction(STEP_MOTOR_DIR_CW, STEP_MOTOR_DIR_CCW);
            break;

        case ACT_TURN_LEFT:

            max_speed_table = ROBOT_Turn_Speed;
            step_motor_direction(STEP_MOTOR_DIR_CCW, STEP_MOTOR_DIR_CCW);
            break;
            
        case ACT_TURN_RIGHT:
        default:
            max_speed_table = ROBOT_Turn_Speed;
            step_motor_direction(STEP_MOTOR_DIR_CW, STEP_MOTOR_DIR_CW);        
            break;
    }

    //1 Calculate parameters for robot movement.
    // Calculate target step
    // Left wheel
    tmp_target_dist = (u32)((float)movement_millimeter / (float)STEP_MOTOR_One_Step_dist_mm);
    // Target distance do not over 2byte size.
    if(tmp_target_dist > 0xFFFF)
    {
        tmp_target_dist = 0xFFFF;
    }
    g_left_mot.target_distance = tmp_target_dist;
    if(g_left_mot.target_distance <= (max_speed_table * 2))  // Why 2x ? ==> Acceleration and Deceleration table
    {
       g_left_mot.max_speed = g_left_mot.target_distance / 2;
    }
    else
    {
       g_left_mot.max_speed = max_speed_table; 
    }
    g_left_mot.constant_distance = g_left_mot.target_distance - g_left_mot.max_speed;
    g_left_mot.step_count = 0;
    g_left_mot.adjustment_time = 0;

    // Right wheel
    tmp_target_dist = (u32)((float)movement_millimeter / (float)STEP_MOTOR_One_Step_dist_mm);
    // Target distance do not over 2byte size.
    if(tmp_target_dist > 0xFFFF)
    {
        tmp_target_dist = 0xFFFF;
    }
    g_right_mot.target_distance = tmp_target_dist;
    if(g_right_mot.target_distance <= (max_speed_table * 2))
    {
        g_right_mot.max_speed = g_right_mot.target_distance / 2;
    }
    else
    {
        g_right_mot.max_speed = max_speed_table;
    }
    g_right_mot.constant_distance = g_right_mot.target_distance - g_right_mot.max_speed;    
    g_right_mot.step_count = 0;
    g_right_mot.adjustment_time = 0;

    // Motor Enable, L297 enable
    step_motor_enable(ENABLE);
    // Start Timer Interrupt
    bsp_motor_timer_interrupt(ENABLE);

    // Only adjusment in case of go forward or backward. Do not adjust on the turning left or right action.
    if((with_what == ADJUSMENT_FRONT)&&((robot_action == ACT_GO_FORWARD) || (robot_action == ACT_GO_FORWARD)))
    {
        robot_adjusment(with_what);
        step_motor_toque_off();
    }
    else if((with_what == ADJUSMENT_BACK)&&((robot_action == ACT_GO_FORWARD) || (robot_action == ACT_GO_FORWARD)))
    {
        robot_adjusment(with_what);
        step_motor_toque_off();
    }
    else if(with_what == NOT_ADJUSMENT)
    {
        robot_adjusment(with_what);
        step_motor_toque_off();
    }
    else if(with_what == SETTING_ONLY)
    {
        // no add action;
        // You must add check count after calling this function
    }
    else
    {
        assert_param(0);
    }
}

// TODO: Have to confirm function test
void motor_stop(robot_stop_type stop)
{
    if(stop == STOP_EMERGENCY)
    {
        g_left_mot.max_speed /= 2;
        g_right_mot.max_speed /=2;
    }

    g_left_mot.step_count = g_left_mot.max_speed;
    g_left_mot.constant_distance = g_left_mot.max_speed;
    g_left_mot.target_distance = g_left_mot.max_speed * 2;

    g_right_mot.step_count = g_right_mot.max_speed;
    g_right_mot.constant_distance = g_right_mot.max_speed;
    g_right_mot.target_distance = g_right_mot.max_speed * 2;

    
    while((g_left_mot.step_count < g_left_mot.max_speed * 2) && (g_right_mot.step_count < g_right_mot.max_speed * 2));
    step_motor_toque_off();
}

void step_motor_toque_off(void)
{
    bsp_motor_timer_interrupt(DISABLE);

    wait_1ms(100);  // Wait 100ms
    step_motor_enable(DISABLE);        
}

void robot_adjusment(robot_with_action_type adj_position)
{
    if((adj_position == ADJUSMENT_FRONT) || (adj_position == ADJUSMENT_BACK))
    {
        while((g_left_mot.step_count <= g_left_mot.target_distance) || (g_right_mot.step_count <= g_right_mot.target_distance))
        {
            robot_adj_line_sensor(adj_position, &g_left_mot.adjustment_time, &g_right_mot.adjustment_time);
        }
    }
    else if(adj_position == NOT_ADJUSMENT)
    {
        while((g_left_mot.step_count <= g_left_mot.target_distance) || (g_right_mot.step_count <= g_right_mot.target_distance));
    }
    else
    {
        assert_param(0);
    }
}

void robot_adj_line_sensor(robot_with_action_type adj_position, u16 *l_adj_time, u16 *r_adj_time)
{
    u8 line_status = 0;
    
    if(adj_position == ADJUSMENT_FRONT)
    {
        line_status = (u8)robot_read_sensor(SENSOR_FRONT_LINE);
        // Left adjusment
        switch (line_status & 0xF0)
        {
            case 0x80: /*1000 xxxx*/    *l_adj_time = 1000;     break;  /* 1000us = 1ms */  
            case 0xC0: /*1100 xxxx*/    *l_adj_time = 700;      break;
            case 0x40: /*0100 xxxx*/    *l_adj_time = 500;      break;
            case 0x60: /*0110 xxxx*/    *l_adj_time = 300;      break;
            case 0x20: /*0010 xxxx*/    *l_adj_time = 200;      break;
            case 0x30: /*0011 xxxx*/    *l_adj_time = 100;      break;
            case 0x10: /*0001 xxxx*/    *l_adj_time = 50;       break;
            default:                    *l_adj_time = 0;        break;
        }
        // Right adjusment
        switch (line_status & 0x0F)
        {
            case 0x01: /*xxxx 0001*/    *r_adj_time = 1000;     break;  /* 1000us = 1ms */  
            case 0x03: /*xxxx 0011*/    *r_adj_time = 700;      break;
            case 0x02: /*xxxx 0010*/    *r_adj_time = 500;      break;
            case 0x06: /*xxxx 0110*/    *r_adj_time = 300;      break;
            case 0x04: /*xxxx 0100*/    *r_adj_time = 200;      break;
            case 0x0C: /*xxxx 1100*/    *r_adj_time = 100;      break;
            case 0x08: /*xxxx 1000*/    *r_adj_time = 50;       break;
            default:                    *r_adj_time = 0;        break;
        }        
    }
    else if(adj_position == ADJUSMENT_BACK)
    {
        // TODO: Add your code
        line_status = (u16)robot_read_sensor(SENSOR_BACK_LINE);
    }
    else
    {
        assert_param(0);
    }
}

void robot_turn(robot_turn_type act, u16 turn_degree)
{
    u16 step_for_1_degree = 0;
    u16 turn_step = 0;
    
    if(act == TURN_LEFT)
    {
        if(turn_degree == 90)
        {
            turn_step = ROBOT_Turn_Left_Step_For_90_Degree;
        }
        else
        {
            step_for_1_degree = (ROBOT_Turn_Left_Step_For_90_Degree * 100)/ 90;
            turn_step = (step_for_1_degree * turn_degree) / 100;
        }
        motor_all(turn_step, ACT_TURN_LEFT, NOT_ADJUSMENT);
    }
    else if(act == TURN_RIGHT)
    {
        if(turn_degree == 90)
        {
            turn_step = ROBOT_Turn_Right_Step_For_90_Degree;
        }
        else
        {
            step_for_1_degree = ROBOT_Turn_Right_Step_For_90_Degree / 90;
            turn_step = step_for_1_degree * turn_degree;
        }
        motor_all(turn_step, ACT_TURN_RIGHT, NOT_ADJUSMENT);
    }
    else
    {
        assert_param(0);
    }
}

void test_func_robot_turn(robot_turn_type act, u16 turn_step)
{
    u16 wait_time_ms = 2000;
    u8 run_cnt = 0;

    for(run_cnt=0; run_cnt<4; run_cnt++)
    {
        motor_all(300, ACT_GO_FORWARD, NOT_ADJUSMENT);
        wait_1ms(wait_time_ms);
        
        motor_all(turn_step, ((act == TURN_LEFT)? ACT_TURN_LEFT : ACT_TURN_RIGHT), NOT_ADJUSMENT);
        wait_1ms(wait_time_ms);
    }
}

void robot_smooth_turn(robot_turn_type smooth_act)
{
    if(smooth_act == TURN_LEFT)
    {
        g_left_mot.smooth_speed_down_max_table = g_left_mot.max_speed - ROBOT_Smooth_Turn_Dec_Table;
        g_left_mot.smooth_constant_dist = ROBOT_Smooth_Turn_Dec_Table + ROBOT_Smooth_Turn_Const_Dist;
        // Acceleration + Constant distance + Deceleration
        g_left_mot.smooth_target_dist = ROBOT_Smooth_Turn_Dec_Table + ROBOT_Smooth_Turn_Const_Dist + ROBOT_Smooth_Turn_Dec_Table;    
        g_left_mot.smooth_step_count = 0;
        g_left_mot.smooth_turn_flag = ENABLE;    // Enable smooth turn
        while(g_left_mot.smooth_step_count < g_left_mot.smooth_target_dist);
        g_left_mot.smooth_turn_flag = DISABLE;
    }
    else if(smooth_act == TURN_RIGHT)
    {
    
    }
    else
    {
        assert_param(0);
    }
}

void test_func_robot_smooth_turn(robot_turn_type smooth_act)
{
    u8 run_cnt=0;
    u16 prev_step_cnt = 0;
    
    motor_all(10000, ACT_GO_FORWARD, SETTING_ONLY); // 10000mm ==> 10meter
    
    while((g_left_mot.step_count <= MM_TO_STEP(700)) /* || (g_right_mot.step_count <= MM_TO_STEP(700))*/);

    for(run_cnt=0;run_cnt < 4; run_cnt++)
    {
        // Go straight
        // TODO: add right wheel function
        while((g_left_mot.step_count <= prev_step_cnt + MM_TO_STEP(150)) || (g_right_mot.step_count <= prev_step_cnt + MM_TO_STEP(150)))
        {
        }
        // Smooth turn
        robot_smooth_turn(TURN_LEFT);
        prev_step_cnt = g_left_mot.step_count;
    }

    prev_step_cnt = g_left_mot.step_count;
    while((g_left_mot.step_count <= prev_step_cnt + MM_TO_STEP(300)) || (g_right_mot.step_count <= prev_step_cnt + MM_TO_STEP(300)));
    
    motor_stop(STOP_NORMAL);
    wait_1ms(3000);        
}

// TODO: make service mene file. To get related service functions.
void service_step_motor_start(void)
{
    //step_motor_start(ROBOT_GO_FORWARD);
    usart1_transmit_string("\r\nservice_step_motor_start()\r\n");
}

void service_step_motor_stop(void)
{
    motor_stop(STOP_NORMAL);
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


