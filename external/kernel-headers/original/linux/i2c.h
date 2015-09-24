/*   Copyright (C) 1995-2000 Simon G. Vogl

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.		     */


#ifndef _LINUX_I2C_H
#define _LINUX_I2C_H

#include <linux/types.h>
#ifdef __KERNEL__ 
#include <linux/module.h>
#include <linux/i2c-id.h>
#include <linux/mod_devicetable.h>
#include <linux/device.h>	
#include <linux/sched.h>	
#include <linux/mutex.h>


extern void i2c_adapter_dev_release(struct device *dev);
extern struct device_driver i2c_adapter_driver;
extern struct class i2c_adapter_class;
extern struct bus_type i2c_bus_type;


struct i2c_msg;
struct i2c_algorithm;
struct i2c_adapter;
struct i2c_client;
struct i2c_driver;
union i2c_smbus_data;

extern int i2c_master_send(struct i2c_client *,const char* ,int);
extern int i2c_master_recv(struct i2c_client *,char* ,int);

extern int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num);

extern int i2c_slave_send(struct i2c_client *,char*,int);
extern int i2c_slave_recv(struct i2c_client *,char*,int);



extern s32 i2c_smbus_xfer (struct i2c_adapter * adapter, u16 addr, 
                           unsigned short flags,
                           char read_write, u8 command, int size,
                           union i2c_smbus_data * data);


extern s32 i2c_smbus_write_quick(struct i2c_client * client, u8 value);
extern s32 i2c_smbus_read_byte(struct i2c_client * client);
extern s32 i2c_smbus_write_byte(struct i2c_client * client, u8 value);
extern s32 i2c_smbus_read_byte_data(struct i2c_client * client, u8 command);
extern s32 i2c_smbus_write_byte_data(struct i2c_client * client,
                                     u8 command, u8 value);
extern s32 i2c_smbus_read_word_data(struct i2c_client * client, u8 command);
extern s32 i2c_smbus_write_word_data(struct i2c_client * client,
                                     u8 command, u16 value);
extern s32 i2c_smbus_write_block_data(struct i2c_client * client,
				      u8 command, u8 length,
				      const u8 *values);
extern s32 i2c_smbus_read_i2c_block_data(struct i2c_client * client,
					 u8 command, u8 *values);
extern s32 i2c_smbus_write_i2c_block_data(struct i2c_client * client,
					  u8 command, u8 length,
					  const u8 *values);


struct i2c_driver {
	int id;
	unsigned int class;

	int (*attach_adapter)(struct i2c_adapter *);
	int (*detach_adapter)(struct i2c_adapter *);

	int (*detach_client)(struct i2c_client *);
	
	int (*command)(struct i2c_client *client,unsigned int cmd, void *arg);

	struct device_driver driver;
	struct list_head list;
};
#define to_i2c_driver(d) container_of(d, struct i2c_driver, driver)

#define I2C_NAME_SIZE	50

struct i2c_client {
	unsigned int flags;		
	unsigned short addr;		
					
					
	struct i2c_adapter *adapter;	
	struct i2c_driver *driver;	
	int usage_count;		
					
	struct device dev;		
	struct list_head list;
	char name[I2C_NAME_SIZE];
	struct completion released;
};
#define to_i2c_client(d) container_of(d, struct i2c_client, dev)

static inline struct i2c_client *kobj_to_i2c_client(struct kobject *kobj)
{
	return to_i2c_client(container_of(kobj, struct device, kobj));
}

static inline void *i2c_get_clientdata (struct i2c_client *dev)
{
	return dev_get_drvdata (&dev->dev);
}

static inline void i2c_set_clientdata (struct i2c_client *dev, void *data)
{
	dev_set_drvdata (&dev->dev, data);
}

struct i2c_algorithm {
	int (*master_xfer)(struct i2c_adapter *adap,struct i2c_msg *msgs, 
	                   int num);
	int (*smbus_xfer) (struct i2c_adapter *adap, u16 addr, 
	                   unsigned short flags, char read_write,
	                   u8 command, int size, union i2c_smbus_data * data);

	
	int (*slave_send)(struct i2c_adapter *,char*,int);
	int (*slave_recv)(struct i2c_adapter *,char*,int);

	
	int (*algo_control)(struct i2c_adapter *, unsigned int, unsigned long);

	
	u32 (*functionality) (struct i2c_adapter *);
};

struct i2c_adapter {
	struct module *owner;
	unsigned int id;
	unsigned int class;
	struct i2c_algorithm *algo;
	void *algo_data;

	
	int (*client_register)(struct i2c_client *);
	int (*client_unregister)(struct i2c_client *);

	
	struct mutex bus_lock;
	struct mutex clist_lock;

	int timeout;
	int retries;
	struct device dev;		
	struct class_device class_dev;	

	int nr;
	struct list_head clients;
	struct list_head list;
	char name[I2C_NAME_SIZE];
	struct completion dev_released;
	struct completion class_dev_released;
};
#define dev_to_i2c_adapter(d) container_of(d, struct i2c_adapter, dev)
#define class_dev_to_i2c_adapter(d) container_of(d, struct i2c_adapter, class_dev)

static inline void *i2c_get_adapdata (struct i2c_adapter *dev)
{
	return dev_get_drvdata (&dev->dev);
}

static inline void i2c_set_adapdata (struct i2c_adapter *dev, void *data)
{
	dev_set_drvdata (&dev->dev, data);
}

#define I2C_CLIENT_PEC  0x04			
#define I2C_CLIENT_TEN	0x10			
						

#define I2C_CLASS_HWMON		(1<<0)	
#define I2C_CLASS_TV_ANALOG	(1<<1)	
#define I2C_CLASS_TV_DIGITAL	(1<<2)	
#define I2C_CLASS_DDC		(1<<3)	
#define I2C_CLASS_CAM_ANALOG	(1<<4)	
#define I2C_CLASS_CAM_DIGITAL	(1<<5)	
#define I2C_CLASS_SOUND		(1<<6)	
#define I2C_CLASS_ALL		(UINT_MAX) 

struct i2c_client_address_data {
	unsigned short *normal_i2c;
	unsigned short *probe;
	unsigned short *ignore;
	unsigned short **forces;
};

#define I2C_CLIENT_END		0xfffeU

#define ANY_I2C_BUS		0xffff
#define ANY_I2C_ISA_BUS		9191



extern int i2c_add_adapter(struct i2c_adapter *);
extern int i2c_del_adapter(struct i2c_adapter *);

extern int i2c_register_driver(struct module *, struct i2c_driver *);
extern int i2c_del_driver(struct i2c_driver *);

static inline int i2c_add_driver(struct i2c_driver *driver)
{
	return i2c_register_driver(THIS_MODULE, driver);
}

extern int i2c_attach_client(struct i2c_client *);
extern int i2c_detach_client(struct i2c_client *);

extern int i2c_use_client(struct i2c_client *);
extern int i2c_release_client(struct i2c_client *);

extern void i2c_clients_command(struct i2c_adapter *adap,
				unsigned int cmd, void *arg);

extern int i2c_check_addr (struct i2c_adapter *adapter, int addr);

extern int i2c_probe(struct i2c_adapter *adapter, 
		struct i2c_client_address_data *address_data,
		int (*found_proc) (struct i2c_adapter *, int, int));

extern int i2c_control(struct i2c_client *,unsigned int, unsigned long);

extern struct i2c_adapter* i2c_get_adapter(int id);
extern void i2c_put_adapter(struct i2c_adapter *adap);


static inline u32 i2c_get_functionality(struct i2c_adapter *adap)
{
	return adap->algo->functionality(adap);
}

static inline int i2c_check_functionality(struct i2c_adapter *adap, u32 func)
{
	return (func & i2c_get_functionality(adap)) == func;
}

static inline int i2c_adapter_id(struct i2c_adapter *adap)
{
	return adap->nr;
}
#endif 

struct i2c_msg {
	__u16 addr;	
 	__u16 flags;		
#define I2C_M_TEN	0x10	
#define I2C_M_RD	0x01
#define I2C_M_NOSTART	0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
#define I2C_M_IGNORE_NAK	0x1000
#define I2C_M_NO_RD_ACK		0x0800
 	__u16 len;		
 	__u8 *buf;		
};


#define I2C_FUNC_I2C			0x00000001
#define I2C_FUNC_10BIT_ADDR		0x00000002
#define I2C_FUNC_PROTOCOL_MANGLING	0x00000004 
#define I2C_FUNC_SMBUS_HWPEC_CALC	0x00000008 
#define I2C_FUNC_SMBUS_BLOCK_PROC_CALL	0x00008000 
#define I2C_FUNC_SMBUS_QUICK		0x00010000 
#define I2C_FUNC_SMBUS_READ_BYTE	0x00020000 
#define I2C_FUNC_SMBUS_WRITE_BYTE	0x00040000 
#define I2C_FUNC_SMBUS_READ_BYTE_DATA	0x00080000 
#define I2C_FUNC_SMBUS_WRITE_BYTE_DATA	0x00100000 
#define I2C_FUNC_SMBUS_READ_WORD_DATA	0x00200000 
#define I2C_FUNC_SMBUS_WRITE_WORD_DATA	0x00400000 
#define I2C_FUNC_SMBUS_PROC_CALL	0x00800000 
#define I2C_FUNC_SMBUS_READ_BLOCK_DATA	0x01000000 
#define I2C_FUNC_SMBUS_WRITE_BLOCK_DATA 0x02000000 
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK	0x04000000 
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK	0x08000000 
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK_2	 0x10000000 
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK_2 0x20000000 

#define I2C_FUNC_SMBUS_BYTE (I2C_FUNC_SMBUS_READ_BYTE | \
                             I2C_FUNC_SMBUS_WRITE_BYTE)
#define I2C_FUNC_SMBUS_BYTE_DATA (I2C_FUNC_SMBUS_READ_BYTE_DATA | \
                                  I2C_FUNC_SMBUS_WRITE_BYTE_DATA)
#define I2C_FUNC_SMBUS_WORD_DATA (I2C_FUNC_SMBUS_READ_WORD_DATA | \
                                  I2C_FUNC_SMBUS_WRITE_WORD_DATA)
#define I2C_FUNC_SMBUS_BLOCK_DATA (I2C_FUNC_SMBUS_READ_BLOCK_DATA | \
                                   I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)
#define I2C_FUNC_SMBUS_I2C_BLOCK (I2C_FUNC_SMBUS_READ_I2C_BLOCK | \
                                  I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)
#define I2C_FUNC_SMBUS_I2C_BLOCK_2 (I2C_FUNC_SMBUS_READ_I2C_BLOCK_2 | \
                                    I2C_FUNC_SMBUS_WRITE_I2C_BLOCK_2)

#define I2C_FUNC_SMBUS_EMUL (I2C_FUNC_SMBUS_QUICK | \
                             I2C_FUNC_SMBUS_BYTE | \
                             I2C_FUNC_SMBUS_BYTE_DATA | \
                             I2C_FUNC_SMBUS_WORD_DATA | \
                             I2C_FUNC_SMBUS_PROC_CALL | \
                             I2C_FUNC_SMBUS_WRITE_BLOCK_DATA | \
                             I2C_FUNC_SMBUS_I2C_BLOCK)

#define I2C_SMBUS_BLOCK_MAX	32		
union i2c_smbus_data {
	__u8 byte;
	__u16 word;
	__u8 block[I2C_SMBUS_BLOCK_MAX + 2]; 
	                       
};

#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0

#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2 
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_DATA    6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		


				
#define I2C_RETRIES	0x0701	
				
                                
#define I2C_TIMEOUT	0x0702	


#define I2C_SLAVE	0x0703	
				
#define I2C_SLAVE_FORCE	0x0706	
				
				
				
#define I2C_TENBIT	0x0704	

#define I2C_FUNCS	0x0705	
#define I2C_RDWR	0x0707	
#define I2C_PEC		0x0708	

#define I2C_SMBUS	0x0720	

#ifdef __KERNEL__

#define I2C_MAJOR	89		

#define I2C_CLIENT_MAX_OPTS 48

#define I2C_CLIENT_DEFAULTS {I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END}


#define I2C_CLIENT_MODULE_PARM(var,desc) \
  static unsigned short var[I2C_CLIENT_MAX_OPTS] = I2C_CLIENT_DEFAULTS; \
  static unsigned int var##_num; \
  module_param_array(var, short, &var##_num, 0); \
  MODULE_PARM_DESC(var,desc)

#define I2C_CLIENT_MODULE_PARM_FORCE(name)				\
I2C_CLIENT_MODULE_PARM(force_##name,					\
		       "List of adapter,address pairs which are "	\
		       "unquestionably assumed to contain a `"		\
		       # name "' chip")


#define I2C_CLIENT_INSMOD_COMMON					\
I2C_CLIENT_MODULE_PARM(probe, "List of adapter,address pairs to scan "	\
		       "additionally");					\
I2C_CLIENT_MODULE_PARM(ignore, "List of adapter,address pairs not to "	\
		       "scan");						\
static struct i2c_client_address_data addr_data = {			\
	.normal_i2c	= normal_i2c,					\
	.probe		= probe,					\
	.ignore		= ignore,					\
	.forces		= forces,					\
}

#define I2C_CLIENT_INSMOD \
  I2C_CLIENT_MODULE_PARM(force, \
                      "List of adapter,address pairs to boldly assume " \
                      "to be present"); \
	static unsigned short *forces[] = {				\
			force,						\
			NULL						\
		};							\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_1(chip1)					\
enum chips { any_chip, chip1 };						\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
static unsigned short *forces[] = { force, force_##chip1, NULL };	\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_2(chip1, chip2)				\
enum chips { any_chip, chip1, chip2 };					\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, NULL };		\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_3(chip1, chip2, chip3)			\
enum chips { any_chip, chip1, chip2, chip3 };				\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    NULL };				\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_4(chip1, chip2, chip3, chip4)			\
enum chips { any_chip, chip1, chip2, chip3, chip4 };			\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, NULL};		\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_5(chip1, chip2, chip3, chip4, chip5)		\
enum chips { any_chip, chip1, chip2, chip3, chip4, chip5 };		\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip5);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, force_##chip5,	\
				    NULL };				\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_6(chip1, chip2, chip3, chip4, chip5, chip6)	\
enum chips { any_chip, chip1, chip2, chip3, chip4, chip5, chip6 };	\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip5);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip6);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, force_##chip5,	\
				    force_##chip6, NULL };		\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_7(chip1, chip2, chip3, chip4, chip5, chip6, chip7) \
enum chips { any_chip, chip1, chip2, chip3, chip4, chip5, chip6,	\
	     chip7 };							\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip5);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip6);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip7);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, force_##chip5,	\
				    force_##chip6, force_##chip7,	\
				    NULL };				\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_8(chip1, chip2, chip3, chip4, chip5, chip6, chip7, chip8) \
enum chips { any_chip, chip1, chip2, chip3, chip4, chip5, chip6,	\
	     chip7, chip8 };						\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip5);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip6);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip7);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip8);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, force_##chip5,	\
				    force_##chip6, force_##chip7,	\
				    force_##chip8, NULL };		\
I2C_CLIENT_INSMOD_COMMON
#endif 
#endif 
