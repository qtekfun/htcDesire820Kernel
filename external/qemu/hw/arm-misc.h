/*
 * Misc ARM declarations
 *
 * Copyright (c) 2006 CodeSourcery.
 * Written by Paul Brook
 *
 * This code is licenced under the LGPL.
 *
 */

#ifndef ARM_MISC_H
#define ARM_MISC_H 1

#define ARM_PIC_CPU_IRQ 0
#define ARM_PIC_CPU_FIQ 1
qemu_irq *arm_pic_init_cpu(CPUState *env);

qemu_irq *armv7m_init(int flash_size, int sram_size,
                      const char *kernel_filename, const char *cpu_model);

struct arm_boot_info {
    int ram_size;
    const char *kernel_filename;
    const char *kernel_cmdline;
    const char *initrd_filename;
    target_phys_addr_t loader_start;
    target_phys_addr_t smp_loader_start;
    target_phys_addr_t smp_priv_base;
    int nb_cpus;
    int board_id;
    int (*atag_board)(struct arm_boot_info *info, void *p);
    
    int is_linux;
    target_phys_addr_t initrd_size;
    target_phys_addr_t entry;
};
void arm_load_kernel(CPUState *env, struct arm_boot_info *info);

extern int system_clock_scale;

#endif 
