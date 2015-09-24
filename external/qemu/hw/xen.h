#ifndef QEMU_HW_XEN_H
#define QEMU_HW_XEN_H 1
#include <inttypes.h>

enum xen_mode {
    XEN_EMULATE = 0,  
    XEN_CREATE,       
    XEN_ATTACH        
};

extern uint32_t xen_domid;
extern enum xen_mode xen_mode;

#endif 
