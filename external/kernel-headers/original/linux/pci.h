/*
 *	pci.h
 *
 *	PCI defines and function prototypes
 *	Copyright 1994, Drew Eckhardt
 *	Copyright 1997--1999 Martin Mares <mj@ucw.cz>
 *
 *	For more information, please consult the following manuals (look at
 *	http://www.pcisig.com/ for how to get them):
 *
 *	PCI BIOS Specification
 *	PCI Local Bus Specification
 *	PCI to PCI Bridge Specification
 *	PCI System Design Guide
 */

#ifndef LINUX_PCI_H
#define LINUX_PCI_H

#include <linux/pci_regs.h>

#include <linux/pci_ids.h>

#define PCI_DEVFN(slot,func)	((((slot) & 0x1f) << 3) | ((func) & 0x07))
#define PCI_SLOT(devfn)		(((devfn) >> 3) & 0x1f)
#define PCI_FUNC(devfn)		((devfn) & 0x07)

#define PCIIOC_BASE		('P' << 24 | 'C' << 16 | 'I' << 8)
#define PCIIOC_CONTROLLER	(PCIIOC_BASE | 0x00)	
#define PCIIOC_MMAP_IS_IO	(PCIIOC_BASE | 0x01)	
#define PCIIOC_MMAP_IS_MEM	(PCIIOC_BASE | 0x02)	
#define PCIIOC_WRITE_COMBINE	(PCIIOC_BASE | 0x03)	

#ifdef __KERNEL__

#include <linux/mod_devicetable.h>

#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/device.h>

enum pci_mmap_state {
	pci_mmap_io,
	pci_mmap_mem
};

#define PCI_DMA_BIDIRECTIONAL	0
#define PCI_DMA_TODEVICE	1
#define PCI_DMA_FROMDEVICE	2
#define PCI_DMA_NONE		3

#define DEVICE_COUNT_COMPATIBLE	4
#define DEVICE_COUNT_RESOURCE	12

typedef int __bitwise pci_power_t;

#define PCI_D0		((pci_power_t __force) 0)
#define PCI_D1		((pci_power_t __force) 1)
#define PCI_D2		((pci_power_t __force) 2)
#define PCI_D3hot	((pci_power_t __force) 3)
#define PCI_D3cold	((pci_power_t __force) 4)
#define PCI_UNKNOWN	((pci_power_t __force) 5)
#define PCI_POWER_ERROR	((pci_power_t __force) -1)

typedef unsigned int __bitwise pci_channel_state_t;

enum pci_channel_state {
	
	pci_channel_io_normal = (__force pci_channel_state_t) 1,

	
	pci_channel_io_frozen = (__force pci_channel_state_t) 2,

	
	pci_channel_io_perm_failure = (__force pci_channel_state_t) 3,
};

typedef unsigned short __bitwise pci_bus_flags_t;
enum pci_bus_flags {
	PCI_BUS_FLAGS_NO_MSI = (__force pci_bus_flags_t) 1,
};

struct pci_cap_saved_state {
	struct hlist_node next;
	char cap_nr;
	u32 data[0];
};

struct pci_dev {
	struct list_head global_list;	
	struct list_head bus_list;	
	struct pci_bus	*bus;		
	struct pci_bus	*subordinate;	

	void		*sysdata;	
	struct proc_dir_entry *procent;	

	unsigned int	devfn;		
	unsigned short	vendor;
	unsigned short	device;
	unsigned short	subsystem_vendor;
	unsigned short	subsystem_device;
	unsigned int	class;		
	u8		hdr_type;	
	u8		rom_base_reg;	
	u8		pin;  		

	struct pci_driver *driver;	
	u64		dma_mask;	

	pci_power_t     current_state;  

	pci_channel_state_t error_state;	
	struct	device	dev;		

	
	unsigned short vendor_compatible[DEVICE_COUNT_COMPATIBLE];
	unsigned short device_compatible[DEVICE_COUNT_COMPATIBLE];

	int		cfg_size;	

	unsigned int	irq;
	struct resource resource[DEVICE_COUNT_RESOURCE]; 

	
	unsigned int	transparent:1;	
	unsigned int	multifunction:1;
	
	unsigned int	is_enabled:1;	
	unsigned int	is_busmaster:1; 
	unsigned int	no_msi:1;	
	unsigned int	no_d1d2:1;   
	unsigned int	block_ucfg_access:1;	
	unsigned int	broken_parity_status:1;	
	unsigned int 	msi_enabled:1;
	unsigned int	msix_enabled:1;

	u32		saved_config_space[16]; 
	struct hlist_head saved_cap_space;
	struct bin_attribute *rom_attr; 
	int rom_attr_enabled;		
	struct bin_attribute *res_attr[DEVICE_COUNT_RESOURCE]; 
};

#define pci_dev_g(n) list_entry(n, struct pci_dev, global_list)
#define pci_dev_b(n) list_entry(n, struct pci_dev, bus_list)
#define	to_pci_dev(n) container_of(n, struct pci_dev, dev)
#define for_each_pci_dev(d) while ((d = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, d)) != NULL)

static inline struct pci_cap_saved_state *pci_find_saved_cap(
	struct pci_dev *pci_dev,char cap)
{
	struct pci_cap_saved_state *tmp;
	struct hlist_node *pos;

	hlist_for_each_entry(tmp, pos, &pci_dev->saved_cap_space, next) {
		if (tmp->cap_nr == cap)
			return tmp;
	}
	return NULL;
}

static inline void pci_add_saved_cap(struct pci_dev *pci_dev,
	struct pci_cap_saved_state *new_cap)
{
	hlist_add_head(&new_cap->next, &pci_dev->saved_cap_space);
}

static inline void pci_remove_saved_cap(struct pci_cap_saved_state *cap)
{
	hlist_del(&cap->next);
}


#define PCI_ROM_RESOURCE	6
#define PCI_BRIDGE_RESOURCES	7
#define PCI_NUM_RESOURCES	11

#ifndef PCI_BUS_NUM_RESOURCES
#define PCI_BUS_NUM_RESOURCES	8
#endif

#define PCI_REGION_FLAG_MASK	0x0fU	

struct pci_bus {
	struct list_head node;		
	struct pci_bus	*parent;	
	struct list_head children;	
	struct list_head devices;	
	struct pci_dev	*self;		
	struct resource	*resource[PCI_BUS_NUM_RESOURCES];
					

	struct pci_ops	*ops;		
	void		*sysdata;	
	struct proc_dir_entry *procdir;	

	unsigned char	number;		
	unsigned char	primary;	
	unsigned char	secondary;	
	unsigned char	subordinate;	

	char		name[48];

	unsigned short  bridge_ctl;	
	pci_bus_flags_t bus_flags;	
	struct device		*bridge;
	struct class_device	class_dev;
	struct bin_attribute	*legacy_io; 
	struct bin_attribute	*legacy_mem; 
};

#define pci_bus_b(n)	list_entry(n, struct pci_bus, node)
#define to_pci_bus(n)	container_of(n, struct pci_bus, class_dev)

#define PCIBIOS_SUCCESSFUL		0x00
#define PCIBIOS_FUNC_NOT_SUPPORTED	0x81
#define PCIBIOS_BAD_VENDOR_ID		0x83
#define PCIBIOS_DEVICE_NOT_FOUND	0x86
#define PCIBIOS_BAD_REGISTER_NUMBER	0x87
#define PCIBIOS_SET_FAILED		0x88
#define PCIBIOS_BUFFER_TOO_SMALL	0x89


struct pci_ops {
	int (*read)(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 *val);
	int (*write)(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 val);
};

struct pci_raw_ops {
	int (*read)(unsigned int domain, unsigned int bus, unsigned int devfn,
		    int reg, int len, u32 *val);
	int (*write)(unsigned int domain, unsigned int bus, unsigned int devfn,
		     int reg, int len, u32 val);
};

extern struct pci_raw_ops *raw_pci_ops;

struct pci_bus_region {
	unsigned long start;
	unsigned long end;
};

struct pci_dynids {
	spinlock_t lock;            
	struct list_head list;      
	unsigned int use_driver_data:1; 
};


typedef unsigned int __bitwise pci_ers_result_t;

enum pci_ers_result {
	
	PCI_ERS_RESULT_NONE = (__force pci_ers_result_t) 1,

	
	PCI_ERS_RESULT_CAN_RECOVER = (__force pci_ers_result_t) 2,

	
	PCI_ERS_RESULT_NEED_RESET = (__force pci_ers_result_t) 3,

	
	PCI_ERS_RESULT_DISCONNECT = (__force pci_ers_result_t) 4,

	
	PCI_ERS_RESULT_RECOVERED = (__force pci_ers_result_t) 5,
};

struct pci_error_handlers
{
	
	pci_ers_result_t (*error_detected)(struct pci_dev *dev,
	                      enum pci_channel_state error);

	
	pci_ers_result_t (*mmio_enabled)(struct pci_dev *dev);

	
	pci_ers_result_t (*link_reset)(struct pci_dev *dev);

	
	pci_ers_result_t (*slot_reset)(struct pci_dev *dev);

	
	void (*resume)(struct pci_dev *dev);
};


struct module;
struct pci_driver {
	struct list_head node;
	char *name;
	const struct pci_device_id *id_table;	
	int  (*probe)  (struct pci_dev *dev, const struct pci_device_id *id);	
	void (*remove) (struct pci_dev *dev);	
	int  (*suspend) (struct pci_dev *dev, pm_message_t state);	
	int  (*resume) (struct pci_dev *dev);	                
	int  (*enable_wake) (struct pci_dev *dev, pci_power_t state, int enable);   
	void (*shutdown) (struct pci_dev *dev);

	struct pci_error_handlers *err_handler;
	struct device_driver	driver;
	struct pci_dynids dynids;
};

#define	to_pci_driver(drv) container_of(drv,struct pci_driver, driver)

#define PCI_DEVICE(vend,dev) \
	.vendor = (vend), .device = (dev), \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID

#define PCI_DEVICE_CLASS(dev_class,dev_class_mask) \
	.class = (dev_class), .class_mask = (dev_class_mask), \
	.vendor = PCI_ANY_ID, .device = PCI_ANY_ID, \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID

#define pci_module_init	pci_register_driver

#ifdef CONFIG_PCI

extern struct bus_type pci_bus_type;

extern struct list_head pci_root_buses;	
extern struct list_head pci_devices;	

void pcibios_fixup_bus(struct pci_bus *);
int pcibios_enable_device(struct pci_dev *, int mask);
char *pcibios_setup (char *str);

void pcibios_align_resource(void *, struct resource *, resource_size_t,
				resource_size_t);
void pcibios_update_irq(struct pci_dev *, int irq);


extern struct pci_bus *pci_find_bus(int domain, int busnr);
void pci_bus_add_devices(struct pci_bus *bus);
struct pci_bus *pci_scan_bus_parented(struct device *parent, int bus, struct pci_ops *ops, void *sysdata);
static inline struct pci_bus *pci_scan_bus(int bus, struct pci_ops *ops, void *sysdata)
{
	struct pci_bus *root_bus;
	root_bus = pci_scan_bus_parented(NULL, bus, ops, sysdata);
	if (root_bus)
		pci_bus_add_devices(root_bus);
	return root_bus;
}
struct pci_bus *pci_create_bus(struct device *parent, int bus, struct pci_ops *ops, void *sysdata);
struct pci_bus * pci_add_new_bus(struct pci_bus *parent, struct pci_dev *dev, int busnr);
int pci_scan_slot(struct pci_bus *bus, int devfn);
struct pci_dev * pci_scan_single_device(struct pci_bus *bus, int devfn);
void pci_device_add(struct pci_dev *dev, struct pci_bus *bus);
unsigned int pci_scan_child_bus(struct pci_bus *bus);
void pci_bus_add_device(struct pci_dev *dev);
void pci_read_bridge_bases(struct pci_bus *child);
struct resource *pci_find_parent_resource(const struct pci_dev *dev, struct resource *res);
int pci_get_interrupt_pin(struct pci_dev *dev, struct pci_dev **bridge);
extern struct pci_dev *pci_dev_get(struct pci_dev *dev);
extern void pci_dev_put(struct pci_dev *dev);
extern void pci_remove_bus(struct pci_bus *b);
extern void pci_remove_bus_device(struct pci_dev *dev);
void pci_setup_cardbus(struct pci_bus *bus);


struct pci_dev *pci_find_device (unsigned int vendor, unsigned int device, const struct pci_dev *from);
struct pci_dev *pci_find_device_reverse (unsigned int vendor, unsigned int device, const struct pci_dev *from);
struct pci_dev *pci_find_slot (unsigned int bus, unsigned int devfn);
int pci_find_capability (struct pci_dev *dev, int cap);
int pci_find_next_capability (struct pci_dev *dev, u8 pos, int cap);
int pci_find_ext_capability (struct pci_dev *dev, int cap);
struct pci_bus * pci_find_next_bus(const struct pci_bus *from);

struct pci_dev *pci_get_device (unsigned int vendor, unsigned int device, struct pci_dev *from);
struct pci_dev *pci_get_subsys (unsigned int vendor, unsigned int device,
				unsigned int ss_vendor, unsigned int ss_device,
				struct pci_dev *from);
struct pci_dev *pci_get_slot (struct pci_bus *bus, unsigned int devfn);
struct pci_dev *pci_get_class (unsigned int class, struct pci_dev *from);
int pci_dev_present(const struct pci_device_id *ids);

int pci_bus_read_config_byte (struct pci_bus *bus, unsigned int devfn, int where, u8 *val);
int pci_bus_read_config_word (struct pci_bus *bus, unsigned int devfn, int where, u16 *val);
int pci_bus_read_config_dword (struct pci_bus *bus, unsigned int devfn, int where, u32 *val);
int pci_bus_write_config_byte (struct pci_bus *bus, unsigned int devfn, int where, u8 val);
int pci_bus_write_config_word (struct pci_bus *bus, unsigned int devfn, int where, u16 val);
int pci_bus_write_config_dword (struct pci_bus *bus, unsigned int devfn, int where, u32 val);

static inline int pci_read_config_byte(struct pci_dev *dev, int where, u8 *val)
{
	return pci_bus_read_config_byte (dev->bus, dev->devfn, where, val);
}
static inline int pci_read_config_word(struct pci_dev *dev, int where, u16 *val)
{
	return pci_bus_read_config_word (dev->bus, dev->devfn, where, val);
}
static inline int pci_read_config_dword(struct pci_dev *dev, int where, u32 *val)
{
	return pci_bus_read_config_dword (dev->bus, dev->devfn, where, val);
}
static inline int pci_write_config_byte(struct pci_dev *dev, int where, u8 val)
{
	return pci_bus_write_config_byte (dev->bus, dev->devfn, where, val);
}
static inline int pci_write_config_word(struct pci_dev *dev, int where, u16 val)
{
	return pci_bus_write_config_word (dev->bus, dev->devfn, where, val);
}
static inline int pci_write_config_dword(struct pci_dev *dev, int where, u32 val)
{
	return pci_bus_write_config_dword (dev->bus, dev->devfn, where, val);
}

int pci_enable_device(struct pci_dev *dev);
int pci_enable_device_bars(struct pci_dev *dev, int mask);
void pci_disable_device(struct pci_dev *dev);
void pci_set_master(struct pci_dev *dev);
#define HAVE_PCI_SET_MWI
int pci_set_mwi(struct pci_dev *dev);
void pci_clear_mwi(struct pci_dev *dev);
void pci_intx(struct pci_dev *dev, int enable);
int pci_set_dma_mask(struct pci_dev *dev, u64 mask);
int pci_set_consistent_dma_mask(struct pci_dev *dev, u64 mask);
void pci_update_resource(struct pci_dev *dev, struct resource *res, int resno);
int pci_assign_resource(struct pci_dev *dev, int i);
int pci_assign_resource_fixed(struct pci_dev *dev, int i);
void pci_restore_bars(struct pci_dev *dev);

void __iomem __must_check *pci_map_rom(struct pci_dev *pdev, size_t *size);
void __iomem __must_check *pci_map_rom_copy(struct pci_dev *pdev, size_t *size);
void pci_unmap_rom(struct pci_dev *pdev, void __iomem *rom);
void pci_remove_rom(struct pci_dev *pdev);

int pci_save_state(struct pci_dev *dev);
int pci_restore_state(struct pci_dev *dev);
int pci_set_power_state(struct pci_dev *dev, pci_power_t state);
pci_power_t pci_choose_state(struct pci_dev *dev, pm_message_t state);
int pci_enable_wake(struct pci_dev *dev, pci_power_t state, int enable);

void pci_bus_assign_resources(struct pci_bus *bus);
void pci_bus_size_bridges(struct pci_bus *bus);
int pci_claim_resource(struct pci_dev *, int);
void pci_assign_unassigned_resources(void);
void pdev_enable_device(struct pci_dev *);
void pdev_sort_resources(struct pci_dev *, struct resource_list *);
void pci_fixup_irqs(u8 (*)(struct pci_dev *, u8 *),
		    int (*)(struct pci_dev *, u8, u8));
#define HAVE_PCI_REQ_REGIONS	2
int pci_request_regions(struct pci_dev *, const char *);
void pci_release_regions(struct pci_dev *);
int pci_request_region(struct pci_dev *, int, const char *);
void pci_release_region(struct pci_dev *, int);

int pci_bus_alloc_resource(struct pci_bus *bus, struct resource *res,
			   resource_size_t size, resource_size_t align,
			   resource_size_t min, unsigned int type_mask,
			   void (*alignf)(void *, struct resource *,
					  resource_size_t, resource_size_t),
			   void *alignf_data);
void pci_enable_bridges(struct pci_bus *bus);

int __pci_register_driver(struct pci_driver *, struct module *);
static inline int pci_register_driver(struct pci_driver *driver)
{
	return __pci_register_driver(driver, THIS_MODULE);
}

void pci_unregister_driver(struct pci_driver *);
void pci_remove_behind_bridge(struct pci_dev *);
struct pci_driver *pci_dev_driver(const struct pci_dev *);
const struct pci_device_id *pci_match_device(struct pci_driver *drv, struct pci_dev *dev);
const struct pci_device_id *pci_match_id(const struct pci_device_id *ids, struct pci_dev *dev);
int pci_scan_bridge(struct pci_bus *bus, struct pci_dev * dev, int max, int pass);

void pci_walk_bus(struct pci_bus *top, void (*cb)(struct pci_dev *, void *),
		  void *userdata);
int pci_cfg_space_size(struct pci_dev *dev);
unsigned char pci_bus_max_busnr(struct pci_bus* bus);


#include <linux/dmapool.h>

#define	pci_pool dma_pool
#define pci_pool_create(name, pdev, size, align, allocation) \
		dma_pool_create(name, &pdev->dev, size, align, allocation)
#define	pci_pool_destroy(pool) dma_pool_destroy(pool)
#define	pci_pool_alloc(pool, flags, handle) dma_pool_alloc(pool, flags, handle)
#define	pci_pool_free(pool, vaddr, addr) dma_pool_free(pool, vaddr, addr)

enum pci_dma_burst_strategy {
	PCI_DMA_BURST_INFINITY,	
	PCI_DMA_BURST_BOUNDARY, 
	PCI_DMA_BURST_MULTIPLE, 
};

#if defined(CONFIG_ISA) || defined(CONFIG_EISA)
extern struct pci_dev *isa_bridge;
#endif

struct msix_entry {
	u16 	vector;	
	u16	entry;	
};

#ifndef CONFIG_PCI_MSI
static inline void pci_scan_msi_device(struct pci_dev *dev) {}
static inline int pci_enable_msi(struct pci_dev *dev) {return -1;}
static inline void pci_disable_msi(struct pci_dev *dev) {}
static inline int pci_enable_msix(struct pci_dev* dev,
	struct msix_entry *entries, int nvec) {return -1;}
static inline void pci_disable_msix(struct pci_dev *dev) {}
static inline void msi_remove_pci_irq_vectors(struct pci_dev *dev) {}
#else
extern void pci_scan_msi_device(struct pci_dev *dev);
extern int pci_enable_msi(struct pci_dev *dev);
extern void pci_disable_msi(struct pci_dev *dev);
extern int pci_enable_msix(struct pci_dev* dev,
	struct msix_entry *entries, int nvec);
extern void pci_disable_msix(struct pci_dev *dev);
extern void msi_remove_pci_irq_vectors(struct pci_dev *dev);
#endif

extern void pci_block_user_cfg_access(struct pci_dev *dev);
extern void pci_unblock_user_cfg_access(struct pci_dev *dev);

#ifndef CONFIG_PCI_DOMAINS
static inline int pci_domain_nr(struct pci_bus *bus) { return 0; }
static inline int pci_proc_domain(struct pci_bus *bus)
{
	return 0;
}
#endif

#else 


#define _PCI_NOP(o,s,t) \
	static inline int pci_##o##_config_##s (struct pci_dev *dev, int where, t val) \
		{ return PCIBIOS_FUNC_NOT_SUPPORTED; }
#define _PCI_NOP_ALL(o,x)	_PCI_NOP(o,byte,u8 x) \
				_PCI_NOP(o,word,u16 x) \
				_PCI_NOP(o,dword,u32 x)
_PCI_NOP_ALL(read, *)
_PCI_NOP_ALL(write,)

static inline struct pci_dev *pci_find_device(unsigned int vendor, unsigned int device, const struct pci_dev *from)
{ return NULL; }

static inline struct pci_dev *pci_find_slot(unsigned int bus, unsigned int devfn)
{ return NULL; }

static inline struct pci_dev *pci_get_device (unsigned int vendor, unsigned int device, struct pci_dev *from)
{ return NULL; }

static inline struct pci_dev *pci_get_subsys (unsigned int vendor, unsigned int device,
unsigned int ss_vendor, unsigned int ss_device, struct pci_dev *from)
{ return NULL; }

static inline struct pci_dev *pci_get_class(unsigned int class, struct pci_dev *from)
{ return NULL; }

#define pci_dev_present(ids)	(0)
#define pci_dev_put(dev)	do { } while (0)

static inline void pci_set_master(struct pci_dev *dev) { }
static inline int pci_enable_device(struct pci_dev *dev) { return -EIO; }
static inline void pci_disable_device(struct pci_dev *dev) { }
static inline int pci_set_dma_mask(struct pci_dev *dev, u64 mask) { return -EIO; }
static inline int pci_assign_resource(struct pci_dev *dev, int i) { return -EBUSY;}
static inline int __pci_register_driver(struct pci_driver *drv, struct module *owner) { return 0;}
static inline int pci_register_driver(struct pci_driver *drv) { return 0;}
static inline void pci_unregister_driver(struct pci_driver *drv) { }
static inline int pci_find_capability (struct pci_dev *dev, int cap) {return 0; }
static inline int pci_find_next_capability (struct pci_dev *dev, u8 post, int cap) { return 0; }
static inline int pci_find_ext_capability (struct pci_dev *dev, int cap) {return 0; }
static inline const struct pci_device_id *pci_match_device(const struct pci_device_id *ids, const struct pci_dev *dev) { return NULL; }

static inline int pci_save_state(struct pci_dev *dev) { return 0; }
static inline int pci_restore_state(struct pci_dev *dev) { return 0; }
static inline int pci_set_power_state(struct pci_dev *dev, pci_power_t state) { return 0; }
static inline pci_power_t pci_choose_state(struct pci_dev *dev, pm_message_t state) { return PCI_D0; }
static inline int pci_enable_wake(struct pci_dev *dev, pci_power_t state, int enable) { return 0; }

#define	isa_bridge	((struct pci_dev *)NULL)

#define pci_dma_burst_advice(pdev, strat, strategy_parameter) do { } while (0)

static inline void pci_block_user_cfg_access(struct pci_dev *dev) { }
static inline void pci_unblock_user_cfg_access(struct pci_dev *dev) { }

#endif 


#include <asm/pci.h>

#define pci_resource_start(dev,bar)   ((dev)->resource[(bar)].start)
#define pci_resource_end(dev,bar)     ((dev)->resource[(bar)].end)
#define pci_resource_flags(dev,bar)   ((dev)->resource[(bar)].flags)
#define pci_resource_len(dev,bar) \
	((pci_resource_start((dev),(bar)) == 0 &&	\
	  pci_resource_end((dev),(bar)) ==		\
	  pci_resource_start((dev),(bar))) ? 0 :	\
	  						\
	 (pci_resource_end((dev),(bar)) -		\
	  pci_resource_start((dev),(bar)) + 1))

static inline void *pci_get_drvdata (struct pci_dev *pdev)
{
	return dev_get_drvdata(&pdev->dev);
}

static inline void pci_set_drvdata (struct pci_dev *pdev, void *data)
{
	dev_set_drvdata(&pdev->dev, data);
}

static inline char *pci_name(struct pci_dev *pdev)
{
	return pdev->dev.bus_id;
}


#ifndef HAVE_ARCH_PCI_RESOURCE_TO_USER
static inline void pci_resource_to_user(const struct pci_dev *dev, int bar,
                const struct resource *rsrc, resource_size_t *start,
		resource_size_t *end)
{
	*start = rsrc->start;
	*end = rsrc->end;
}
#endif 



struct pci_fixup {
	u16 vendor, device;	
	void (*hook)(struct pci_dev *dev);
};

enum pci_fixup_pass {
	pci_fixup_early,	
	pci_fixup_header,	
	pci_fixup_final,	
	pci_fixup_enable,	
};

#define DECLARE_PCI_FIXUP_SECTION(section, name, vendor, device, hook)	\
	static const struct pci_fixup __pci_fixup_##name __attribute_used__ \
	__attribute__((__section__(#section))) = { vendor, device, hook };
#define DECLARE_PCI_FIXUP_EARLY(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_early,			\
			vendor##device##hook, vendor, device, hook)
#define DECLARE_PCI_FIXUP_HEADER(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_header,			\
			vendor##device##hook, vendor, device, hook)
#define DECLARE_PCI_FIXUP_FINAL(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_final,			\
			vendor##device##hook, vendor, device, hook)
#define DECLARE_PCI_FIXUP_ENABLE(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_enable,			\
			vendor##device##hook, vendor, device, hook)


void pci_fixup_device(enum pci_fixup_pass pass, struct pci_dev *dev);

extern int pci_pci_problems;
#define PCIPCI_FAIL		1
#define PCIPCI_TRITON		2
#define PCIPCI_NATOMA		4
#define PCIPCI_VIAETBF		8
#define PCIPCI_VSFX		16
#define PCIPCI_ALIMAGIK		32

#endif 
#endif 
