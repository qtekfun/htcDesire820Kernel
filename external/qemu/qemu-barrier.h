#ifndef __QEMU_BARRIER_H
#define __QEMU_BARRIER_H 1

#define smp_wmb()   asm volatile("" ::: "memory")

#define barrier()   asm volatile("" ::: "memory")

#endif
