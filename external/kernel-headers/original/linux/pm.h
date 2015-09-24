/*
 *  pm.h - Power management interface
 *
 *  Copyright (C) 2000 Andrew Henroid
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LINUX_PM_H
#define _LINUX_PM_H

#ifdef __KERNEL__

#include <linux/list.h>
#include <asm/atomic.h>

typedef int __bitwise pm_request_t;

#define PM_SUSPEND	((__force pm_request_t) 1)	
#define PM_RESUME	((__force pm_request_t) 2)	


typedef int __bitwise pm_dev_t;

#define PM_UNKNOWN_DEV	((__force pm_dev_t) 0)	
#define PM_SYS_DEV	((__force pm_dev_t) 1)	
#define PM_PCI_DEV	((__force pm_dev_t) 2)	
#define PM_USB_DEV	((__force pm_dev_t) 3)	
#define PM_SCSI_DEV	((__force pm_dev_t) 4)	
#define PM_ISA_DEV	((__force pm_dev_t) 5)	
#define	PM_MTD_DEV	((__force pm_dev_t) 6)	

enum
{
	PM_SYS_UNKNOWN = 0x00000000, 
	PM_SYS_KBC =	 0x41d00303, 
	PM_SYS_COM =	 0x41d00500, 
	PM_SYS_IRDA =	 0x41d00510, 
	PM_SYS_FDC =	 0x41d00700, 
	PM_SYS_VGA =	 0x41d00900, 
	PM_SYS_PCMCIA =	 0x41d00e00, 
};

#define PM_PCI_ID(dev) ((dev)->bus->number << 16 | (dev)->devfn)

struct pm_dev;

typedef int (*pm_callback)(struct pm_dev *dev, pm_request_t rqst, void *data);

struct pm_dev
{
	pm_dev_t	 type;
	unsigned long	 id;
	pm_callback	 callback;
	void		*data;

	unsigned long	 flags;
	unsigned long	 state;
	unsigned long	 prev_state;

	struct list_head entry;
};


extern void (*pm_idle)(void);
extern void (*pm_power_off)(void);

typedef int __bitwise suspend_state_t;

#define PM_SUSPEND_ON		((__force suspend_state_t) 0)
#define PM_SUSPEND_STANDBY	((__force suspend_state_t) 1)
#define PM_SUSPEND_MEM		((__force suspend_state_t) 3)
#define PM_SUSPEND_DISK		((__force suspend_state_t) 4)
#define PM_SUSPEND_MAX		((__force suspend_state_t) 5)

typedef int __bitwise suspend_disk_method_t;

#define	PM_DISK_FIRMWARE	((__force suspend_disk_method_t) 1)
#define	PM_DISK_PLATFORM	((__force suspend_disk_method_t) 2)
#define	PM_DISK_SHUTDOWN	((__force suspend_disk_method_t) 3)
#define	PM_DISK_REBOOT		((__force suspend_disk_method_t) 4)
#define	PM_DISK_MAX		((__force suspend_disk_method_t) 5)

struct pm_ops {
	suspend_disk_method_t pm_disk_mode;
	int (*valid)(suspend_state_t state);
	int (*prepare)(suspend_state_t state);
	int (*enter)(suspend_state_t state);
	int (*finish)(suspend_state_t state);
};

extern void pm_set_ops(struct pm_ops *);
extern struct pm_ops *pm_ops;
extern int pm_suspend(suspend_state_t state);



struct device;

typedef struct pm_message {
	int event;
} pm_message_t;


#define PM_EVENT_ON 0
#define PM_EVENT_FREEZE 1
#define PM_EVENT_SUSPEND 2

#define PMSG_FREEZE	((struct pm_message){ .event = PM_EVENT_FREEZE, })
#define PMSG_SUSPEND	((struct pm_message){ .event = PM_EVENT_SUSPEND, })
#define PMSG_ON		((struct pm_message){ .event = PM_EVENT_ON, })

struct dev_pm_info {
	pm_message_t		power_state;
	unsigned		can_wakeup:1;
#ifdef	CONFIG_PM
	unsigned		should_wakeup:1;
	pm_message_t		prev_state;
	void			* saved_state;
	struct device		* pm_parent;
	struct list_head	entry;
#endif
};

extern void device_pm_set_parent(struct device * dev, struct device * parent);

extern int device_power_down(pm_message_t state);
extern void device_power_up(void);
extern void device_resume(void);

#ifdef CONFIG_PM
extern suspend_disk_method_t pm_disk_mode;

extern int device_suspend(pm_message_t state);

#define device_set_wakeup_enable(dev,val) \
	((dev)->power.should_wakeup = !!(val))
#define device_may_wakeup(dev) \
	(device_can_wakeup(dev) && (dev)->power.should_wakeup)

extern int dpm_runtime_suspend(struct device *, pm_message_t);
extern void dpm_runtime_resume(struct device *);
extern void __suspend_report_result(const char *function, void *fn, int ret);

#define suspend_report_result(fn, ret)					\
	do {								\
		__suspend_report_result(__FUNCTION__, fn, ret);		\
	} while (0)

#else 

static inline int device_suspend(pm_message_t state)
{
	return 0;
}

#define device_set_wakeup_enable(dev,val)	do{}while(0)
#define device_may_wakeup(dev)			(0)

static inline int dpm_runtime_suspend(struct device * dev, pm_message_t state)
{
	return 0;
}

static inline void dpm_runtime_resume(struct device * dev)
{
}

#define suspend_report_result(fn, ret) do { } while (0)

#endif

#define device_can_wakeup(dev) \
	((dev)->power.can_wakeup)
#define device_init_wakeup(dev,val) \
	do { \
		device_can_wakeup(dev) = !!(val); \
		device_set_wakeup_enable(dev,val); \
	} while(0)

#endif 

#endif 
