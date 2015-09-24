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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include "cpu.h"
#include "exec-all.h"
#include "android-trace.h"
#include "varint.h"
#include "android/utils/path.h"

typedef struct TraceBB {
    char        *filename;
    FILE        *fstream;
    BBRec       buffer[kMaxNumBasicBlocks];
    BBRec       *next;          
    uint64_t    flush_time;     
    char        compressed[kCompressedSize];
    char        *compressed_ptr;
    char        *high_water_ptr;
    int64_t     prev_bb_num;
    uint64_t    prev_bb_time;
    uint64_t    current_bb_num;
    uint64_t    current_bb_start_time;
    uint64_t    recnum;         
    uint32_t    current_bb_addr;
    int         num_insns;
} TraceBB;

typedef struct TraceInsn {
    char        *filename;
    FILE        *fstream;
    InsnRec     dummy;          
    InsnRec     buffer[kInsnBufferSize];
    InsnRec     *current;
    uint64_t    prev_time;      
    char        compressed[kCompressedSize];
    char        *compressed_ptr;
    char        *high_water_ptr;
} TraceInsn;

typedef struct TraceStatic {
    char        *filename;
    FILE        *fstream;
    uint32_t    insns[kMaxInsnPerBB];
    int         next_insn;
    uint64_t    bb_num;
    uint32_t    bb_addr;
    int         is_thumb;
} TraceStatic;

typedef struct TraceAddr {
    char        *filename;
    FILE        *fstream;
    AddrRec     buffer[kMaxNumAddrs];
    AddrRec     *next;
    char        compressed[kCompressedSize];
    char        *compressed_ptr;
    char        *high_water_ptr;
    uint32_t    prev_addr;
    uint64_t    prev_time;
} TraceAddr;

typedef struct TraceExc {
    char        *filename;
    FILE        *fstream;
    char        compressed[kCompressedSize];
    char        *compressed_ptr;
    char        *high_water_ptr;
    uint64_t    prev_time;
    uint64_t    prev_bb_recnum;
} TraceExc;

typedef struct TracePid {
    char        *filename;
    FILE        *fstream;
    char        compressed[kCompressedSize];
    char        *compressed_ptr;
    uint64_t    prev_time;
} TracePid;

typedef struct TraceMethod {
    char        *filename;
    FILE        *fstream;
    char        compressed[kCompressedSize];
    char        *compressed_ptr;
    uint64_t    prev_time;
    uint32_t    prev_addr;
    int32_t     prev_pid;
} TraceMethod;

extern TraceBB trace_bb;
extern TraceInsn trace_insn;
extern TraceStatic trace_static;
extern TraceAddr trace_load;
extern TraceAddr trace_store;
extern TraceExc trace_exc;
extern TracePid trace_pid;
extern TraceMethod trace_method;

TraceBB trace_bb;
TraceInsn trace_insn;
TraceStatic trace_static;
TraceAddr trace_load;
TraceAddr trace_store;
TraceExc trace_exc;
TracePid trace_pid;
TraceMethod trace_method;
static TraceHeader header;

const char *trace_filename;
int tracing;
int trace_cache_miss;
int trace_all_addr;

uint64_t sim_time = 1;

int current_pid;

uint64_t start_time, end_time;
uint64_t elapsed_usecs;

FILE *ftrace_debug;

#define kMaxInsnCompressed 14

#define kMaxBBCompressed 32

#define kMaxAddrCompressed 14

#define kMaxMethodCompressed 18

#define kMaxExcCompressed 38

#define kMaxPidCompressed 15

#define kMaxPid2Compressed 20

#define kMaxExecArgsCompressed 15

#define kMaxNameCompressed 20

#define kMaxMmapCompressed 33

#define kMaxMunmapCompressed 28

#define kMaxSymbolCompressed 24

#define kMaxKthreadNameCompressed 25

void trace_cleanup();

uint64 Now() {
    struct timeval        tv;

    gettimeofday(&tv, NULL);
    uint64 val = tv.tv_sec;
    val = val * 1000000ull + tv.tv_usec;
    return val;
}

static void create_trace_dir(const char *dirname)
{
    int err;

    err = path_mkdir(dirname, 0755);
    if (err != 0 && errno != EEXIST) {
        printf("err: %d\n", err);
        perror(dirname);
        exit(1);
    }
}

static char *create_trace_path(const char *filename, const char *ext)
{
    char *fname;
    const char *base_start, *base_end;
    int ii, len, base_len, dir_len, path_len, qtrace_len;

    
    if (filename == NULL || *filename == 0 || strcmp(filename, "/") == 0)
        return NULL;

    
    len = strlen(filename);
    if (filename[len - 1] == '/')
        len -= 1;

    
    
    
    base_start = base_end = &filename[len];
    for (ii = 0; ii < len; ++ii) {
        base_start -= 1;
        if (*base_start == '/') {
            base_start += 1;
            break;
        }
    }
    base_len = base_end - base_start;
    dir_len = len - base_len;
    qtrace_len = strlen("/qtrace");

    
    
    
    path_len = dir_len + base_len + qtrace_len + strlen(ext) + 1;
    fname = malloc(path_len);
    if (dir_len > 0)
        strncpy(fname, filename, dir_len);
    fname[dir_len] = 0;
    strncat(fname, base_start, base_len);
    strcat(fname, "/qtrace");
    strcat(fname, ext);
    return fname;
}

void convert_secs_to_date_time(time_t secs, uint32_t *pdate, uint32_t *ptime)
{
    struct tm *tm = localtime(&secs);
    uint32_t year = tm->tm_year + 1900;
    uint32_t thousands = year / 1000;
    year -= thousands * 1000;
    uint32_t hundreds = year / 100;
    year -= hundreds * 100;
    uint32_t tens = year / 10;
    year -= tens * 10;
    uint32_t ones = year;
    year = (thousands << 12) | (hundreds << 8) | (tens << 4) | ones;

    uint32_t mon = tm->tm_mon + 1;
    tens = mon / 10;
    ones = (mon - tens * 10);
    mon = (tens << 4) | ones;

    uint32_t day = tm->tm_mday;
    tens = day / 10;
    ones = (day - tens * 10);
    day = (tens << 4) | ones;

    *pdate = (year << 16) | (mon << 8) | day;

    uint32_t hour = tm->tm_hour;
    tens = hour / 10;
    ones = (hour - tens * 10);
    hour = (tens << 4) | ones;

    uint32_t min = tm->tm_min;
    tens = min / 10;
    ones = (min - tens * 10);
    min = (tens << 4) | ones;

    uint32_t sec = tm->tm_sec;
    tens = sec / 10;
    ones = (sec - tens * 10);
    sec = (tens << 4) | ones;

    *ptime = (hour << 16) | (min << 8) | sec;
}

void write_trace_header(TraceHeader *header)
{
    TraceHeader swappedHeader;

    memcpy(&swappedHeader, header, sizeof(TraceHeader));

    convert32(swappedHeader.version);
    convert32(swappedHeader.start_sec);
    convert32(swappedHeader.start_usec);
    convert32(swappedHeader.pdate);
    convert32(swappedHeader.ptime);
    convert32(swappedHeader.num_used_pids);
    convert32(swappedHeader.first_unused_pid);
    convert64(swappedHeader.num_static_bb);
    convert64(swappedHeader.num_static_insn);
    convert64(swappedHeader.num_dynamic_bb);
    convert64(swappedHeader.num_dynamic_insn);
    convert64(swappedHeader.elapsed_usecs);

    fwrite(&swappedHeader, sizeof(TraceHeader), 1, trace_static.fstream);
}

void create_trace_bb(const char *filename)
{
    char *fname = create_trace_path(filename, ".bb");
    trace_bb.filename = fname;

    FILE *fstream = fopen(fname, "wb");
    if (fstream == NULL) {
        perror(fname);
        exit(1);
    }
    trace_bb.fstream = fstream;
    trace_bb.next = &trace_bb.buffer[0];
    trace_bb.flush_time = 0;
    trace_bb.compressed_ptr = trace_bb.compressed;
    trace_bb.high_water_ptr = &trace_bb.compressed[kCompressedSize] - kMaxBBCompressed;
    trace_bb.prev_bb_num = 0;
    trace_bb.prev_bb_time = 0;
    trace_bb.num_insns = 0;
    trace_bb.recnum = 0;
}

void create_trace_insn(const char *filename)
{
    
    char *fname = create_trace_path(filename, ".insn");
    trace_insn.filename = fname;

    FILE *fstream = fopen(fname, "wb");
    if (fstream == NULL) {
        perror(fname);
        exit(1);
    }
    trace_insn.fstream = fstream;
    trace_insn.current = &trace_insn.dummy;
    trace_insn.dummy.time_diff = 0;
    trace_insn.dummy.repeat = 0;
    trace_insn.prev_time = 0;
    trace_insn.compressed_ptr = trace_insn.compressed;
    trace_insn.high_water_ptr = &trace_insn.compressed[kCompressedSize] - kMaxInsnCompressed;
}

void create_trace_static(const char *filename)
{
    
    char *fname = create_trace_path(filename, ".static");
    trace_static.filename = fname;

    FILE *fstream = fopen(fname, "wb");
    if (fstream == NULL) {
        perror(fname);
        exit(1);
    }
    trace_static.fstream = fstream;
    trace_static.next_insn = 0;
    trace_static.bb_num = 1;
    trace_static.bb_addr = 0;

    
    
    
    memset(&header, 0, sizeof(TraceHeader));

    
    
    header.version = TRACE_VERSION;

    
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    header.start_sec = tv.tv_sec;
    header.start_usec = tv.tv_usec;
    convert_secs_to_date_time(header.start_sec, &header.pdate, &header.ptime);
    write_trace_header(&header);

    
    uint64_t zero = 0;
    fwrite(&zero, sizeof(uint64_t), 1, trace_static.fstream);	
    fwrite(&zero, sizeof(uint32_t), 1, trace_static.fstream);	
    fwrite(&zero, sizeof(uint32_t), 1, trace_static.fstream);	
}

void create_trace_addr(const char *filename)
{
    
    trace_load.fstream = NULL;
    trace_store.fstream = NULL;
    if (trace_all_addr || trace_cache_miss) {
        
        char *fname = create_trace_path(filename, ".load");
        trace_load.filename = fname;

        FILE *fstream = fopen(fname, "wb");
        if (fstream == NULL) {
            perror(fname);
            exit(1);
        }
        trace_load.fstream = fstream;
        trace_load.next = &trace_load.buffer[0];
        trace_load.compressed_ptr = trace_load.compressed;
        trace_load.high_water_ptr = &trace_load.compressed[kCompressedSize] - kMaxAddrCompressed;
        trace_load.prev_addr = 0;
        trace_load.prev_time = 0;

        
        fname = create_trace_path(filename, ".store");
        trace_store.filename = fname;

        fstream = fopen(fname, "wb");
        if (fstream == NULL) {
            perror(fname);
            exit(1);
        }
        trace_store.fstream = fstream;
        trace_store.next = &trace_store.buffer[0];
        trace_store.compressed_ptr = trace_store.compressed;
        trace_store.high_water_ptr = &trace_store.compressed[kCompressedSize] - kMaxAddrCompressed;
        trace_store.prev_addr = 0;
        trace_store.prev_time = 0;
    }
}

void create_trace_exc(const char *filename)
{
    
    char *fname = create_trace_path(filename, ".exc");
    trace_exc.filename = fname;

    FILE *fstream = fopen(fname, "wb");
    if (fstream == NULL) {
        perror(fname);
        exit(1);
    }
    trace_exc.fstream = fstream;
    trace_exc.compressed_ptr = trace_exc.compressed;
    trace_exc.high_water_ptr = &trace_exc.compressed[kCompressedSize] - kMaxExcCompressed;
    trace_exc.prev_time = 0;
    trace_exc.prev_bb_recnum = 0;
}

void create_trace_pid(const char *filename)
{
    
    char *fname = create_trace_path(filename, ".pid");
    trace_pid.filename = fname;

    FILE *fstream = fopen(fname, "wb");
    if (fstream == NULL) {
        perror(fname);
        exit(1);
    }
    trace_pid.fstream = fstream;
    trace_pid.compressed_ptr = trace_pid.compressed;
    trace_pid.prev_time = 0;
}

void create_trace_method(const char *filename)
{
    
    char *fname = create_trace_path(filename, ".method");
    trace_method.filename = fname;

    FILE *fstream = fopen(fname, "wb");
    if (fstream == NULL) {
        perror(fname);
        exit(1);
    }
    trace_method.fstream = fstream;
    trace_method.compressed_ptr = trace_method.compressed;
    trace_method.prev_time = 0;
    trace_method.prev_addr = 0;
    trace_method.prev_pid = 0;
}

void trace_init(const char *filename)
{
    
    create_trace_dir(filename);
    create_trace_bb(filename);
    create_trace_insn(filename);
    create_trace_static(filename);
    create_trace_addr(filename);
    create_trace_exc(filename);
    create_trace_pid(filename);
    create_trace_method(filename);

#if 0
    char *fname = create_trace_path(filename, ".debug");
    ftrace_debug = fopen(fname, "wb");
    if (ftrace_debug == NULL) {
        perror(fname);
        exit(1);
    }
#else
    ftrace_debug = NULL;
#endif
    atexit(trace_cleanup);

    
    if (tracing)
        start_time = Now();
}


static int   interlocks[16];
static int   interlock_base;

static void
_interlock_def(int  reg, int  delay)
{
    if (reg >= 0)
        interlocks[reg] = interlock_base + delay;
}

static int
_interlock_use(int  reg)
{
    int  delay = 0;

    if (reg >= 0)
    {
        delay = interlocks[reg] - interlock_base;
        if (delay < 0)
            delay = 0;
    }
    return delay;
}

void trace_bb_start(uint32_t bb_addr)
{
    int  nn;

    trace_static.bb_addr = bb_addr;
    trace_static.is_thumb = 0;

    interlock_base = 0;
    for (nn = 0; nn < 16; nn++)
        interlocks[nn] = 0;
}

void trace_add_insn(uint32_t insn, int is_thumb)
{
    trace_static.insns[trace_static.next_insn++] = insn;
    
    
    
    trace_static.is_thumb = is_thumb;
}

void trace_bb_end()
{
    int		ii, num_insns;
    uint32_t	insn;

    uint64_t bb_num = hostToLE64(trace_static.bb_num);
    
    
    uint32_t bb_addr = trace_static.bb_addr | trace_static.is_thumb;
    bb_addr = hostToLE32(bb_addr);
    num_insns = hostToLE32(trace_static.next_insn);
    fwrite(&bb_num, sizeof(bb_num), 1, trace_static.fstream);
    fwrite(&bb_addr, sizeof(bb_addr), 1, trace_static.fstream);
    fwrite(&num_insns, sizeof(num_insns), 1, trace_static.fstream);
    for (ii = 0; ii < trace_static.next_insn; ++ii) {
        insn = hostToLE32(trace_static.insns[ii]);
        fwrite(&insn, sizeof(insn), 1, trace_static.fstream);
    }

    trace_static.bb_num += 1;
    trace_static.next_insn = 0;
}

void trace_cleanup()
{
    if (tracing) {
        end_time = Now();
        elapsed_usecs += end_time - start_time;
    }
    header.elapsed_usecs = elapsed_usecs;
    double elapsed_secs = elapsed_usecs / 1000000.0;
    double cycles_per_sec = 0;
    if (elapsed_secs != 0)
        cycles_per_sec = sim_time / elapsed_secs;
    char *suffix = "";
    if (cycles_per_sec >= 1000000) {
        cycles_per_sec /= 1000000.0;
        suffix = "M";
    } else if (cycles_per_sec > 1000) {
        cycles_per_sec /= 1000.0;
        suffix = "K";
    }
    printf("Elapsed seconds: %.2f, simulated cycles/sec: %.1f%s\n",
           elapsed_secs, cycles_per_sec, suffix);
    if (trace_bb.fstream) {
        BBRec *ptr;
        BBRec *next = trace_bb.next;
        char *comp_ptr = trace_bb.compressed_ptr;
        int64_t prev_bb_num = trace_bb.prev_bb_num;
        uint64_t prev_bb_time = trace_bb.prev_bb_time;
        for (ptr = trace_bb.buffer; ptr != next; ++ptr) {
            if (comp_ptr >= trace_bb.high_water_ptr) {
                uint32_t size = comp_ptr - trace_bb.compressed;
                fwrite(trace_bb.compressed, sizeof(char), size,
                       trace_bb.fstream);
                comp_ptr = trace_bb.compressed;
            }
            int64_t bb_diff = ptr->bb_num - prev_bb_num;
            prev_bb_num = ptr->bb_num;
            uint64_t time_diff = ptr->start_time - prev_bb_time;
            prev_bb_time = ptr->start_time;
            comp_ptr = varint_encode_signed(bb_diff, comp_ptr);
            comp_ptr = varint_encode(time_diff, comp_ptr);
            comp_ptr = varint_encode(ptr->repeat, comp_ptr);
            if (ptr->repeat)
                comp_ptr = varint_encode(ptr->time_diff, comp_ptr);
        }

        
        
        uint64_t time_diff = sim_time - prev_bb_time;
        if (time_diff > 0) {
            int64_t bb_diff = -prev_bb_num;
            comp_ptr = varint_encode_signed(bb_diff, comp_ptr);
            comp_ptr = varint_encode(time_diff, comp_ptr);
            comp_ptr = varint_encode(0, comp_ptr);
        }

        uint32_t size = comp_ptr - trace_bb.compressed;
        if (size)
            fwrite(trace_bb.compressed, sizeof(char), size, trace_bb.fstream);

        
        
        uint32_t zeros = 0;
        fwrite(&zeros, 3, 1, trace_bb.fstream);
        fclose(trace_bb.fstream);
    }

    if (trace_insn.fstream) {
        InsnRec *ptr;
        InsnRec *current = trace_insn.current + 1;
        char *comp_ptr = trace_insn.compressed_ptr;
        for (ptr = trace_insn.buffer; ptr != current; ++ptr) {
            if (comp_ptr >= trace_insn.high_water_ptr) {
                uint32_t size = comp_ptr - trace_insn.compressed;
                uint32_t rval = fwrite(trace_insn.compressed, sizeof(char),
                                       size, trace_insn.fstream);
                if (rval != size) {
                    fprintf(stderr, "fwrite() failed\n");
                    perror(trace_insn.filename);
                    exit(1);
                }
                comp_ptr = trace_insn.compressed;
            }
            comp_ptr = varint_encode(ptr->time_diff, comp_ptr);
            comp_ptr = varint_encode(ptr->repeat, comp_ptr);
        }

        uint32_t size = comp_ptr - trace_insn.compressed;
        if (size) {
            uint32_t rval = fwrite(trace_insn.compressed, sizeof(char), size,
                                   trace_insn.fstream);
            if (rval != size) {
                fprintf(stderr, "fwrite() failed\n");
                perror(trace_insn.filename);
                exit(1);
            }
        }
        fclose(trace_insn.fstream);
    }

    if (trace_static.fstream) {
        fseek(trace_static.fstream, 0, SEEK_SET);
        write_trace_header(&header);
        fclose(trace_static.fstream);
    }

    if (trace_load.fstream) {
        AddrRec *ptr;
        char *comp_ptr = trace_load.compressed_ptr;
        AddrRec *next = trace_load.next;
        uint32_t prev_addr = trace_load.prev_addr;
        uint64_t prev_time = trace_load.prev_time;
        for (ptr = trace_load.buffer; ptr != next; ++ptr) {
            if (comp_ptr >= trace_load.high_water_ptr) {
                uint32_t size = comp_ptr - trace_load.compressed;
                fwrite(trace_load.compressed, sizeof(char), size,
                       trace_load.fstream);
                comp_ptr = trace_load.compressed;
            }

            int addr_diff = ptr->addr - prev_addr;
            uint64_t time_diff = ptr->time - prev_time;
            prev_addr = ptr->addr;
            prev_time = ptr->time;

            comp_ptr = varint_encode_signed(addr_diff, comp_ptr);
            comp_ptr = varint_encode(time_diff, comp_ptr);
        }

        uint32_t size = comp_ptr - trace_load.compressed;
        if (size) {
            fwrite(trace_load.compressed, sizeof(char), size,
                   trace_load.fstream);
        }

        
        
        uint32_t zeros = 0;
        fwrite(&zeros, 2, 1, trace_load.fstream);
        fclose(trace_load.fstream);
    }

    if (trace_store.fstream) {
        AddrRec *ptr;
        char *comp_ptr = trace_store.compressed_ptr;
        AddrRec *next = trace_store.next;
        uint32_t prev_addr = trace_store.prev_addr;
        uint64_t prev_time = trace_store.prev_time;
        for (ptr = trace_store.buffer; ptr != next; ++ptr) {
            if (comp_ptr >= trace_store.high_water_ptr) {
                uint32_t size = comp_ptr - trace_store.compressed;
                fwrite(trace_store.compressed, sizeof(char), size,
                       trace_store.fstream);
                comp_ptr = trace_store.compressed;
            }

            int addr_diff = ptr->addr - prev_addr;
            uint64_t time_diff = ptr->time - prev_time;
            prev_addr = ptr->addr;
            prev_time = ptr->time;

            comp_ptr = varint_encode_signed(addr_diff, comp_ptr);
            comp_ptr = varint_encode(time_diff, comp_ptr);
        }

        uint32_t size = comp_ptr - trace_store.compressed;
        if (size) {
            fwrite(trace_store.compressed, sizeof(char), size,
                   trace_store.fstream);
        }

        
        
        uint32_t zeros = 0;
        fwrite(&zeros, 2, 1, trace_store.fstream);
        fclose(trace_store.fstream);
    }

    if (trace_exc.fstream) {
        uint32_t size = trace_exc.compressed_ptr - trace_exc.compressed;
        if (size) {
            fwrite(trace_exc.compressed, sizeof(char), size,
                   trace_exc.fstream);
        }

        
        
        uint64_t zeros = 0;
        fwrite(&zeros, 7, 1, trace_exc.fstream);
        fclose(trace_exc.fstream);
    }
    if (trace_pid.fstream) {
        uint32_t size = trace_pid.compressed_ptr - trace_pid.compressed;
        if (size) {
            fwrite(trace_pid.compressed, sizeof(char), size,
                   trace_pid.fstream);
        }

        
        
        uint64_t zeros = 0;
        fwrite(&zeros, 2, 1, trace_pid.fstream);
        fclose(trace_pid.fstream);
    }
    if (trace_method.fstream) {
        uint32_t size = trace_method.compressed_ptr - trace_method.compressed;
        if (size) {
            fwrite(trace_method.compressed, sizeof(char), size,
                   trace_method.fstream);
        }

        
        
        uint64_t zeros = 0;
        fwrite(&zeros, 2, 1, trace_method.fstream);
        fclose(trace_method.fstream);
    }
    if (ftrace_debug)
        fclose(ftrace_debug);
}

#define TICKS_OTHER	1
#define TICKS_SMULxy	1
#define TICKS_SMLAWy	1
#define TICKS_SMLALxy	2
#define TICKS_MUL	2
#define TICKS_MLA	2
#define TICKS_MULS	4	
#define TICKS_MLAS	4	
#define TICKS_UMULL	3
#define TICKS_UMLAL	3
#define TICKS_SMULL	3
#define TICKS_SMLAL	3
#define TICKS_UMULLS	5	
#define TICKS_UMLALS	5	
#define TICKS_SMULLS	5	
#define TICKS_SMLALS	5	

int get_insn_ticks_arm(uint32_t insn)
{
#if 1
    int   result   =  1;   

    

    
    if ((insn >> 28) == 0xf)
    {
        if ((insn >> 25) == 0x7d) {  
            result = 3;
            goto Exit;
        }
        
        
        goto Exit;
    }

    
    switch ((insn >> 25) & 7)
    {
        case 0:
            if ((insn & 0x00000090) == 0x00000090)  
            {
                

                if ((insn & 0x0fc000f0) == 0x00000090)   
                {
                    int  Rm = (insn & 15);
                    int  Rs = (insn >> 8) & 15;
                    int  Rn = (insn >> 12) & 15;

                    if ((insn & 0x00200000) != 0) {  
                        result += _interlock_use(Rn);
                    } else {   
                        if (Rn != 0)      
                            goto Exit;
                    }
                    
                    result += 2 + _interlock_use(Rm) + _interlock_use(Rs);
                }
                else if ((insn & 0x0f8000f0) == 0x00800090)  
                {
                    int  Rm   = (insn & 15);
                    int  Rs   = (insn >> 8) & 15;
                    int  RdLo = (insn >> 12) & 15;
                    int  RdHi = (insn >> 16) & 15;

                    if ((insn & 0x00200000) != 0) { 
                        result += _interlock_use(RdLo) + _interlock_use(RdHi);
                    }
                    

                    
                    result += 3 + _interlock_use(Rm) + _interlock_use(Rs);
                }
                else if ((insn & 0x0fd00ff0) == 0x01000090)  
                {
                    int  Rm = (insn & 15);
                    int  Rd = (insn >> 8) & 15;

                    result = 2 + _interlock_use(Rm);
                    _interlock_def(Rd, result+1);
                }
                else if ((insn & 0x0e400ff0) == 0x00000090)  
                {
                    int  Rm = (insn & 15);
                    int  Rd = (insn >> 12) & 15;
                    int  Rn = (insn >> 16) & 15;

                    result += _interlock_use(Rn) + _interlock_use(Rm);
                    if ((insn & 0x00100000) != 0)  
                        _interlock_def(Rd, result+2);
                }
                else if ((insn & 0x0e400ff0) == 0x00400090)  
                {
                    int  Rd = (insn >> 12) & 15;
                    int  Rn = (insn >> 16) & 15;

                    result += _interlock_use(Rn);
                    if ((insn & 0x00100000) != 0)  
                        _interlock_def(Rd, result+2);
                }
                else if ((insn & 0x0e500fd0) == 0x000000d0) 
                {
                    
                }
                else if ((insn & 0x0e500fd0) == 0x001000d0) 
                {
                    int  Rm = (insn & 15);
                    int  Rd = (insn >> 12) & 15;
                    int  Rn = (insn >> 16) & 15;

                    result += _interlock_use(Rn) + _interlock_use(Rm);
                    if ((insn & 0x00100000) != 0)  
                        _interlock_def(Rd, result+2);
                }
                else if ((insn & 0x0e5000d0) == 0x004000d0) 
                {
                    
                }
                else if ((insn & 0x0e5000d0) == 0x005000d0) 
                {
                    int  Rd = (insn >> 12) & 15;
                    int  Rn = (insn >> 16) & 15;

                    result += _interlock_use(Rn);
                    if ((insn & 0x00100000) != 0)  
                        _interlock_def(Rd, result+2);
                }
                else
                {
                    
                }
            }
            else if ((insn & 0x0f900000) == 0x01000000)  
            {
                switch ((insn >> 4) & 15)
                {
                    case 0:
                        if ((insn & 0x0fb0fff0) == 0x0120f000) 
                        {
                            int  Rm = (insn & 15);
                            result += _interlock_use(Rm);
                        }
                        break;

                    case 1:
                        if ( ((insn & 0x0ffffff0) == 0x01200010) ||  
                             ((insn & 0x0fff0ff0) == 0x01600010) )   
                        {
                            int  Rm = (insn & 15);
                            result += _interlock_use(Rm);
                        }
                        break;

                    case 3:
                        if ((insn & 0x0ffffff0) == 0x01200030)   
                        {
                            int  Rm = (insn & 15);
                            result += _interlock_use(Rm);
                        }
                        break;

                    default:
                        
                        ;
                }
            }
            else  
            {
                int  Rm = (insn & 15);
                int  Rn = (insn >> 16) & 15;

                result += _interlock_use(Rn) + _interlock_use(Rm);
                if ((insn & 0x10)) {   
                    int  Rs = (insn >> 8) & 15;
                    result += 1 + _interlock_use(Rs);
                }
            }
            break;

        case 1:
            if ((insn & 0x01900000) == 0x01900000)
            {
                
            }
            else  
            {
                int  Rn = (insn >> 12) & 15;
                result += _interlock_use(Rn);
            }
            break;

        case 2:  
            {
                int  Rn = (insn >> 16) & 15;

                result += _interlock_use(Rn);
                if (insn & 0x00100000) {  
                    int  Rd = (insn >> 12) & 15;

                    if (Rd == 15)  
                        result = 5;
                    else
                        _interlock_def(Rd,result+1);
                }
            }
            break;

        case 3:
            if ((insn & 0x10) == 0)  
            {
                int  Rm = (insn & 15);
                int  Rn = (insn >> 16) & 15;

                result += _interlock_use(Rm) + _interlock_use(Rn);

                if (insn & 0x00100000) {  
                    int  Rd = (insn >> 12) & 15;
                    if (Rd == 15)
                        result = 5;
                    else
                        _interlock_def(Rd,result+1);
                }
            }
            
            break;

        case 4:  
            {
                int       Rn   = (insn >> 16) & 15;
                uint32_t  mask = (insn & 0xffff);
                int       count;

                for (count = 0; mask; count++)
                    mask &= (mask-1);

                result += _interlock_use(Rn);

                if (insn & 0x00100000)  
                {
                    int  nn;

                    if (insn & 0x8000) {  
                        result = count+4;
                    } else {  
                        result = (count < 2) ? 2 : count;
                    }
                    
                    for (nn = 0; nn < 15; nn++)
                        if ((insn & (1U << nn)) != 0)
                            _interlock_def(nn,result);
                }
                else  
                    result = (count < 2) ? 2 : count;
            }
            break;

        case 5:  
            break;

        case 6:  
            {
                int  Rn = (insn >> 16) & 15;

                if (insn & 0x00100000)
                    result += _interlock_use(Rn);

                
            }
            break;

        default: 
            
            ;
    }
Exit:
    interlock_base += result;
    return result;
#else 
    if ((insn & 0x0ff0f090) == 0x01600080) {
        return TICKS_SMULxy;
    } else if ((insn & 0x0ff00090) == 0x01200080) {
        return TICKS_SMLAWy;
    } else if ((insn & 0x0ff00090) == 0x01400080) {
        return TICKS_SMLALxy;
    } else if ((insn & 0x0f0000f0) == 0x00000090) {
        
        uint8_t bit23 = (insn >> 23) & 0x1;
        uint8_t bit22_U = (insn >> 22) & 0x1;
        uint8_t bit21_A = (insn >> 21) & 0x1;
        uint8_t bit20_S = (insn >> 20) & 0x1;

        if (bit23 == 0) {
            
            if (bit22_U != 0) {
                
                return TICKS_OTHER;
            }
            if (bit21_A == 0) {
                if (bit20_S)
                    return TICKS_MULS;
                return TICKS_MUL;
            }
            if (bit20_S)
                return TICKS_MLAS;
            return TICKS_MLA;
        }
        
        if (bit22_U == 0) {
            
            if (bit21_A == 0) {
                if (bit20_S)
                    return TICKS_UMULLS;
                return TICKS_UMULL;
            }
            if (bit20_S)
                return TICKS_UMLALS;
            return TICKS_UMLAL;
        }
        
        if (bit21_A == 0) {
            if (bit20_S)
                return TICKS_SMULLS;
            return TICKS_SMULL;
        }
        if (bit20_S)
            return TICKS_SMLALS;
        return TICKS_SMLAL;
    }
    return TICKS_OTHER;
#endif
}

int  get_insn_ticks_thumb(uint32_t  insn)
{
#if 1
    int  result = 1;

    switch ((insn >> 11) & 31)
    {
        case 0:
        case 1:
        case 2:   
            {
                int  Rm = (insn >> 3) & 7;
                result += _interlock_use(Rm);
            }
            break;

        case 3:  
            {
                int  Rn = (insn >> 3) & 7;
                result += _interlock_use(Rn);

                if ((insn & 0x0400) == 0) {  
                    int  Rm = (insn >> 6) & 7;
                    result += _interlock_use(Rm);
                }
            }
            break;

        case 4:  
            break;

        case 5:
        case 6:
        case 7:  
            {
                int  Rd = (insn >> 8) & 7;
                result += _interlock_use(Rd);
            }
            break;

        case 8:
            {
                if ((insn & 0x0400) == 0)  
                {
                    
                    
                    
                    int  Rm = (insn >> 3) & 7;
                    int  Rd = (insn >> 3) & 7;

                    result += _interlock_use(Rm) + _interlock_use(Rd);
                }
                else switch ((insn >> 8) & 3)
                {
                    case 0:
                    case 1:
                    case 2:  
                        {
                            int  Rn = (insn & 7) | ((insn >> 4) & 0x8);
                            int  Rm = ((insn >> 3) & 15);

                            result += _interlock_use(Rn) + _interlock_use(Rm);
                        }
                        break;

                    case 3:
                        if ((insn & 0xff07) == 0x4700)  
                        {
                            int  Rm = (insn >> 3) & 15;

                            result = 3 + _interlock_use(Rm);
                        }
                        
                        break;
                }
            }
            break;

        case 9:  
            {
                int  Rd = (insn >> 8) & 7;
                _interlock_def(Rd,result+1);
            }
            break;

        case 10:
        case 11:  
            {
                int  Rd = (insn & 7);
                int  Rn = (insn >> 3) & 7;
                int  Rm = (insn >> 6) & 7;

                result += _interlock_use(Rn) + _interlock_use(Rm);

                switch ((insn >> 9) & 7)
                {
                    case 0: 
                    case 1: 
                    case 2: 
                        result += _interlock_use(Rd);
                        break;

                    case 3: 
                    case 5: 
                    case 6: 
                    case 7: 
                        _interlock_def(Rd,result+2);
                        break;

                    case 4: 
                        _interlock_def(Rd,result+1);
                }
            }
            break;

        case 12:  
        case 14:  
            {
                int  Rd = (insn & 7);
                int  Rn = (insn >> 3) & 7;

                result += _interlock_use(Rd) + _interlock_use(Rn);
            }
            break;

        case 13:  
            {
                int  Rd = (insn & 7);
                int  Rn = (insn >> 3) & 7;

                result += _interlock_use(Rn);
                _interlock_def(Rd,result+1);
            }
            break;

        case 15:  
            {
                int  Rd = (insn & 7);
                int  Rn = (insn >> 3) & 7;

                result += _interlock_use(Rn);
                _interlock_def(Rd,result+2);
            }
            break;

        case 16:  
            {
                int  Rd = (insn & 7);
                int  Rn = (insn >> 3) & 7;

                result += _interlock_use(Rn) + _interlock_use(Rd);
            }
            break;

        case 17:  
            {
                int  Rd = (insn & 7);
                int  Rn = (insn >> 3) & 7;

                result += _interlock_use(Rn);
                _interlock_def(Rd,result+2);
            }
            break;

        case 18:  
            {
                int  Rd = (insn >> 8) & 3;
                result += _interlock_use(Rd);
            }
            break;

        case 19:  
            {
                int  Rd = (insn >> 8) & 3;
                _interlock_def(Rd,result+1);
            }
            break;

        case 20:  
        case 21:  
            {
                int  Rd = (insn >> 8) & 3;
                result += _interlock_use(Rd);
            }
            break;

        case 22:
        case 23:  
            {
                if ((insn & 0xff00) == 0xb000)  
                {
                    result += _interlock_use(14);
                }
                else if ((insn & 0x0600) == 0x0400)  
                {
                    uint32_t  mask = insn & 0x01ff;
                    int       count, nn;

                    for (count = 0; mask; count++)
                        mask &= (mask-1);

                    result = (count < 2) ? 2 : count;

                    if (insn & 0x0800)  
                    {
                        for (nn = 0; nn < 9; nn++)
                            if (insn & (1 << nn))
                                _interlock_def(nn, result);
                    }
                    else  
                    {
                        for (nn = 0; nn < 9; nn++)
                            if (insn & (1 << nn))
                                result += _interlock_use(nn);
                    }
                }
                
            }
            break;

        case 24:  
            {
                int  Rd = (insn >> 8) & 7;
                uint32_t  mask = insn & 255;
                int       count, nn;

                for (count = 0; mask; count++)
                    mask &= (mask-1);

                result = (count < 2) ? 2 : count;
                result += _interlock_use(Rd);

                for (nn = 0; nn < 8; nn++)
                    if (insn & (1 << nn))
                        result += _interlock_use(nn);
            }
            break;

        case 25:  
            {
                int  Rd = (insn >> 8) & 7;
                uint32_t  mask = insn & 255;
                int       count, nn;

                for (count = 0; mask; count++)
                    mask &= (mask-1);

                result  = (count < 2) ? 2 : count;
                result += _interlock_use(Rd);

                for (nn = 0; nn < 8; nn++)
                    if (insn & (1 << nn))
                        _interlock_def(nn, result);
            }
            break;

        case 26:
        case 27:  
            switch ((insn >> 8) & 15)
            {
                case 14: 
                case 15: 
                    break;

                default:  
                    result = 3;
            }
            break;

        case 28:  
            result = 3;
            break;

        case 29:  
            if ((insn & 1) == 0)
                result = 3;
            break;

        case 30:  
            break;

        case 31:  
            result = 3;
            break;
    }
    interlock_base += result;
    return result;
#else 
    if ((insn & 0xfc00) == 0x4340) 
        return TICKS_SMULxy;

    return TICKS_OTHER;
#endif
}

void trace_exception(uint32 target_pc)
{
    if (trace_exc.fstream == NULL)
        return;

    
    
    if (trace_bb.current_bb_num == 0)
        return;

    uint32_t current_pc = trace_bb.current_bb_addr + 4 * (trace_bb.num_insns - 1);
#if 0
    if (ftrace_debug) {
        fprintf(ftrace_debug, "t%llu exc pc: 0x%x bb_addr: 0x%x num_insns: %d current_pc: 0x%x bb_num %llu bb_start_time %llu\n",
                sim_time, target_pc, trace_bb.current_bb_addr,
                trace_bb.num_insns, current_pc, trace_bb.current_bb_num,
                trace_bb.current_bb_start_time);
    }
#endif
    char *comp_ptr = trace_exc.compressed_ptr;
    if (comp_ptr >= trace_exc.high_water_ptr) {
        uint32_t size = comp_ptr - trace_exc.compressed;
        fwrite(trace_exc.compressed, sizeof(char), size, trace_exc.fstream);
        comp_ptr = trace_exc.compressed;
    }
    uint64_t time_diff = sim_time - trace_exc.prev_time;
    trace_exc.prev_time = sim_time;
    uint64_t bb_recnum_diff = trace_bb.recnum - trace_exc.prev_bb_recnum;
    trace_exc.prev_bb_recnum = trace_bb.recnum;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    comp_ptr = varint_encode(current_pc, comp_ptr);
    comp_ptr = varint_encode(bb_recnum_diff, comp_ptr);
    comp_ptr = varint_encode(target_pc, comp_ptr);
    comp_ptr = varint_encode(trace_bb.current_bb_num, comp_ptr);
    comp_ptr = varint_encode(trace_bb.current_bb_start_time, comp_ptr);
    comp_ptr = varint_encode(trace_bb.num_insns, comp_ptr);
    trace_exc.compressed_ptr = comp_ptr;
}

void trace_pid_1arg(int pid, int rec_type)
{
    if (trace_pid.fstream == NULL)
        return;
    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + kMaxPidCompressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(pid, comp_ptr);
    trace_pid.compressed_ptr = comp_ptr;
}

void trace_pid_2arg(int tgid, int pid, int rec_type)
{
    if (trace_pid.fstream == NULL)
        return;
    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + kMaxPid2Compressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(tgid, comp_ptr);
    comp_ptr = varint_encode(pid, comp_ptr);
    trace_pid.compressed_ptr = comp_ptr;
}

void trace_switch(int pid)
{
#if 0
    if (ftrace_debug && trace_pid.fstream)
        fprintf(ftrace_debug, "t%lld switch %d\n", sim_time, pid);
#endif
    trace_pid_1arg(pid, kPidSwitch);
    current_pid = pid;
}

void trace_fork(int tgid, int pid)
{
#if 0
    if (ftrace_debug && trace_pid.fstream)
        fprintf(ftrace_debug, "t%lld fork %d\n", sim_time, pid);
#endif
    trace_pid_2arg(tgid, pid, kPidFork);
}

void trace_clone(int tgid, int pid)
{
#if 0
    if (ftrace_debug && trace_pid.fstream)
        fprintf(ftrace_debug, "t%lld clone %d\n", sim_time, pid);
#endif
    trace_pid_2arg(tgid, pid, kPidClone);
}

void trace_exit(int exitcode)
{
#if 0
    if (ftrace_debug && trace_pid.fstream)
        fprintf(ftrace_debug, "t%lld exit %d\n", sim_time, exitcode);
#endif
    trace_pid_1arg(exitcode, kPidExit);
}

void trace_name(char *name)
{
#if 0
    if (ftrace_debug && trace_pid.fstream) {
        fprintf(ftrace_debug, "t%lld pid %d name %s\n",
                sim_time, current_pid, name);
    }
#endif
    if (trace_pid.fstream == NULL)
        return;
    int len = strlen(name);
    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + len + kMaxNameCompressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    int rec_type = kPidName;
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(current_pid, comp_ptr);
    comp_ptr = varint_encode(len, comp_ptr);
    strncpy(comp_ptr, name, len);
    comp_ptr += len;
    trace_pid.compressed_ptr = comp_ptr;
}

void trace_execve(const char *argv, int len)
{
    int ii;

    if (trace_pid.fstream == NULL)
        return;
    
    int alen = 0;
    int sum_len = 0;
    int argc = 0;
    const char *ptr = argv;
    while (sum_len < len) {
        argc += 1;
        alen = strlen(ptr);
        ptr += alen + 1;
        sum_len += alen + 1;
    }

#if 0
    if (ftrace_debug) {
        fprintf(ftrace_debug, "t%lld argc: %d\n", sim_time, argc);
        alen = 0;
        ptr = argv;
        for (ii = 0; ii < argc; ++ii) {
            fprintf(ftrace_debug, "  argv[%d]: %s\n", ii, ptr);
            alen = strlen(ptr);
            ptr += alen + 1;
        }
    }
#endif

    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + len + 5 * argc + kMaxExecArgsCompressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    int rec_type = kPidExec;
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(argc, comp_ptr);

    ptr = argv;
    for (ii = 0; ii < argc; ++ii) {
        alen = strlen(ptr);
        comp_ptr = varint_encode(alen, comp_ptr);
        strncpy(comp_ptr, ptr, alen);
        comp_ptr += alen;
        ptr += alen + 1;
    }
    trace_pid.compressed_ptr = comp_ptr;
}

void trace_mmap(unsigned long vstart, unsigned long vend,
                unsigned long offset, const char *path)
{
    if (trace_pid.fstream == NULL)
        return;
#if 0
    if (ftrace_debug)
        fprintf(ftrace_debug, "t%lld mmap %08lx - %08lx, offset %08lx '%s'\n",
                sim_time, vstart, vend, offset, path);
#endif
    int len = strlen(path);
    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + len + kMaxMmapCompressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    int rec_type = kPidMmap;
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(vstart, comp_ptr);
    comp_ptr = varint_encode(vend, comp_ptr);
    comp_ptr = varint_encode(offset, comp_ptr);
    comp_ptr = varint_encode(len, comp_ptr);
    strncpy(comp_ptr, path, len);
    trace_pid.compressed_ptr = comp_ptr + len;
}

void trace_munmap(unsigned long vstart, unsigned long vend)
{
    if (trace_pid.fstream == NULL)
        return;
#if 0
    if (ftrace_debug)
        fprintf(ftrace_debug, "t%lld munmap %08lx - %08lx\n",
                sim_time, vstart, vend);
#endif
    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + kMaxMunmapCompressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    int rec_type = kPidMunmap;
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(vstart, comp_ptr);
    comp_ptr = varint_encode(vend, comp_ptr);
    trace_pid.compressed_ptr = comp_ptr;
}

void trace_dynamic_symbol_add(unsigned long vaddr, const char *name)
{
    if (trace_pid.fstream == NULL)
        return;
#if 0
    if (ftrace_debug)
        fprintf(ftrace_debug, "t%lld sym %08lx '%s'\n", sim_time, vaddr, name);
#endif
    int len = strlen(name);
    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + len + kMaxSymbolCompressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    int rec_type = kPidSymbolAdd;
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(vaddr, comp_ptr);
    comp_ptr = varint_encode(len, comp_ptr);
    strncpy(comp_ptr, name, len);
    trace_pid.compressed_ptr = comp_ptr + len;
}

void trace_dynamic_symbol_remove(unsigned long vaddr)
{
    if (trace_pid.fstream == NULL)
        return;
#if 0
    if (ftrace_debug)
        fprintf(ftrace_debug, "t%lld remove %08lx\n", sim_time, vaddr);
#endif
    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + kMaxSymbolCompressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    int rec_type = kPidSymbolRemove;
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(vaddr, comp_ptr);
    trace_pid.compressed_ptr = comp_ptr;
}

void trace_init_name(int tgid, int pid, const char *name)
{
    if (trace_pid.fstream == NULL)
        return;
#if 0
    if (ftrace_debug)
        fprintf(ftrace_debug, "t%lld kthread %d %s\n", sim_time, pid, name);
#endif
    int len = strlen(name);
    char *comp_ptr = trace_pid.compressed_ptr;
    char *max_end_ptr = comp_ptr + len + kMaxKthreadNameCompressed;
    if (max_end_ptr >= &trace_pid.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_pid.compressed;
        fwrite(trace_pid.compressed, sizeof(char), size, trace_pid.fstream);
        comp_ptr = trace_pid.compressed;
    }
    uint64_t time_diff = sim_time - trace_pid.prev_time;
    trace_pid.prev_time = sim_time;
    comp_ptr = varint_encode(time_diff, comp_ptr);
    int rec_type = kPidKthreadName;
    comp_ptr = varint_encode(rec_type, comp_ptr);
    comp_ptr = varint_encode(tgid, comp_ptr);
    comp_ptr = varint_encode(pid, comp_ptr);
    comp_ptr = varint_encode(len, comp_ptr);
    strncpy(comp_ptr, name, len);
    trace_pid.compressed_ptr = comp_ptr + len;
}

void trace_init_exec(unsigned long start, unsigned long end,
                     unsigned long offset, const char *exe)
{
}

void trace_bb_helper(uint64_t bb_num, TranslationBlock *tb)
{
    BBRec *bb_rec = tb->bb_rec;
    uint64_t prev_time = tb->prev_time;
    trace_bb.current_bb_addr = tb->pc;
    trace_bb.current_bb_num = bb_num;
    trace_bb.current_bb_start_time = sim_time;
    trace_bb.num_insns = 0;
    trace_bb.recnum += 1;

#if 0
    if (ftrace_debug)
        fprintf(ftrace_debug, "t%lld %lld\n", sim_time, bb_num);
#endif
    if (bb_rec && bb_rec->bb_num == bb_num && prev_time > trace_bb.flush_time) {
        uint64_t time_diff = sim_time - prev_time;
        if (bb_rec->repeat == 0) {
            bb_rec->repeat = 1;
            bb_rec->time_diff = time_diff;
            tb->prev_time = sim_time;
            return;
        } else if (time_diff == bb_rec->time_diff) {
            bb_rec->repeat += 1;
            tb->prev_time = sim_time;
            return;
        }
    }

    BBRec *next = trace_bb.next;
    if (next == &trace_bb.buffer[kMaxNumBasicBlocks]) {
        BBRec *ptr;
        char *comp_ptr = trace_bb.compressed_ptr;
        int64_t prev_bb_num = trace_bb.prev_bb_num;
        uint64_t prev_bb_time = trace_bb.prev_bb_time;
        for (ptr = trace_bb.buffer; ptr != next; ++ptr) {
            if (comp_ptr >= trace_bb.high_water_ptr) {
                uint32_t size = comp_ptr - trace_bb.compressed;
                fwrite(trace_bb.compressed, sizeof(char), size, trace_bb.fstream);
                comp_ptr = trace_bb.compressed;
            }
            int64_t bb_diff = ptr->bb_num - prev_bb_num;
            prev_bb_num = ptr->bb_num;
            uint64_t time_diff = ptr->start_time - prev_bb_time;
            prev_bb_time = ptr->start_time;
            comp_ptr = varint_encode_signed(bb_diff, comp_ptr);
            comp_ptr = varint_encode(time_diff, comp_ptr);
            comp_ptr = varint_encode(ptr->repeat, comp_ptr);
            if (ptr->repeat)
                comp_ptr = varint_encode(ptr->time_diff, comp_ptr);
        }
        trace_bb.compressed_ptr = comp_ptr;
        trace_bb.prev_bb_num = prev_bb_num;
        trace_bb.prev_bb_time = prev_bb_time;

        next = trace_bb.buffer;
        trace_bb.flush_time = sim_time;
    }
    tb->bb_rec = next;
    next->bb_num = bb_num;
    next->start_time = sim_time;
    next->time_diff = 0;
    next->repeat = 0;
    tb->prev_time = sim_time;
    next += 1;
    trace_bb.next = next;
}

void trace_insn_helper()
{
    InsnRec *current = trace_insn.current;
    uint64_t time_diff = sim_time - trace_insn.prev_time;
    trace_insn.prev_time = sim_time;

    
    
    trace_bb.num_insns += 1;

#if 0
    if (ftrace_debug) {
        uint32_t current_pc = trace_bb.current_bb_addr + 4 * (trace_bb.num_insns - 1);
        fprintf(ftrace_debug, "%llu %x\n", sim_time, current_pc);
    }
#endif
    if (time_diff == current->time_diff) {
        current->repeat += 1;
        if (current->repeat != 0)
            return;

        
        
        current->repeat -= 1;
    }
    current += 1;

    if (current == &trace_insn.buffer[kInsnBufferSize]) {
        InsnRec *ptr;
        char *comp_ptr = trace_insn.compressed_ptr;
        for (ptr = trace_insn.buffer; ptr != current; ++ptr) {
            if (comp_ptr >= trace_insn.high_water_ptr) {
                uint32_t size = comp_ptr - trace_insn.compressed;
                uint32_t rval = fwrite(trace_insn.compressed, sizeof(char),
                                       size, trace_insn.fstream);
                if (rval != size) {
                    fprintf(stderr, "fwrite() failed\n");
                    perror(trace_insn.filename);
                    exit(1);
                }
                comp_ptr = trace_insn.compressed;
            }
            comp_ptr = varint_encode(ptr->time_diff, comp_ptr);
            comp_ptr = varint_encode(ptr->repeat, comp_ptr);
        }
        trace_insn.compressed_ptr = comp_ptr;
        current = trace_insn.buffer;
    }
    current->time_diff = time_diff;
    current->repeat = 0;
    trace_insn.current = current;
}

void trace_interpreted_method(uint32_t addr, int call_type)
{
    if (trace_method.fstream == NULL)
        return;
#if 0
    fprintf(stderr, "trace_method time: %llu p%d 0x%x %d\n",
            sim_time, current_pid, addr, call_type);
#endif
    char *comp_ptr = trace_method.compressed_ptr;
    char *max_end_ptr = comp_ptr + kMaxMethodCompressed;
    if (max_end_ptr >= &trace_method.compressed[kCompressedSize]) {
        uint32_t size = comp_ptr - trace_method.compressed;
        fwrite(trace_method.compressed, sizeof(char), size, trace_method.fstream);
        comp_ptr = trace_method.compressed;
    }
    uint64_t time_diff = sim_time - trace_method.prev_time;
    trace_method.prev_time = sim_time;

    int32_t addr_diff = addr - trace_method.prev_addr;
    trace_method.prev_addr = addr;

    int32_t pid_diff = current_pid - trace_method.prev_pid;
    trace_method.prev_pid = current_pid;

    comp_ptr = varint_encode(time_diff, comp_ptr);
    comp_ptr = varint_encode_signed(addr_diff, comp_ptr);
    comp_ptr = varint_encode_signed(pid_diff, comp_ptr);
    comp_ptr = varint_encode(call_type, comp_ptr);
    trace_method.compressed_ptr = comp_ptr;
}

uint64_t trace_static_bb_num(void)
{
    return trace_static.bb_num;
}
