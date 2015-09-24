#ifndef QDEV_H
#define QDEV_H

#include "hw.h"
#include "qemu-queue.h"

typedef struct DeviceType DeviceType;

typedef struct DeviceProperty DeviceProperty;

typedef struct BusState BusState;

struct DeviceState {
    DeviceType *type;
    BusState *parent_bus;
    DeviceProperty *props;
    int num_gpio_out;
    qemu_irq *gpio_out;
    int num_gpio_in;
    qemu_irq *gpio_in;
    QLIST_HEAD(, BusState) child_bus;
    NICInfo *nd;
    QLIST_ENTRY(DeviceState) sibling;
};

typedef enum {
    BUS_TYPE_SYSTEM,
    BUS_TYPE_PCI,
    BUS_TYPE_SCSI,
    BUS_TYPE_I2C,
    BUS_TYPE_SSI
} BusType;

struct BusState {
    DeviceState *parent;
    const char *name;
    BusType type;
    QLIST_HEAD(, DeviceState) children;
    QLIST_ENTRY(BusState) sibling;
};


DeviceState *qdev_create(BusState *bus, const char *name);
void qdev_init(DeviceState *dev);
void qdev_free(DeviceState *dev);

void qdev_set_prop_int(DeviceState *dev, const char *name, uint64_t value);
void qdev_set_prop_dev(DeviceState *dev, const char *name, DeviceState *value);
void qdev_set_prop_ptr(DeviceState *dev, const char *name, void *value);
void qdev_set_netdev(DeviceState *dev, NICInfo *nd);

qemu_irq qdev_get_gpio_in(DeviceState *dev, int n);
void qdev_connect_gpio_out(DeviceState *dev, int n, qemu_irq pin);

BusState *qdev_get_child_bus(DeviceState *dev, const char *name);


typedef enum {
    PROP_TYPE_INT,
    PROP_TYPE_PTR,
    PROP_TYPE_DEV
} DevicePropType;

typedef struct {
    const char *name;
    DevicePropType type;
} DevicePropList;

typedef struct DeviceInfo DeviceInfo;

typedef void (*qdev_initfn)(DeviceState *dev, DeviceInfo *info);
typedef void (*SCSIAttachFn)(DeviceState *host, BlockDriverState *bdrv,
              int unit);

struct DeviceInfo {
    const char *name;
    size_t size;
    DevicePropList *props;

    
    qdev_initfn init;
    BusType bus_type;
};

void qdev_register(DeviceInfo *info);

void qdev_init_gpio_in(DeviceState *dev, qemu_irq_handler handler, int n);
void qdev_init_gpio_out(DeviceState *dev, qemu_irq *pins, int n);

void scsi_bus_new(DeviceState *host, SCSIAttachFn attach);

CharDriverState *qdev_init_chardev(DeviceState *dev);

BusState *qdev_get_parent_bus(DeviceState *dev);
uint64_t qdev_get_prop_int(DeviceState *dev, const char *name, uint64_t def);
DeviceState *qdev_get_prop_dev(DeviceState *dev, const char *name);
void *qdev_get_prop_ptr(DeviceState *dev, const char *name);

#ifdef __GNUC__
#define DO_UPCAST(type, field, dev) ( __extension__ ( { \
    char __attribute__((unused)) offset_must_be_zero[ \
        -offsetof(type, field)]; \
    container_of(dev, type, field);}))
#else
#define DO_UPCAST(type, field, dev) container_of(dev, type, field)
#endif


BusState *qbus_create(BusType type, size_t size,
                      DeviceState *parent, const char *name);

#define FROM_QBUS(type, dev) DO_UPCAST(type, qbus, dev)


void do_info_qtree(Monitor *mon);
void sysbus_dev_print(Monitor *mon, DeviceState *dev, int indent);

#endif
