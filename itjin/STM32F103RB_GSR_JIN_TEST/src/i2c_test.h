#ifndef I2C_TEST_PRESENT
#define I2C_TEST_PRESENT


#define TOUCH_KEY_DEV_ADDR      0x40


void i2c_Test_Example(void);
void i2c_GPIO_Config(void);
uint32_t i2c_WriteRegister(uint8_t Address, uint8_t RegisterAddr, uint8_t RegisterValue);
uint32_t i2c_ReadRegister(uint8_t Address, uint8_t RegisterAddr);
uint32_t i2c_ReadRegisterByte(uint8_t Address, uint8_t RegisterAddr);


#endif /* I2C_TEST_PRESENT */

