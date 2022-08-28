/*
 * i2c_user.h
 *
 *  Created on: Aug 23, 2022
 *      Author: ilnur
 */

#ifndef INC_I2C_USER_H_
#define INC_I2C_USER_H_
#include "osapi.h"
#include "gpio.h"
#include "ets_sys.h"
#include "driver/i2c_master.h"


//------------------copy from std lib i2c_master.h
#define I2C_USER_SDA_MUX PERIPHS_IO_MUX_GPIO5_U
#define I2C_USER_SCL_MUX PERIPHS_IO_MUX_GPIO4_U
#define I2C_USER_SDA_GPIO 5
#define I2C_USER_SCL_GPIO 4
#define I2C_USER_SDA_FUNC FUNC_GPIO5
#define I2C_USER_SCL_FUNC FUNC_GPIO4

//#define I2C_USER_SDA_MUX PERIPHS_IO_MUX_GPIO2_U
//#define I2C_USER_SCL_MUX PERIPHS_IO_MUX_GPIO0_U
//#define I2C_USER_SDA_GPIO 2
//#define I2C_USER_SCL_GPIO 0
//#define I2C_USER_SDA_FUNC FUNC_GPIO2
//#define I2C_USER_SCL_FUNC FUNC_GPIO0

#if 0
#define I2C_USER_GPIO_SET(pin)  \
    gpio_output_set(1<<pin,0,1<<pin,0)

#define I2C_USER_GPIO_CLR(pin) \
    gpio_output_set(0,1<<pin,1<<pin,0)

#define I2C_USER_GPIO_OUT(pin,val) \
    if(val) I2C_USER_GPIO_SET(pin);\
    else I2C_USER_GPIO_CLR(pin)
#endif

#define I2C_USER_SDA_HIGH_SCL_HIGH()  \
    gpio_output_set(1<<I2C_USER_SDA_GPIO | 1<<I2C_USER_SCL_GPIO, 0, 1<<I2C_USER_SDA_GPIO | 1<<I2C_USER_SCL_GPIO, 0)

#define I2C_USER_SDA_HIGH_SCL_LOW()  \
    gpio_output_set(1<<I2C_USER_SDA_GPIO, 1<<I2C_USER_SCL_GPIO, 1<<I2C_USER_SDA_GPIO | 1<<I2C_USER_SCL_GPIO, 0)

#define I2C_USER_SDA_LOW_SCL_HIGH()  \
    gpio_output_set(1<<I2C_USER_SCL_GPIO, 1<<I2C_USER_SDA_GPIO, 1<<I2C_USER_SDA_GPIO | 1<<I2C_USER_SCL_GPIO, 0)

#define I2C_USER_SDA_LOW_SCL_LOW()  \
    gpio_output_set(0, 1<<I2C_USER_SDA_GPIO | 1<<I2C_USER_SCL_GPIO, 1<<I2C_USER_SDA_GPIO | 1<<I2C_USER_SCL_GPIO, 0)

void i2c_user_gpio_init(void);
void i2c_user_init(void);

#define i2c_user_wait    os_delay_us
void i2c_user_stop(void);
void i2c_user_start(void);
void i2c_user_setAck(uint8 level);
uint8 i2c_user_getAck(void);
uint8 i2c_user_readByte(void);
void i2c_user_writeByte(uint8 wrdata);

bool i2c_user_checkAck(void);
void i2c_user_send_ack(void);
void i2c_user_send_nack(void);
//-------------end copy from std lib i2c_master.h



void I2C_SendBytesByAdrMasterI2C(uint8_t adr, uint8_t *buf, uint8_t len);
void I2C_SendBytesByAdrUserI2C(uint8_t adr, uint8_t *buf, uint8_t len);


#endif /* INC_I2C_USER_H_ */
