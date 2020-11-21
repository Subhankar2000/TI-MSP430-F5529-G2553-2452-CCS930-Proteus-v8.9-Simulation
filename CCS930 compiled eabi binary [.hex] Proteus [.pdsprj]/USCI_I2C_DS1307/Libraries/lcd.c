#include "lcd.h"


void LCD_init(void)
{
    LCD_PORT_DIR |= (LCD_RS | LCD_EN | LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7);
    LCD_PORT |= (LCD_RS | LCD_EN | LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7);

    delay_ms(20);

    toggle_EN_pin();

    LCD_send(0x33, CMD);
    LCD_send(0x32, CMD);

    LCD_send((_4_pin_interface | _2_row_display | _5x7_dots), CMD);
    LCD_send((display_on | cursor_off | blink_off), CMD);
    LCD_send(clear_display, CMD);
    LCD_send((cursor_direction_inc | display_no_shift), CMD);
}


void LCD_send(unsigned char value, unsigned char mode)
{
    switch(mode)
    {
        case DAT:
        {
            LCD_RS_HIGH;
            break;
        }
        case CMD:
        {
            LCD_RS_LOW;
            break;
        }
    }

    LCD_4bit_send(value);
}


void LCD_4bit_send(unsigned char lcd_data)
{
    toggle_io(lcd_data, 7, LCD_DB7);
    toggle_io(lcd_data, 6, LCD_DB6);
    toggle_io(lcd_data, 5, LCD_DB5);
    toggle_io(lcd_data, 4, LCD_DB4);

    toggle_EN_pin();

    toggle_io(lcd_data, 3, LCD_DB7);
    toggle_io(lcd_data, 2, LCD_DB6);
    toggle_io(lcd_data, 1, LCD_DB5);
    toggle_io(lcd_data, 0, LCD_DB4);

    toggle_EN_pin();
}


void LCD_putstr(char *lcd_string)
{
    do
    {
        LCD_send(*lcd_string++, DAT);
    }while(*lcd_string != '\0');
}


void LCD_putchar(char char_data)
{
    LCD_send(char_data, DAT);
}


void LCD_clear_home(void)
{
    LCD_send(clear_display, CMD);
    LCD_send(goto_home, CMD);
}


void LCD_goto(unsigned char x_pos, unsigned char y_pos)
{
    if(y_pos == 0)
    {
        LCD_send((0x80 | x_pos), CMD);
    }
    else
    {
        LCD_send((0x80 | 0x40 | x_pos), CMD);
    }
}


void toggle_EN_pin(void)
{
    LCD_EN_HIGH;
    delay_ms(2);
    LCD_EN_LOW;
    delay_ms(2);
}


void toggle_io(unsigned char lcd_data, unsigned char bit_pos, unsigned char pin_num)
{
    unsigned char temp = 0x00;

    temp = (0x01 & (lcd_data >> bit_pos));

    switch(temp)
    {
        case 0:
        {
            LCD_PORT &= ~pin_num;
            break;
        }

        default:
        {
            LCD_PORT |= pin_num;
            break;
        }
    }
}
