#include "stm32f10x.h"
#include "usart.h"
#include "main.h"
#include "i2c_test.h"


#ifdef DEV_KIT_I2C_TOUCH_KEY_TEST
static void I2C_Config(void);




void i2c_Test_Example(void)
{
    unsigned char ReadData=0;

    /* Disable the I2C1 peripheral  */
//    I2C_Cmd(I2C1, DISABLE);
    i2c_GPIO_Config();
#if 1
    /* Reset all I2C2 registers */
    I2C_SoftwareResetCmd(I2C1, ENABLE);
    I2C_SoftwareResetCmd(I2C1, DISABLE);

    /* Configure the I2C peripheral */
    I2C_Config();
    I2C_Cmd(I2C1, ENABLE);
#endif    
    while(1)
    {
//        i2c_WriteRegister(TOUCH_KEY_DEV_ADDR, 0, 0x55);
        ReadData=(unsigned char)i2c_ReadRegister(TOUCH_KEY_DEV_ADDR, 0x01);

//        ReadData=(unsigned char)i2c_ReadRegisterByte(TOUCH_KEY_DEV_ADDR, 0x00);
        usart1_transmit_string_format("\r\n Data: 0x%02X", ReadData);
        wait_1ms(1000);
    }
}

void i2c_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /* I2C1 SCL PB6 and SDA PB7 pins configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Enable the I2C1 APB1 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

}
static void I2C_Config(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  /* I2C1 configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x0;//0x33; // RECHECK
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 200000;
  I2C_Init(I2C1, &I2C_InitStructure);
}

uint32_t i2c_WriteRegister(uint8_t Address, uint8_t RegisterAddr, uint8_t RegisterValue)
{
  uint32_t read_verif = 0;

  /* Reset all I2C2 registers */
  I2C_SoftwareResetCmd(I2C1, ENABLE);
  I2C_SoftwareResetCmd(I2C1, DISABLE);

  /* Enable the I2C1 peripheral  */
  I2C_Cmd(I2C1, ENABLE);

  /* Configure the I2C peripheral */
  I2C_Config();

  /* Begin the config sequence */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, Address, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}
  /* Transmit the first address for r/w operations */
  I2C_SendData(I2C1, RegisterAddr);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /* Prepare the register value to be sent */
  I2C_SendData(I2C1, RegisterValue);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /* End the configuration sequence */
  I2C_GenerateSTOP(I2C1, ENABLE);

   /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return read_verif;
}

uint32_t i2c_ReadRegister(uint8_t Address, uint8_t RegisterAddr)
{
  uint32_t tmp = 0;

  /* Disable the I2C1 peripheral  */
  I2C_Cmd(I2C1, DISABLE);

  /* Reset all I2C2 registers */
  I2C_SoftwareResetCmd(I2C1, ENABLE);
  I2C_SoftwareResetCmd(I2C1, DISABLE);

  /* Configure the I2C peripheral */
  I2C_Config();

  /* Enable the I2C peripheral */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}

  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(I2C1, DISABLE);

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, Address, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}

  /* Transmit the first address for r/w operations */
  I2C_SendData(I2C1, RegisterAddr);

  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}

  /* Regenerate a start condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, Address, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {}

  /* Test on EV7 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {}

  /* End the configuration sequence */
  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Load the register value */
  tmp = I2C_ReceiveData(I2C1);

  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /* Return the read value */
  return tmp;
}

uint32_t i2c_ReadRegisterByte(uint8_t Address, uint8_t RegisterAddr)
{
  uint32_t tmp = 0;

  /* Disable the I2C1 peripheral  */
  I2C_Cmd(I2C1, DISABLE);

  /* Reset all I2C2 registers */
  I2C_SoftwareResetCmd(I2C1, ENABLE);
  I2C_SoftwareResetCmd(I2C1, DISABLE);

  /* Configure the I2C peripheral */
  I2C_Config();

  /* Enable the I2C peripheral */
  I2C_GenerateSTART(I2C1, ENABLE);
#if 0
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}
#endif

  /* Disable Acknowledgement */
//  I2C_AcknowledgeConfig(I2C1, DISABLE);

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, Address, I2C_Direction_Receiver);

#if 1
  wait_1ms(10);
#else
  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {}
#endif
  /* Transmit the first address for r/w operations */
  I2C_SendData(I2C1, RegisterAddr);

#if 1
  wait_1ms(10);
#else
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {}
#endif

#if 1
  /* Load the register value */
  tmp = I2C_ReceiveData(I2C1);

  wait_1ms(10);
  /* Disable Acknowledgement */
//  I2C_AcknowledgeConfig(I2C1, ENABLE);

  /* End the configuration sequence */
  I2C_GenerateSTOP(I2C1, ENABLE);
#else
  /* Regenerate a start condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {}

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(I2C1, TOUCH_KEY_DEV_ADDR, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {}

  /* Test on EV7 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {}

  /* End the configuration sequence */
  I2C_GenerateSTOP(I2C1, ENABLE);

  /* Load the register value */
  tmp = I2C_ReceiveData(I2C1);

  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
#endif

  /* Return the read value */
  return tmp;
}




#endif /* DEV_KIT_I2C_TOUCH_KEY_TEST */

