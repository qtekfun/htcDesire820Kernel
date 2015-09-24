/* Copyright (C) 2006-2007 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

#ifndef TRACE_COMMON_H
#define TRACE_COMMON_H

#include <inttypes.h>

#define kMaxInsnPerBB 512

#define kMaxNumBasicBlocks 1024

#define kMaxNumAddrs 1024

#define kInsnBufferSize 1024

#define kCompressedSize 8192

#define kMethodEnter		0
#define kMethodExit		1
#define kMethodException	2
#define kNativeEnter		4
#define kNativeExit		5
#define kNativeException	6

#define TRACE_IDENT "qemu_trace_file"
#define TRACE_VERSION 2

typedef struct TraceHeader {
    char	ident[16];
    int		version;
    uint32_t	start_sec;
    uint32_t	start_usec;
    uint32_t	pdate;
    uint32_t	ptime;
    uint32_t	num_used_pids;		
    int		first_unused_pid;	
    uint8_t	padding[4];		
    uint64_t	num_static_bb;
    uint64_t	num_static_insn;
    uint64_t	num_dynamic_bb;
    uint64_t	num_dynamic_insn;
    uint64_t	elapsed_usecs;
} TraceHeader;

typedef struct BBRec {
    uint64_t	start_time;	
    uint64_t	bb_num;		
    uint32_t	repeat;		
    uint64_t	time_diff;	
} BBRec;

typedef struct AddrRec {
    uint64_t	time;
    uint32_t	addr;
} AddrRec;

typedef struct InsnRec {
    uint64_t	time_diff;	
    uint32_t	repeat;		
} InsnRec;

#define kPidEndOfFile		0
#define kPidFork		1
#define kPidClone		2
#define kPidSwitch		3
#define kPidExec		4
#define kPidMmap		5
#define kPidExit		6
#define kPidKthreadName		7
#define kPidSymbolAdd		8
#define kPidSymbolRemove	9
#define kPidMunmap		10
#define kPidNoAction		11
#define kPidName		12

#define bswap16(x) ((((x) & 0xff) << 8) | (((x) >> 8) & 0xff))

#define bswap32(x) ((((x) & 0xff) << 24) | (((x) & 0xff00) << 8) \
        | (((x) >> 8) & 0xff00) | (((x) >> 24) & 0xff))

#define bswap64(x) (((x) << 56) | (((x) & 0xff00) << 40) \
        | (((x) & 0xff0000) << 24) | (((x) & 0xff000000ull) << 8) \
        | (((x) >> 8) & 0xff000000ull) | (((x) >> 24) & 0xff0000) \
        | (((x) >> 40) & 0xff00) | ((x) >> 56))

#if BYTE_ORDER == LITTLE_ENDIAN
#define hostToLE16(x)	(x)
#define hostToLE32(x)	(x)
#define hostToLE64(x)	(x)
#define LE16ToHost(x)	(x)
#define LE32ToHost(x)	(x)
#define LE64ToHost(x)	(x)
#define convert16(x)
#define convert32(x)
#define convert64(x)
#else
#define hostToLE16(x)	bswap16(x)
#define hostToLE32(x)	bswap32(x)
#define hostToLE64(x)	bswap64(x)
#define LE16ToHost(x)	bswap16(x)
#define LE32ToHost(x)	bswap32(x)
#define LE64ToHost(x)	bswap64(x)
#define convert16(x) (x = bswap16(x))
#define convert32(x) (x = bswap32(x))
#define convert64(x) (x = bswap64(x))
#endif

static __inline__ int   insn_is_thumb(uint32_t   insn)
{
    return ((insn & 0xfff000f0) == 0xf7f000f0);
}

static __inline__ uint32_t  insn_wrap_thumb(uint32_t  insn)
{
    return 0xf7f000f0 | ((insn & 0xfff0) << 4) | (insn & 0x000f);
}

static __inline__ uint32_t  insn_unwrap_thumb(uint32_t  insn)
{
    return ((insn >> 4) & 0xfff0) | (insn & 0x000f);
}

#endif 
