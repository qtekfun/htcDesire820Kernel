/*
 *  Copyright (c) 2007, 2008 HTC, Inc
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without modification,
 *   are permitted provided that the following conditions are met: 
 *
 *   Redistributions of source code must retain the above copyright notice, this list 
 *   of conditions and the following disclaimer. 
 *   Redistributions in binary form must reproduce the above copyright notice, this 
 *   list of conditions and the following disclaimer in the documentation and/or other
 *   materials provided with the distribution. 
 *   Neither the name of the HTC,Inc nor the names of its contributors may be used 
 *   to endorse or promote products derived from this software without specific prior
 *   written permission. 
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
 *   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 *   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;LOSS OF USE, DATA, OR 
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 *   OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT 
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 *   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
 *   DAMAGE
 */

#ifndef CAMERA_MT9T013_H
#define CAMERA_MT9T013_H
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <asm/sizes.h>


#define MT9T013_I2C_IOCTL_MAGIC 'm'

#define MT9T013_I2C_IOCTL_W \
	        _IOW(MT9T013_I2C_IOCTL_MAGIC, 0, unsigned)

#define MT9T013_I2C_IOCTL_R \
	        _IOR(MT9T013_I2C_IOCTL_MAGIC, 1, unsigned)

#define MT9T013_I2C_IOCTL_AF_W \
	        _IOW(MT9T013_I2C_IOCTL_MAGIC, 2, unsigned)

#define MT9T013_I2C_IOCTL_CAMIF_PAD_REG_RESET \
	_IO(MT9T013_I2C_IOCTL_MAGIC, 3)

#define MT9T013_I2C_IOCTL_CAMIF_PAD_REG_RESET_2 \
	_IO(MT9T013_I2C_IOCTL_MAGIC, 4)

#define CAMERA_CONFIGURE_GPIOS \
	_IO(MT9T013_I2C_IOCTL_MAGIC, 7)

#define CAMERA_UNCONFIGURE_GPIOS \
	_IO(MT9T013_I2C_IOCTL_MAGIC, 8)
	
#define CAMERA_LENS_POWER_ON \
	_IO(MT9T013_I2C_IOCTL_MAGIC, 9)
	
#define CAMERA_LENS_POWER_OFF \
	_IO(MT9T013_I2C_IOCTL_MAGIC, 10)

#define MT9T013_I2C_IOCTL_CAMIF_APPS_RESET \
	_IO(MT9T013_I2C_IOCTL_MAGIC, 11)


#define CAMIO_VFE_MDC_CLK 1 
#define CAMIO_MDC_CLK     2 
#define CAMIO_VFE_CLK     3 

#define MT9T013_I2C_IOCTL_CLK_ENABLE \
	_IOW(MT9T013_I2C_IOCTL_MAGIC, 12, unsigned)

#define MT9T013_I2C_IOCTL_CLK_DISABLE \
	_IOW(MT9T013_I2C_IOCTL_MAGIC, 13, unsigned)

#define MT9T013_I2C_IOCTL_CLK_SELECT \
	_IOW(MT9T013_I2C_IOCTL_MAGIC, 14, unsigned)

#define MT9T013_I2C_IOCTL_CLK_FREQ_PROG \
	_IOW(MT9T013_I2C_IOCTL_MAGIC, 15, unsigned)

#define CAMSENSOR_REG_INIT		0<<0
#define CAMSENSOR_REG_UPDATE_PERIODIC 	1<<0
#define CAMSENSOR_TYPE_PREVIEW          0<<1
#define CAMSENSOR_TYPE_SNAPSHOT		1<<1

#define MT9T013_I2C_IOCTL_SENSOR_SETTING \
	_IOW(MT9T013_I2C_IOCTL_MAGIC, 16, uint32_t)

struct mt9t013_reg_struct
{
	uint16_t vt_pix_clk_div;            
	uint16_t vt_sys_clk_div;            
	uint16_t pre_pll_clk_div;           
	uint16_t pll_multiplier;            
	uint16_t op_pix_clk_div;            
	uint16_t op_sys_clk_div;            
	uint16_t scale_m;                   
	uint16_t row_speed;                 
	uint16_t x_addr_start;              
	uint16_t x_addr_end;                
	uint16_t y_addr_start;              
	uint16_t y_addr_end;                
	uint16_t read_mode;                 
	uint16_t x_output_size ;            
	uint16_t y_output_size;             
	uint16_t line_length_pck;           
	uint16_t frame_length_lines;        
	uint16_t coarse_integration_time;   
	uint16_t fine_integration_time;     
}; 

struct mt9t013_reg_pat {
	struct mt9t013_reg_struct reg[2];
};

#define MT9T013_I2C_IOCTL_GET_REGISTERS \
	_IOR(MT9T013_I2C_IOCTL_MAGIC, 17, struct mt9t013_reg_pat *)

struct mt9t013_exposure_gain {
	uint16_t gain;
	uint16_t line;
        uint32_t mode;
};

#define MT9T013_I2C_IOCTL_EXPOSURE_GAIN \
	_IOW(MT9T013_I2C_IOCTL_MAGIC, 18, struct exposure_gain *)

#define MT9T013_I2C_IOCTL_MOVE_FOCUS \
	_IOW(MT9T013_I2C_IOCTL_MAGIC, 19, uint32_t)

#define MT9T013_I2C_IOCTL_SET_DEFAULT_FOCUS \
	_IOW(MT9T013_I2C_IOCTL_MAGIC, 20, uint32_t)

#define MT9T013_I2C_IOCTL_POWER_DOWN \
	_IO(MT9T013_I2C_IOCTL_MAGIC, 21)

struct mt9t013_init {
	int preview; 
	uint16_t chipid; 
};

#define MT9T013_I2C_IOCTL_INIT \
	_IOWR(MT9T013_I2C_IOCTL_MAGIC, 22, struct mt9t013_init *)

#endif

