/**
 * @file op_arch.h
 * defines registers for x86
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Will Cohen
 */

#ifndef OP_ARCH_H
#define OP_ARCH_H

#include <asm/ptrace.h>

#ifndef instruction_pointer
#define instruction_pointer(regs) ((regs)->eip)
#endif

#define IRQ_ENABLED(regs)	((regs)->eflags & IF_MASK)

#endif 
