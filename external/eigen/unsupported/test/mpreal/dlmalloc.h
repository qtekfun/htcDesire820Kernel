/*
  Default header file for malloc-2.8.x, written by Doug Lea
  and released to the public domain, as explained at
  http://creativecommons.org/licenses/publicdomain. 
 
  last update: Wed May 27 14:25:17 2009  Doug Lea  (dl at gee)

  This header is for ANSI C/C++ only.  You can set any of
  the following #defines before including:

  * If USE_DL_PREFIX is defined, it is assumed that malloc.c 
    was also compiled with this option, so all routines
    have names starting with "dl".

  * If HAVE_USR_INCLUDE_MALLOC_H is defined, it is assumed that this
    file will be #included AFTER <malloc.h>. This is needed only if
    your system defines a struct mallinfo that is incompatible with the
    standard one declared here.  Otherwise, you can include this file
    INSTEAD of your system system <malloc.h>.  At least on ANSI, all
    declarations should be compatible with system versions

  * If MSPACES is defined, declarations for mspace versions are included.
*/

#ifndef MALLOC_280_H
#define MALLOC_280_H

#define USE_DL_PREFIX

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   

#ifndef ONLY_MSPACES
#define ONLY_MSPACES 0     
#endif  
#ifndef NO_MALLINFO
#define NO_MALLINFO 0
#endif  


#if !ONLY_MSPACES

#ifndef USE_DL_PREFIX
#define dlcalloc               calloc
#define dlfree                 free
#define dlmalloc               malloc
#define dlmemalign             memalign
#define dlrealloc              realloc
#define dlvalloc               valloc
#define dlpvalloc              pvalloc
#define dlmallinfo             mallinfo
#define dlmallopt              mallopt
#define dlmalloc_trim          malloc_trim
#define dlmalloc_stats         malloc_stats
#define dlmalloc_usable_size   malloc_usable_size
#define dlmalloc_footprint     malloc_footprint
#define dlindependent_calloc   independent_calloc
#define dlindependent_comalloc independent_comalloc
#endif 
#if !NO_MALLINFO 
#ifndef HAVE_USR_INCLUDE_MALLOC_H
#ifndef _MALLOC_H
#ifndef MALLINFO_FIELD_TYPE
#define MALLINFO_FIELD_TYPE size_t
#endif 
#ifndef STRUCT_MALLINFO_DECLARED
#define STRUCT_MALLINFO_DECLARED 1
struct mallinfo {
  MALLINFO_FIELD_TYPE arena;    
  MALLINFO_FIELD_TYPE ordblks;  
  MALLINFO_FIELD_TYPE smblks;   
  MALLINFO_FIELD_TYPE hblks;    
  MALLINFO_FIELD_TYPE hblkhd;   
  MALLINFO_FIELD_TYPE usmblks;  
  MALLINFO_FIELD_TYPE fsmblks;  
  MALLINFO_FIELD_TYPE uordblks; 
  MALLINFO_FIELD_TYPE fordblks; 
  MALLINFO_FIELD_TYPE keepcost; 
};
#endif 
#endif  
#endif  
#endif  

void* dlmalloc(size_t);

void  dlfree(void*);

void* dlcalloc(size_t, size_t);


void* dlrealloc(void*, size_t);

void* dlmemalign(size_t, size_t);

void* dlvalloc(size_t);

int dlmallopt(int, int);

#define M_TRIM_THRESHOLD     (-1)
#define M_GRANULARITY        (-2)
#define M_MMAP_THRESHOLD     (-3)


size_t dlmalloc_footprint();

#if !NO_MALLINFO

struct mallinfo dlmallinfo(void);
#endif  

void** dlindependent_calloc(size_t, size_t, void**);

void** dlindependent_comalloc(size_t, size_t*, void**);


void*  dlpvalloc(size_t);

int  dlmalloc_trim(size_t);

void  dlmalloc_stats();

#endif 

size_t dlmalloc_usable_size(void*);


#if MSPACES

typedef void* mspace;

mspace create_mspace(size_t capacity, int locked);

size_t destroy_mspace(mspace msp);

mspace create_mspace_with_base(void* base, size_t capacity, int locked);

int mspace_track_large_chunks(mspace msp, int enable);

void* mspace_malloc(mspace msp, size_t bytes);

void mspace_free(mspace msp, void* mem);

void* mspace_realloc(mspace msp, void* mem, size_t newsize);

void* mspace_calloc(mspace msp, size_t n_elements, size_t elem_size);

void* mspace_memalign(mspace msp, size_t alignment, size_t bytes);

void** mspace_independent_calloc(mspace msp, size_t n_elements,
                                 size_t elem_size, void* chunks[]);

void** mspace_independent_comalloc(mspace msp, size_t n_elements,
                                   size_t sizes[], void* chunks[]);

size_t mspace_footprint(mspace msp);


#if !NO_MALLINFO
struct mallinfo mspace_mallinfo(mspace msp);
#endif 

 size_t mspace_usable_size(void* mem);

void mspace_malloc_stats(mspace msp);

int mspace_trim(mspace msp, size_t pad);

int mspace_mallopt(int, int);

#endif  

#ifdef __cplusplus
};  
#endif

#endif 
