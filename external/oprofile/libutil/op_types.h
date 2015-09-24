/**
 * @file op_types.h
 * General-utility types
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_TYPES_H
#define OP_TYPES_H

#ifndef __KERNEL__

#include <sys/types.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef int fd_t;

typedef unsigned long long vma_t;

typedef u64 count_type;

#else
#include <linux/types.h>
#endif

#endif 
