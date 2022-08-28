#include <stdio.h>
#include <stdint.h>
#include <c_types.h>
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "ets_sys.h"
#include "user_interface.h"

//-----------DEFINES
#define LED 2
#define BUTTON 4
//-------------------

//------------------------------------------------------
uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}
//------------------------------------------------------

//------------------------------------------------------
void ICACHE_FLASH_ATTR user_init(){
	bool fl = false;
	gpio_init();
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U); //подтягиваем резистора на ногу


	gpio_output_set(0, 0, (1 << LED), 0);
	gpio_output_set(0, 0, 0, (1 << BUTTON));

	while(1)
	{
	    ets_delay_us(1000);
	    system_soft_wdt_feed();
	    if(!((gpio_input_get()>>BUTTON) & BIT0)){
	    	if(fl){
	    		gpio_output_set(0, (1 << LED), 0, 0);
	    		fl = false;
	    	}
	    	else{
	    		gpio_output_set((1 << LED), 0, 0, 0);
	    		fl = true;
	    	}
	    	while(!((gpio_input_get()>>BUTTON) & BIT0))
	    		ets_delay_us(1);
	    }
	    ets_delay_us(1000);
	    system_soft_wdt_feed();

	}
}
//------------------------------------------------------

