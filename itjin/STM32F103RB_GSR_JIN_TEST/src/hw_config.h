/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H


#ifndef NULL
#define NULL ((void *)0)
#endif


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define  INTLOCK( )  
#define  INTFREE( ) 

#define HSE_8MHZ

/* Exported functions ------------------------------------------------------- */
extern void bsp_init_rcc(void);
extern void bsp_init_gpio(void);
extern void bsp_init_adc(void);
extern void DMA_Initial(void);

extern void bsp_init_interrupt(void);

void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
