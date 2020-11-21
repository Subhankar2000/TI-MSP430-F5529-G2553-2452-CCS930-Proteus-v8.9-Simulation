#include "DS1307.h"


extern struct
{
   unsigned char sec;
   unsigned char min;
   unsigned char hr;
   unsigned char day;
   unsigned char dt;
   unsigned char mt;
   unsigned char yr;
}rtc;


void DS1307_init(void)
{
	I2C_USCI_init(DS1307_address);
	DS1307_write(sec_reg, 0x00);
    DS1307_write(control_reg, 0x90);
}


unsigned char DS1307_read(unsigned char address)
{
    return I2C_USCI_read_byte(address);
}


void DS1307_write(unsigned char address, unsigned char value)
{
    I2C_USCI_write_byte(address, value);
}


unsigned char bcd_to_decimal(unsigned char value)
{
    return ((value & 0x0F) + (((value & 0xF0) >> 0x04) * 0x0A));
}


unsigned char decimal_to_bcd(unsigned char value)
{
    return (((value / 0x0A) << 0x04) & 0xF0) | ((value % 0x0A) & 0x0F);
}


void get_time(void)
{
    rtc.sec = DS1307_read(sec_reg);
    rtc.sec = bcd_to_decimal(rtc.sec);

    rtc.min = DS1307_read(min_reg);
    rtc.min = bcd_to_decimal(rtc.min);

    rtc.hr = DS1307_read(hr_reg);
    rtc.hr = bcd_to_decimal(rtc.hr);
}


void get_date(void)
{
    rtc.day = DS1307_read(day_reg);
    rtc.day = bcd_to_decimal(rtc.day);

    rtc.dt = DS1307_read(date_reg);
    rtc.dt = bcd_to_decimal(rtc.dt);

    rtc.mt = DS1307_read(month_reg);
    rtc.mt = bcd_to_decimal(rtc.mt);

    rtc.yr = DS1307_read(year_reg);
    rtc.yr = bcd_to_decimal(rtc.yr);
}


void set_time(void)
{
    rtc.sec = decimal_to_bcd(rtc.sec);
	DS1307_write(sec_reg, rtc.sec);
	
	rtc.min = decimal_to_bcd(rtc.min);
	DS1307_write(min_reg, rtc.min);
	
	rtc.hr = decimal_to_bcd(rtc.hr);
	DS1307_write(hr_reg, rtc.hr);
}


void set_date(void)
{
    rtc.day = decimal_to_bcd(rtc.day);
    DS1307_write(day_reg, rtc.day);

    rtc.dt = decimal_to_bcd(rtc.dt);
    DS1307_write(date_reg, rtc.dt);

    rtc.mt = decimal_to_bcd(rtc.mt);
    DS1307_write(month_reg, rtc.mt);

    rtc.yr = decimal_to_bcd(rtc.yr);
    DS1307_write(year_reg, rtc.yr);
}
