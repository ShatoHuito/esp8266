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

void ICACHE_FLASH_ATTR user_pre_init(void)
{
	system_partition_table_regist(part_table, 3, system_get_flash_size_map());
}

//------------------------------------------------------
char rx_buf[100] = {};
uint8 fifo_len;
uint8_t fl;

static void uart0_rx_intr_handler(void *para)
{
	if (UART_FRM_ERR_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_FRM_ERR_INT_ST)) {
		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_FRM_ERR_INT_CLR);
	}
	if (UART_RXFIFO_FULL_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_RXFIFO_FULL_INT_ST)) {
		uart_rx_intr_disable(UART0);
	} else if (UART_RXFIFO_TOUT_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_RXFIFO_TOUT_INT_ST)) {
		uart_rx_intr_disable(UART0);
	} else if (UART_TXFIFO_EMPTY_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_TXFIFO_EMPTY_INT_ST)) {
		CLEAR_PERI_REG_MASK(UART_INT_ENA(UART0), UART_TXFIFO_EMPTY_INT_ENA);
		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_TXFIFO_EMPTY_INT_CLR);
	} else if (UART_RXFIFO_OVF_INT_ST == (READ_PERI_REG(UART_INT_ST(UART0)) & UART_RXFIFO_OVF_INT_ST)) {
		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_OVF_INT_CLR);
	}
	fifo_len = (READ_PERI_REG(UART_STATUS(UART0))>>UART_RXFIFO_CNT_S)&UART_RXFIFO_CNT;
	if(fifo_len>0) fl=1;
}

//------------------------------------------------------
void ICACHE_FLASH_ATTR user_init()
{
	uint16_t i=0;
	uint8 d_tmp = 0;
	uint8 idx=0;
	//---UART INIT
	uart_init(UART_BAUD_RATE, UART_BAUD_RATE);
	ETS_UART_INTR_DISABLE();
	ETS_UART_INTR_ATTACH(uart0_rx_intr_handler,  rx_buf);
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);
	ETS_UART_INTR_ENABLE();

	//---GPIO_INIT
	gpio_init();
	//	i2c_master_gpio_init();
	//	I2C_MASTER_SDA_LOW_SCL_LOW();
	i2c_user_gpio_init();
	I2C_USER_SDA_LOW_SCL_LOW();

	ets_delay_us(100000);
	//---DISPLAY INIT
	OLED_init();
	LCD_Clear();
	//--------------------------------
	ets_delay_us(100000);

	LCD_Goto(0, 0);
	OLED_string("ZDAROVA123!@#");
	ets_delay_us(100000);



	while(1)
	{
		while(!fl)
		{
			i++;
			if(i>500)
			{
				i=0;
				system_soft_wdt_feed();
			}
			ets_delay_us(1000);
		}
		fl = 0;
		os_printf("fifo_len: %d\r\n",fifo_len);
		LCD_Clear();
		LCD_Goto(0,1);
		for(idx=0;idx<fifo_len;idx++)
		{
			d_tmp = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;
			if((d_tmp != 0x0D) && (d_tmp != 0x0A))
				LCD_Char(d_tmp);
		}
		WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
		uart_rx_intr_enable(UART0);
	}
}

