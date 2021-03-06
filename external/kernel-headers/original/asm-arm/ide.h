/*
 *  linux/include/asm-arm/ide.h
 *
 *  Copyright (C) 1994-1996  Linus Torvalds & authors
 */


#ifndef __ASMARM_IDE_H
#define __ASMARM_IDE_H

#ifdef __KERNEL__

#ifndef MAX_HWIFS
#define MAX_HWIFS	4
#endif

#if !defined(CONFIG_ARCH_L7200)
# define IDE_ARCH_OBSOLETE_INIT
# ifdef CONFIG_ARCH_CLPS7500
#  define ide_default_io_ctl(base)	((base) + 0x206) 
# else
#  define ide_default_io_ctl(base)	(0)
# endif
#endif 

#define __ide_mm_insw(port,addr,len)	readsw(port,addr,len)
#define __ide_mm_insl(port,addr,len)	readsl(port,addr,len)
#define __ide_mm_outsw(port,addr,len)	writesw(port,addr,len)
#define __ide_mm_outsl(port,addr,len)	writesl(port,addr,len)

#endif 

#endif 
