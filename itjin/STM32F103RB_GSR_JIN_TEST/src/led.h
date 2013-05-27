
#ifndef  LED_PRESENT
#define  LED_PRESENT

#include "hw_config.h"
#include <stm32f10x.h>


typedef enum
{
	ledUser = 0x00,
	ledMax
} BSP_LED_Def;


#define LED_USER_PORT				GPIOC
#define LED_USER_PIN				GPIO_Pin_12

typedef struct
{
	GPIO_TypeDef* gpio_reg;
	s16          pin;
}bsp_led_group_type;



/* ------------------------------------------------------------------------------------------------- */
/* BSP LED */
/* ------------------------------------------------------------------------------------------------- */

extern void bsp_led_on(BSP_LED_Def led);
extern void bsp_led_off(BSP_LED_Def led);
extern void bsp_led_toggle(BSP_LED_Def led);
#ifdef DEV_KIT_LED_TEST
void led_rotate_test(void);
void led_status_test(void);

#endif /* DEV_KIT_LED_TEST */


#endif
