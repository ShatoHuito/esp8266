#include <stdio.h>
#include <stdint.h>
#include <c_types.h>
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "ets_sys.h"
#include "user_interface.h"
#include "user_config.h"
#include "driver/uart.h"
#include "driver/i2c_master.h"

#include "ssd1306_i2c.h"
#include "i2c_user.h"


#define UART_BAUD_RATE				115200
//----------------------------------------------------
#define SYSTEM_PARTITION_RF_CAL_SZ              0x1000
#define SYSTEM_PARTITION_PHY_DATA_SZ            0x1000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ    0x3000
#define SYSTEM_SPI_SIZE				0x400000
#define SYSTEM_PARTITION_RF_CAL_ADDR            SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ - SYSTEM_PARTITION_PHY_DATA_SZ - SYSTEM_PARTITION_RF_CAL_SZ
#define SYSTEM_PARTITION_PHY_DATA_ADDR          SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ - SYSTEM_PARTITION_PHY_DATA_SZ
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR  SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ

static const partition_item_t part_table[] =
{
	{ SYSTEM_PARTITION_RF_CAL,              SYSTEM_PARTITION_RF_CAL_ADDR,           SYSTEM_PARTITION_RF_CAL_SZ              },
	{ SYSTEM_PARTITION_PHY_DATA,            SYSTEM_PARTITION_PHY_DATA_ADDR,         SYSTEM_PARTITION_PHY_DATA_SZ            },
	{ SYSTEM_PARTITION_SYSTEM_PARAMETER,    SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ    }
};
//----------------------------------------------------
//---------------------------------------------------------

void ICACHE_FLASH_ATTR user_pre_init(void)
{
	system_partition_table_regist(part_table, 3, system_get_flash_size_map());
}

//------------------------------------------------------
void ICACHE_FLASH_ATTR user_init()
{
	uart_init(UART_BAUD_RATE, UART_BAUD_RATE);
	//------------------------
	gpio_init();
//	i2c_master_gpio_init();
//	I2C_MASTER_SDA_LOW_SCL_LOW();
	i2c_user_gpio_init();
	I2C_USER_SDA_LOW_SCL_LOW();
	ets_delay_us(100000);

	//--------------------------------
	OLED_init();
	LCD_Clear();
	ets_delay_us(100000);

	//--------------------------------
	LCD_Goto(0, 0);
	OLED_string("ZDAROVA!!");
	ets_delay_us(100000);



	while(1)
	{
		ets_delay_us(100000);
		system_soft_wdt_feed();
		ets_delay_us(100000);
		system_soft_wdt_feed();
	}
}

