/*
 * ARM kernel loader.
 *
 * Copyright (c) 2006-2007 CodeSourcery.
 * Written by Paul Brook
 *
 * This code is licenced under the GPL.
 */

#include "hw.h"
#include "arm-misc.h"
#include "sysemu.h"

#define KERNEL_ARGS_ADDR 0x100
#define KERNEL_LOAD_ADDR 0x00010000
#define INITRD_LOAD_ADDR 0x00800000

static uint32_t bootloader[] = {
  0xe3a00000, 
  0xe3a01000, 
  0xe3811c00, 
  0xe59f2000, 
  0xe59ff000, 
  0, 
  0  
};

/* Entry point for secondary CPUs.  Enable interrupt controller and
   Issue WFI until start address is written to system controller.  */
static uint32_t smpboot[] = {
  0xe3a00201, 
  0xe3800601, 
  0xe3a01001, 
  0xe5801100, 
  0xe3a00201, 
  0xe3800030, 
  0xe320f003, 
  0xe5901000, 
  0xe3110003, 
  0x1afffffb, 
  0xe12fff11  
};

static void main_cpu_reset(void *opaque)
{
    CPUState *env = opaque;

    cpu_reset(env);
    if (env->boot_info)
        arm_load_kernel(env, env->boot_info);

    
}

#define WRITE_WORD(p, value) do { \
    stl_phys_notdirty(p, value);  \
    p += 4;                       \
} while (0)

static void set_kernel_args(struct arm_boot_info *info,
                int initrd_size, target_phys_addr_t base)
{
    target_phys_addr_t p;

    p = base + KERNEL_ARGS_ADDR;
    
    WRITE_WORD(p, 5);
    WRITE_WORD(p, 0x54410001);
    WRITE_WORD(p, 1);
    WRITE_WORD(p, 0x1000);
    WRITE_WORD(p, 0);
    
    
    WRITE_WORD(p, 4);
    WRITE_WORD(p, 0x54410002);
    WRITE_WORD(p, info->ram_size);
    WRITE_WORD(p, info->loader_start);
    if (initrd_size) {
        
        WRITE_WORD(p, 4);
        WRITE_WORD(p, 0x54420005);
        WRITE_WORD(p, info->loader_start + INITRD_LOAD_ADDR);
        WRITE_WORD(p, initrd_size);
    }
    if (info->kernel_cmdline && *info->kernel_cmdline) {
        
        int cmdline_size;

        cmdline_size = strlen(info->kernel_cmdline);
        cpu_physical_memory_write(p + 8, (void *)info->kernel_cmdline,
                                  cmdline_size + 1);
        cmdline_size = (cmdline_size >> 2) + 1;
        WRITE_WORD(p, cmdline_size + 2);
        WRITE_WORD(p, 0x54410009);
        p += cmdline_size * 4;
    }
    if (info->atag_board) {
        
        int atag_board_len;
        uint8_t atag_board_buf[0x1000];

        atag_board_len = (info->atag_board(info, atag_board_buf) + 3) & ~3;
        WRITE_WORD(p, (atag_board_len + 8) >> 2);
        WRITE_WORD(p, 0x414f4d50);
        cpu_physical_memory_write(p, atag_board_buf, atag_board_len);
        p += atag_board_len;
    }
    
    WRITE_WORD(p, 0);
    WRITE_WORD(p, 0);
}

static void set_kernel_args_old(struct arm_boot_info *info,
                int initrd_size, target_phys_addr_t base)
{
    target_phys_addr_t p;
    const char *s;


    
    p = base + KERNEL_ARGS_ADDR;
    
    WRITE_WORD(p, 4096);
    
    WRITE_WORD(p, info->ram_size / 4096);
    
    WRITE_WORD(p, 0);
#define FLAG_READONLY	1
#define FLAG_RDLOAD	4
#define FLAG_RDPROMPT	8
    
    WRITE_WORD(p, FLAG_READONLY | FLAG_RDLOAD | FLAG_RDPROMPT);
    
    WRITE_WORD(p, (31 << 8) | 0);	
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    
    
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    WRITE_WORD(p, 0);
    WRITE_WORD(p, 0);
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    if (initrd_size)
        WRITE_WORD(p, info->loader_start + INITRD_LOAD_ADDR);
    else
        WRITE_WORD(p, 0);
    
    WRITE_WORD(p, initrd_size);
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    WRITE_WORD(p, 0);
    
    while (p < base + KERNEL_ARGS_ADDR + 256 + 1024) {
        WRITE_WORD(p, 0);
    }
    s = info->kernel_cmdline;
    if (s) {
        cpu_physical_memory_write(p, (void *)s, strlen(s) + 1);
    } else {
        WRITE_WORD(p, 0);
    }
}

void arm_load_kernel(CPUState *env, struct arm_boot_info *info)
{
    int kernel_size;
    int initrd_size;
    int n;
    int is_linux = 0;
    uint64_t elf_entry;
    target_phys_addr_t entry;
    int big_endian;

    
    if (!info->kernel_filename) {
        fprintf(stderr, "Kernel image must be specified\n");
        exit(1);
    }

    if (info->nb_cpus == 0)
        info->nb_cpus = 1;
    env->boot_info = info;

#ifdef TARGET_WORDS_BIGENDIAN
    big_endian = 1;
#else
    big_endian = 0;
#endif

    
    kernel_size = load_elf(info->kernel_filename, 0, &elf_entry, NULL, NULL);
    entry = elf_entry;
    if (kernel_size < 0) {
        kernel_size = load_uimage(info->kernel_filename, &entry, NULL,
                                  &is_linux);
    }
    if (kernel_size < 0) {
        entry = info->loader_start + KERNEL_LOAD_ADDR;
        kernel_size = load_image_targphys(info->kernel_filename, entry,
                                          ram_size - KERNEL_LOAD_ADDR);
        is_linux = 1;
    }
    if (kernel_size < 0) {
        fprintf(stderr, "qemu: could not load kernel '%s'\n",
                info->kernel_filename);
        exit(1);
    }
    if (!is_linux) {
        
        env->regs[15] = entry & 0xfffffffe;
        env->thumb = entry & 1;
    } else {
        if (info->initrd_filename) {
            initrd_size = load_image_targphys(info->initrd_filename,
                                              info->loader_start
                                              + INITRD_LOAD_ADDR,
                                              ram_size - INITRD_LOAD_ADDR);
            if (initrd_size < 0) {
                fprintf(stderr, "qemu: could not load initrd '%s'\n",
                        info->initrd_filename);
                exit(1);
            }
        } else {
            initrd_size = 0;
        }
        bootloader[1] |= info->board_id & 0xff;
        bootloader[2] |= (info->board_id >> 8) & 0xff;
        bootloader[5] = info->loader_start + KERNEL_ARGS_ADDR;
        bootloader[6] = entry;
        for (n = 0; n < sizeof(bootloader) / 4; n++) {
            stl_phys_notdirty(info->loader_start + (n * 4), bootloader[n]);
        }
        if (info->nb_cpus > 1) {
            for (n = 0; n < sizeof(smpboot) / 4; n++) {
                stl_phys_notdirty(info->smp_loader_start + (n * 4), smpboot[n]);
            }
        }
        if (old_param)
            set_kernel_args_old(info, initrd_size, info->loader_start);
        else
            set_kernel_args(info, initrd_size, info->loader_start);
    }
    info->is_linux = is_linux;
    qemu_register_reset(main_cpu_reset, 0, env);
}
