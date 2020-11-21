#include "SSD1306.h"
#include "fonts.h"


void setup_GPIOs(void)
{
    OLED_PORT_OUT();
    P1SEL2 = BIT5 | BIT7;
    P1SEL = BIT5 | BIT7;
}


void OLED_init(void)
{
    setup_GPIOs();
    HW_SPI_init();
    
    OLED_reset_sequence();
    
    OLED_write((Set_Display_ON_or_OFF_CMD + Display_OFF), CMD);
    
    OLED_write(Set_Display_Clock_CMD, CMD);
    OLED_write(0x80, CMD);
    
    OLED_write(Set_Multiplex_Ratio_CMD, CMD);
    OLED_write(0x3F, CMD);
    
    OLED_write(Set_Display_Offset_CMD, CMD);
    OLED_write(0x00, CMD);
    
    OLED_write((Set_Display_Start_Line_CMD | 0x00), CMD);
    
    OLED_write(Set_Charge_Pump_CMD, CMD);
    OLED_write((Set_Higher_Column_Start_Address_CMD | Enable_Charge_Pump), CMD);

    OLED_write(Set_Memory_Addressing_Mode_CMD, CMD);
    OLED_write(Page_Addressing_Mode, CMD);
    
    OLED_write((Set_Segment_Remap_CMD | Column_Address_0_Mapped_to_SEG127), CMD);
    
    OLED_write((Set_COM_Output_Scan_Direction_CMD | Scan_from_COM63_to_0), CMD);
    
    OLED_write(Set_Common_HW_Config_CMD, CMD);
    OLED_write(0x12, CMD);

    OLED_write(Set_Contrast_Control_CMD, CMD);
    OLED_write(0xCF, CMD);
    
    OLED_write(Set_Pre_charge_Period_CMD, CMD);
    OLED_write(0xF1, CMD);
    
    OLED_write(Set_VCOMH_Level_CMD, CMD);
    OLED_write(0x40, CMD);

    OLED_write((Set_Entire_Display_ON_CMD | Normal_Display), CMD);

    OLED_write((Set_Normal_or_Inverse_Display_CMD | Non_Inverted_Display), CMD);
    
    OLED_write((Set_Display_ON_or_OFF_CMD + Display_ON) , CMD);
    
    OLED_gotoxy(0, 0);
    
    OLED_clear_screen();
}


void OLED_reset_sequence(void)
{
    delay_ms(40);
    OLED_RST_LOW();
    delay_ms(40);
    OLED_RST_HIGH();
}


void OLED_write(unsigned char value, unsigned char type)
{
    switch(type)
    {
        case DAT:
        {
            OLED_DC_HIGH();
            break; 
        }
        case CMD:
        {
            OLED_DC_LOW();
            break; 
        }
    }

    OLED_CS_LOW();
    SPI_transfer(value);
    OLED_CS_HIGH();
}  


void OLED_gotoxy(unsigned char x_pos, unsigned char y_pos)
{                                    
    OLED_write((Set_Page_Start_Address_CMD + y_pos), CMD);
    OLED_write(((x_pos & 0x0F) | Set_Lower_Column_Start_Address_CMD), CMD);
    OLED_write((((x_pos & 0xF0) >> 0x04) | Set_Higher_Column_Start_Address_CMD), CMD);
}


void OLED_fill(unsigned char bmp_data) 
{                                                     
    unsigned char x_pos = 0;
    unsigned char page = 0;

    for(page = y_min; page < y_max; page++)
    {
        OLED_write((Set_Page_Start_Address_CMD + page), CMD);
        OLED_write(Set_Lower_Column_Start_Address_CMD, CMD);
        OLED_write(Set_Higher_Column_Start_Address_CMD, CMD);

        for(x_pos = x_min; x_pos < x_max; x_pos++)
        {
            OLED_write(bmp_data, DAT);
        }
    }
} 


void OLED_clear_screen(void)
{ 
    OLED_fill(0x00);
}


void OLED_cursor(unsigned char x_pos, unsigned char y_pos)
{         
    unsigned char i = 0;

    if(y_pos != 0)
    {
        if(x_pos == 1)
        {
            OLED_gotoxy(0x00, (y_pos + 0x02));
        }
        else
        {
            OLED_gotoxy((0x50 + ((x_pos - 0x02) * 0x06)), (y_pos + 0x02));
        }

        for(i = 0; i < 6; i++)
        {
            OLED_write(0xFF, DAT);
        }
    }
}
       
                    
void OLED_print_char(unsigned char x_pos, unsigned char y_pos, unsigned char ch)
{
    unsigned char chr = 0;
    unsigned char s = 0;

    chr = (ch - 32);

    if(x_pos > (x_max - 6))
    {
        x_pos = 0;
        y_pos++;
    }
    
    OLED_gotoxy(x_pos, y_pos);

    for(s = 0; s < 6; s++)
    {
        OLED_write(font_regular[chr][s], DAT);
    }
}
  

void OLED_print_string(unsigned char x_pos, unsigned char y_pos, unsigned char *ch)
{
    unsigned char chr = 0;
    unsigned char i = 0;
    unsigned char j = 0;

    while(ch[j] != '\0')
    {
        chr = (ch[j] - 32);

        if(x_pos > (x_max - 0x06))
        {
            x_pos = 0x00;
            y_pos++;
        }
        OLED_gotoxy(x_pos, y_pos);

        for(i = 0; i < 6; i++)
        {
            OLED_write(font_regular[chr][i], DAT);
        }

        j++;
        x_pos += 6;
     }
}    
                                             
                      
void OLED_print_chr(unsigned char x_pos, unsigned char y_pos, signed int value)
{                                             
    unsigned char ch = 0;

    if(value < 0)
    {
        OLED_print_char(x_pos, y_pos, '-');
        value = -value;
    }
    else
    {
        OLED_print_char(x_pos, y_pos,' ');
    }

     if((value > 99) && (value <= 999))
     {
         ch = (value / 100);
         OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));
         ch = ((value % 100) / 10);
         OLED_print_char((x_pos + 12), y_pos , (0x30 + ch));
         ch = (value % 10);
         OLED_print_char((x_pos + 18), y_pos , (0x30 + ch));
     }
     else if((value > 9) && (value <= 99))
     {
         ch = ((value % 100) / 10);
         OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));
         ch = (value % 10);
         OLED_print_char((x_pos + 12), y_pos , (0x30 + ch));
         OLED_print_char((x_pos + 18), y_pos , 0x20);
     }
     else if((value >= 0) && (value <= 9))
     {
         ch = (value % 10);
         OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));
         OLED_print_char((x_pos + 12), y_pos , 0x20);
         OLED_print_char((x_pos + 18), y_pos , 0x20);
     }
}


void OLED_print_int(unsigned char x_pos, unsigned char y_pos, signed long value)
{ 
    unsigned char ch = 0;

    if(value < 0)
    {
        OLED_print_char(x_pos, y_pos, '-');
        value = -value;
    }
    else
    {
        OLED_print_char(x_pos, y_pos,' ');
    }

    if(value > 9999)
    {
        ch = (value / 10000);
        OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));

        ch = ((value % 10000)/ 1000);
        OLED_print_char((x_pos + 12), y_pos , (0x30 + ch));

        ch = ((value % 1000) / 100);
        OLED_print_char((x_pos + 18), y_pos , (0x30 + ch));

        ch = ((value % 100) / 10);
        OLED_print_char((x_pos + 24), y_pos , (0x30 + ch));

        ch = (value % 10);
        OLED_print_char((x_pos + 30), y_pos , (0x30 + ch));
    }

    else if((value > 999) && (value <= 9999))
    {
        ch = ((value % 10000)/ 1000);
        OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));

        ch = ((value % 1000) / 100);
        OLED_print_char((x_pos + 12), y_pos , (0x30 + ch));

        ch = ((value % 100) / 10);
        OLED_print_char((x_pos + 18), y_pos , (0x30 + ch));

        ch = (value % 10);
        OLED_print_char((x_pos + 24), y_pos , (0x30 + ch));
        OLED_print_char((x_pos + 30), y_pos , 0x20);
    }
    else if((value > 99) && (value <= 999))
    {
        ch = ((value % 1000) / 100);
        OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));

        ch = ((value % 100) / 10);
        OLED_print_char((x_pos + 12), y_pos , (0x30 + ch));

        ch = (value % 10);
        OLED_print_char((x_pos + 18), y_pos , (0x30 + ch));
        OLED_print_char((x_pos + 24), y_pos , 0x20);
        OLED_print_char((x_pos + 30), y_pos , 0x20);
    }
    else if((value > 9) && (value <= 99))
    {
        ch = ((value % 100) / 10);
        OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));

        ch = (value % 10);
        OLED_print_char((x_pos + 12), y_pos , (0x30 + ch));
        
        OLED_print_char((x_pos + 18), y_pos , 0x20);
        OLED_print_char((x_pos + 24), y_pos , 0x20);
        OLED_print_char((x_pos + 30), y_pos , 0x20);
    }
    else
    {
        ch = (value % 10);
        OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));
        OLED_print_char((x_pos + 12), y_pos , 0x20);
        OLED_print_char((x_pos + 18), y_pos , 0x20);
        OLED_print_char((x_pos + 24), y_pos , 0x20);
        OLED_print_char((x_pos + 30), y_pos , 0x20);
    }
}                                                      


void OLED_print_decimal(unsigned char x_pos, unsigned char y_pos, unsigned int value, unsigned char points)
{
    unsigned char ch = 0;

    OLED_print_char(x_pos, y_pos, '.');

    ch = (value / 1000);
    OLED_print_char((x_pos + 6), y_pos , (0x30 + ch));

    if(points > 1)
    {
        ch = ((value % 1000) / 100);
        OLED_print_char((x_pos + 12), y_pos , (0x30 + ch));


        if(points > 2)
        {
            ch = ((value % 100) / 10);
            OLED_print_char((x_pos + 18), y_pos , (0x30 + ch));

            if(points > 3)
            {
                ch = (value % 10);
                OLED_print_char((x_pos + 24), y_pos , (0x30 + ch));
            }
        }
    }
}
 
                             
void OLED_print_float(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points) 
{
    signed long tmp = 0;

    tmp = value;
    OLED_print_int(x_pos, y_pos, tmp);
    tmp = ((value - tmp) * 10000);
    
    if(tmp < 0)
    {
       tmp = -tmp;
    }

    if((value >= 10000) && (value < 100000))
    {
        OLED_print_decimal((x_pos + 36), y_pos, tmp, points);
    }
    else if((value >= 1000) && (value < 10000))
    {
        OLED_print_decimal((x_pos + 30), y_pos, tmp, points);
    }
    else if((value >= 100) && (value < 1000))
    {
        OLED_print_decimal((x_pos + 24), y_pos, tmp, points);
    }
    else if((value >= 10) && (value < 100))
    {
        OLED_print_decimal((x_pos + 18), y_pos, tmp, points);
    }
    else if(value < 10)
    {
        OLED_print_decimal((x_pos + 12), y_pos, tmp, points);
        if(value < 0)
        {
            OLED_print_char(x_pos, y_pos, '-');
        }
        else
        {
            OLED_print_char(x_pos, y_pos, ' ');
        }
    }
}
