#include "main.h"
#include "stm32f10x.h"
#include "hw_config.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "motor_step.h"
#include "sensor.h"

vu16 ADC1ConvertedValue[3] = {0};

u16 robot_read_sensor(robot_sensor_type sensor)
{
    u16 ret_value = 0;
    
    if(sensor == SENSOR_FRONT_LINE)
    {
        // TODO: Add your function
    }
    else if(sensor == SENSOR_BACK_LINE)
    {
        // TODO: Add your function

    }
    else if(sensor == SENSOR_TOP_AD)
    {
        // TODO: Add your function
    }
    else if(sensor == SENSOR_BOTTOM_AD)
    {
        // TODO: Add your function
    }
    else if(sensor == SENSOR_LEFT_AD)
    {
        // TODO: Add your function
    }    
    else if(sensor == SENSOR_RIGHT_AD)
    {
        // TODO: Add your function
    }
    else
    {
        assert_param(0);
    }

    return ret_value;
}

void test_func_adc_conv(void)
{   
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    wait_1ms(3000);
    
    while(1)
    {
    	usart1_transmit_string_format("\r\n ADC1=%05d ADC2=%05d ADC3=%05d", ADC1ConvertedValue[0], ADC1ConvertedValue[1], ADC1ConvertedValue[2]);        

        wait_1ms(500);
    }
}




