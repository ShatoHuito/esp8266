/**
 * original author:  Mishany984 https://www.drive2.ru/c/494502881764835520/
 *
 * modification for STM32H743: Ilnur Mustafin School 21 Kazan Robolab. Intra nickname - gbrittan
 * 16.08.2021
 *
   ----------------------------------------------------------------------
 */

#ifndef __SSD1306_I2C_H
#define __SSD1306_I2C_H
#include <stdio.h>
#include <stdint.h>
#include <c_types.h>


#define OLED_adress 0x78
#define OLED_SETCONTRAST 0x81
#define OLED_DISPLAYALLON_RESUME 0xA4
#define OLED_DISPLAYALLON 0xA5
#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7
#define OLED_DISPLAYOFF 0xAE
#define OLED_DISPLAYON 0xAF
#define OLED_SETDISPLAYOFFSET 0xD3
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_SETDISPLAYCLOCKDIV 0xD5
#define OLED_SETPRECHARGE 0xD9
#define OLED_SETMULTIPLEX 0xA8
#define OLED_SETLOWCOLUMN 0x00
#define OLED_SETHIGHCOLUMN 0x10
#define OLED_SETSTARTLINE 0x40
#define OLED_MEMORYMODE 0x20
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR   0x22
#define OLED_COMSCANINC 0xC0
#define OLED_COMSCANDEC 0xC8
#define OLED_SEGREMAP 0xA0
#define OLED_CHARGEPUMP 0x8D
#define OLED_SWITCHCAPVCC 0x2
#define OLED_NOP 0xE3

#define OLED_WIDTH 128
#define OLED_HEIGHT 64 //32 or 64
#define OLED_BUFFERSIZE (OLED_WIDTH*OLED_HEIGHT)/8
#define OLED_DEFAULT_SPACE 5

#define COMAND 0x00
#define DATA   0x40
uint16_t gg;
uint8_t temp_screen[OLED_HEIGHT / 8][OLED_WIDTH + 1];

void i2c_init(void);
void OLED_init(void);
//unsigned char OLED_write(unsigned char data);
void sendCommand(unsigned char command); //функция отправки команды
void sendData(uint8_t data_s);
void LCD_Clear(void); //очистка экрана
void LCD_Char(unsigned int c); //вывод символа
void LCD_Goto(unsigned char x, unsigned char y); // установка курсора на экране
void LCD_DrawImage(unsigned char num_image);
void OLED_string(char *string); //вывод строки
void OLED_num_to_str(unsigned int value, unsigned char nDigit); //вывод числа
void OLED_print_pix(uint32_t x, uint32_t y); // отрисовка пикселя
void OLED_print_circle(uint32_t x, uint32_t y, uint32_t rad); // type C - закраеный c - пустой
void OLED_print_all_screen(void); // печать всего экрана

void test_all(void);

#endif
