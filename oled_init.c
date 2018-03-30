#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>
#include <libzed/mmap_regs.h>

#define AXI_OLED_CTL 0x41200000
#define AXI_OLED_SPI 0x41630000

static char* OledCtl_regs = NULL;
static char* OledData_regs = NULL;

int oled_init()
{
  if (!OledCtl_regs)
    {
      OledCtl_regs = map_region(AXI_OLED_CTL+0x8, 0x1000);
    }

  if (!OledData_regs)
  {
	  OledData_regs = map_region(AXI_OLED_SPI, 0x1000);
  }
  
  write_reg(OledCtl_regs, 0); //set vdd high
  write_reg(OledCtl_regs, 0); //set oled to receive commands for init
  write_reg(OledData_regs, 0xAE); //set display off command
  write_reg(OledData_regs, 0xD5); //set display clock divide ratio command
  write_reg(OledData_regs, 0x80); //value of clock divide ratio
  write_reg(OledData_regs, 0xA8); //set multiplex ratio command
  write_reg(OledData_regs, 0x1F); //set multiplex ratio value
  write_reg(OledData_regs, 0xD3); //set display offset command
  write_reg(OledData_regs, 0); //set display offset value
  write_reg(OledData_regs, 0x40); //set display start line
  write_reg(OledData_regs, 0x8D); //set charge pump command
  write_reg(OledData_regs, 0x14); //set sharge pump with vcc generated by internal circuit *
  write_reg(OledData_regs, 0xA1); //set segment remap
  write_reg(OledData_regs, 0xC8); //set com output scan direction
  write_reg(OledData_regs, 0xDA); //Set com pins hardware configuration
  write_reg(OledData_regs, 0x02); //"
  write_reg(OledData_regs, 0x81); //set contrast control
  write_reg(OledData_regs, 0x8F); //"
  write_reg(OledData_regs, 0xD9); //set precharge period command
  write_reg(OledData_regs, 0xF1); //set precharge period value with vcc generated by internal circuit *
  write_reg(OledData_regs, 0xDB); //set vcomh deselect level command
  write_reg(OledData_regs, 0x40); //" value
  write_reg(OledData_regs, 0xA4); //resume to RAM content display
  write_reg(OledData_regs, 0xA6); //set display based on value in ram (0 for off, 1 for on)
  write_reg(OledData_regs, 0xAF); //Set Display on  
	
 return 0;
}