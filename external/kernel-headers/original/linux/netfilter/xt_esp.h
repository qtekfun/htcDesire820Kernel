#ifndef _XT_ESP_H
#define _XT_ESP_H

struct xt_esp
{
	u_int32_t spis[2];	
	u_int8_t  invflags;	
};

#define XT_ESP_INV_SPI	0x01	
#define XT_ESP_INV_MASK	0x01	

#endif 
