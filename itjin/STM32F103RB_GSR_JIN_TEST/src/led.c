

#include  <stm32f10x.h>
#include "main.h"    // DEV_KIT_LED_TEST
#include "led.h"


const bsp_led_group_type led_group[ledMax]=
{
	{LED_USER_PORT, LED_USER_PIN }
};


void bsp_led_on(BSP_LED_Def led)
{
	if( led >= ledMax )
		return;

	GPIO_SetBits(led_group[led].gpio_reg, led_group[led].pin);
}

void bsp_led_off(BSP_LED_Def led)
{
	if( led >= ledMax )
		return;

	GPIO_ResetBits(led_group[led].gpio_reg, led_group[led].pin);
}

void bsp_led_toggle(BSP_LED_Def led)
{
	s32 pins;
	if( led >= ledMax )
		return;

	pins = GPIO_ReadOutputData(led_group[ led ].gpio_reg);
	if ((pins & led_group[led].pin) == 0)
	{
		GPIO_SetBits(led_group[led].gpio_reg, led_group[led].pin);
	}
	else
	{
		GPIO_ResetBits(led_group[led].gpio_reg, led_group[led].pin);
	}	
}

#ifdef DEV_KIT_LED_TEST
void led_rotate_test(void)
{
    u8 rotate_bit = 1;
    u8 loop_cnt =0, stack_cnt=0;
    u16 led_disp_delay = 5;
    u16 read_led_port = 0, out_led_port = 0;

    read_led_port = GPIO_ReadOutputData(GPIOC);
    read_led_port |= 0x00ff;    // All led off. Active low
    GPIO_Write(GPIOC, read_led_port);
    wait_10ms(50); // 500ms

    // Led push to stack

    for(loop_cnt = 8; loop_cnt > 0; loop_cnt--)
    {
        rotate_bit = 0x80;
        read_led_port = GPIO_ReadOutputData(GPIOC);
        for(stack_cnt = 0; stack_cnt < loop_cnt; stack_cnt++)
        {
            out_led_port = read_led_port & (u16)(~rotate_bit);
            GPIO_Write(GPIOC, out_led_port);
            wait_10ms(led_disp_delay);
            rotate_bit >>= 1;
        }
    }
    

    for(loop_cnt = 0; loop_cnt < 8; loop_cnt++)
    {
        rotate_bit = 0x80;
        rotate_bit >>= (u16)loop_cnt;
        
        read_led_port = GPIO_ReadOutputData(GPIOC);
        read_led_port |=  (u16)(rotate_bit);

        for(stack_cnt = 0; stack_cnt <= loop_cnt; stack_cnt++)
        {
            rotate_bit <<= 1;
            out_led_port = read_led_port & (u16)(~rotate_bit);
            GPIO_Write(GPIOC, out_led_port);
            wait_10ms(led_disp_delay);
        }
    }    
}

/* Call from isr */
void led_status_test(void)
{
    static u8 shift_bit = 0x80;
    static u8 led_direction = 0;    // 0: right 1:left
    u16 read_led_port = 0, out_led_port = 0;
    
    read_led_port = GPIO_ReadOutputData(GPIOC);
    read_led_port |= 0x00FF;    // LED OFF
    out_led_port = read_led_port & (u16)(~shift_bit);
    GPIO_Write(GPIOC, out_led_port);
    if(led_direction==0) // shift right
    {
        shift_bit >>= 1;
        if((shift_bit & 0x01) == 0x01)
        {
            led_direction = 1; // Change led toward right
        }        
    }
    else    // shift left
    {
        shift_bit <<= 1;
        if((shift_bit & 0x80) == 0x80)
        {
            led_direction = 0; // Change led toward right
        }
    }
}
#endif /* DEV_KIT_LED_TEST */

