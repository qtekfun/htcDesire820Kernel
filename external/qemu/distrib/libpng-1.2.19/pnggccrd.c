
/* pnggccrd.c - mixed C/assembler version of utilities to read a PNG file
 *
 * For Intel/AMD x86 or x86-64 CPU (Pentium-MMX or later) and GNU C compiler.
 *
 * Last changed in libpng 1.2.19 August 18, 2007
 * For conditions of distribution and use, see copyright notice in png.h
 * Copyright (c) 1998 Intel Corporation
 * Copyright (c) 1999-2002,2007 Greg Roelofs
 * Copyright (c) 1998-2007 Glenn Randers-Pehrson
 *
 * Based on MSVC code contributed by Nirav Chhatrapati, Intel Corp., 1998.
 * Interface to libpng contributed by Gilles Vollant, 1999.
 * GNU C port by Greg Roelofs, 1999-2001.
 *
 * References:
 *
 *     http://www.intel.com/drg/pentiumII/appnotes/916/916.htm
 *     http://www.intel.com/drg/pentiumII/appnotes/923/923.htm
 *       [Intel's performance analysis of the MMX vs. non-MMX code;
 *        moved/deleted as of 2006, but text and some graphs still
 *        available via WayBack Machine at archive.org]
 *
 *     http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
 *     http://sam.zoy.org/blog/2007-04-13-shlib-with-non-pic-code-have-inline-assembly-and-pic-mix-well
 *     http://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
 *     http://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html
 *     http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 *     AMD64 Architecture Programmer's Manual, volumes 1 and 5
 *       [http://www.amd.com/us-en/Processors/TechnicalResources/0,,30_182_739_7044,00.html]
 *     Intel 64 and IA-32 Software Developer's Manuals
 *       [http://developer.intel.com/products/processor/manuals/]
 *
 * png_read_filter_row_mmx_*() were converted in place with intel2gas 1.3.1:
 *
 *     intel2gas -mdI pnggccrd.c.partially-msvc -o pnggccrd.c
 *
 * and then cleaned up by hand.  See http://hermes.terminal.at/intel2gas/ .
 *
 * NOTE:  A sufficiently recent version of GNU as (or as.exe under DOS/Windows)
 * is required to assemble the newer asm instructions such as movq.  (Version
 * 2.5.2l.15 is definitely too old.)  See ftp://ftp.gnu.org/pub/gnu/binutils/ .
 */


#if defined(__GNUC__)

#define PNG_INTERNAL
#include "png.h"


#if defined(PIC) && !defined(__PIC__)   
#  define __PIC__
#endif

#if defined(PNG_ASSEMBLER_CODE_SUPPORTED) && defined(PNG_USE_PNGGCCRD)

#if defined(__x86_64__) && defined(__PIC__)
#  define PNG_x86_64_USE_GOTPCREL            
#  define PNG_CLOBBER_x86_64_REGS_SUPPORTED  
#endif

int PNGAPI png_mmx_support(void);

#if defined(PNG_USE_LOCAL_ARRAYS)
static PNG_CONST int FARDATA png_pass_start[7] = {0, 4, 0, 2, 0, 1, 0};
static PNG_CONST int FARDATA png_pass_inc[7]   = {8, 8, 4, 4, 2, 2, 1};
static PNG_CONST int FARDATA png_pass_width[7] = {8, 4, 4, 2, 2, 1, 1};
#endif

#if defined(__DJGPP__) || defined(WIN32) || defined(__CYGWIN__) || \
    defined(__OS2__)
#  define _mmx_supported  mmx_supported
#  define _mask8_0        mask8_0
#  define _mask16_1       mask16_1
#  define _mask16_0       mask16_0
#  define _mask24_2       mask24_2
#  define _mask24_1       mask24_1
#  define _mask24_0       mask24_0
#  define _mask32_3       mask32_3
#  define _mask32_2       mask32_2
#  define _mask32_1       mask32_1
#  define _mask32_0       mask32_0
#  define _mask48_5       mask48_5
#  define _mask48_4       mask48_4
#  define _mask48_3       mask48_3
#  define _mask48_2       mask48_2
#  define _mask48_1       mask48_1
#  define _mask48_0       mask48_0
#  define _amask5_3_0     amask5_3_0
#  define _amask7_1_0     amask7_1_0
#  define _LBCarryMask    LBCarryMask
#  define _HBClearMask    HBClearMask
#  define _amask0_8_0     amask0_8_0
#  define _amask6_2_0     amask6_2_0
#  define _amask4_4_0     amask4_4_0
#  define _amask0_2_6     amask0_2_6
#  define _amask2_3_3     amask2_3_3
#  define _amask4_2_2     amask4_2_2
#  if defined(PNG_THREAD_UNSAFE_OK)
#    define _patemp       patemp
#    define _pbtemp       pbtemp
#    define _pctemp       pctemp
#  endif
#endif 



typedef unsigned long long  ull;

#if defined(PNG_x86_64_USE_GOTPCREL)
static PNG_CONST struct {
    

    
    ull _mask8_0;
    ull _mask16_0, _mask16_1;
    ull _mask24_0, _mask24_1, _mask24_2;
    ull _mask32_0, _mask32_1, _mask32_2, _mask32_3;
    ull _mask48_0, _mask48_1, _mask48_2, _mask48_3, _mask48_4, _mask48_5;

    
    ull _amask5_3_0, _amask7_1_0;  

    
    ull _LBCarryMask, _HBClearMask;
    ull _amask0_8_0, _amask6_2_0;  

    
    ull _amask4_4_0, _amask0_2_6;  

    
    ull _amask2_3_3, _amask4_2_2;  

} _c64 __attribute__((used, aligned(8))) = {

    
    0x0102040810204080LL, 

    0x1010202040408080LL, 
    0x0101020204040808LL, 

    0x2020404040808080LL, 
    0x0408080810101020LL, 
    0x0101010202020404LL, 

    0x4040404080808080LL, 
    0x1010101020202020LL, 
    0x0404040408080808LL, 
    0x0101010102020202LL, 

    0x4040808080808080LL, 
    0x2020202040404040LL, 
    0x1010101010102020LL, 
    0x0404080808080808LL, 
    0x0202020204040404LL, 
    0x0101010101010202LL, 

    
    0x0000000000FFFFFFLL, 
    0x00000000000000FFLL, 

    
    0x0101010101010101LL, 
    0x7F7F7F7F7F7F7F7FLL, 
    0xFFFFFFFFFFFFFFFFLL, 
    0x000000000000FFFFLL, 

    
    0x00000000FFFFFFFFLL, 
    0xFFFF000000000000LL, 

    
    0x0000FFFFFF000000LL, 
    0x00000000FFFF0000LL, 

};

#define MASK8_0        "(%%rbp)"
#define MASK16_0       "8(%%rbp)"
#define MASK16_1       "16(%%rbp)"
#define MASK24_0       "24(%%rbp)"
#define MASK24_1       "32(%%rbp)"
#define MASK24_2       "40(%%rbp)"
#define MASK32_0       "48(%%rbp)"
#define MASK32_1       "56(%%rbp)"
#define MASK32_2       "64(%%rbp)"
#define MASK32_3       "72(%%rbp)"
#define MASK48_0       "80(%%rbp)"
#define MASK48_1       "88(%%rbp)"
#define MASK48_2       "96(%%rbp)"
#define MASK48_3       "104(%%rbp)"
#define MASK48_4       "112(%%rbp)"
#define MASK48_5       "120(%%rbp)"
#define AMASK5_3_0     "128(%%rbp)"
#define AMASK7_1_0     "136(%%rbp)"
#define LB_CARRY_MASK  "144(%%rbp)"
#define HB_CLEAR_MASK  "152(%%rbp)"
#define AMASK0_8_0     "160(%%rbp)"
#define AMASK6_2_0     "168(%%rbp)"
#define AMASK4_4_0     "176(%%rbp)"
#define AMASK0_2_6     "184(%%rbp)"
#define AMASK2_3_3     "192(%%rbp)"
#define AMASK4_2_2     "200(%%rbp)"

#else 

static PNG_CONST ull _mask8_0  __attribute__((used, aligned(8))) = 0x0102040810204080LL;

static PNG_CONST ull _mask16_1 __attribute__((used, aligned(8))) = 0x0101020204040808LL;
static PNG_CONST ull _mask16_0 __attribute__((used, aligned(8))) = 0x1010202040408080LL;

static PNG_CONST ull _mask24_2 __attribute__((used, aligned(8))) = 0x0101010202020404LL;
static PNG_CONST ull _mask24_1 __attribute__((used, aligned(8))) = 0x0408080810101020LL;
static PNG_CONST ull _mask24_0 __attribute__((used, aligned(8))) = 0x2020404040808080LL;

static PNG_CONST ull _mask32_3 __attribute__((used, aligned(8))) = 0x0101010102020202LL;
static PNG_CONST ull _mask32_2 __attribute__((used, aligned(8))) = 0x0404040408080808LL;
static PNG_CONST ull _mask32_1 __attribute__((used, aligned(8))) = 0x1010101020202020LL;
static PNG_CONST ull _mask32_0 __attribute__((used, aligned(8))) = 0x4040404080808080LL;

static PNG_CONST ull _mask48_5 __attribute__((used, aligned(8))) = 0x0101010101010202LL;
static PNG_CONST ull _mask48_4 __attribute__((used, aligned(8))) = 0x0202020204040404LL;
static PNG_CONST ull _mask48_3 __attribute__((used, aligned(8))) = 0x0404080808080808LL;
static PNG_CONST ull _mask48_2 __attribute__((used, aligned(8))) = 0x1010101010102020LL;
static PNG_CONST ull _mask48_1 __attribute__((used, aligned(8))) = 0x2020202040404040LL;
static PNG_CONST ull _mask48_0 __attribute__((used, aligned(8))) = 0x4040808080808080LL;

static PNG_CONST ull _amask5_3_0  __attribute__((aligned(8))) = 0x0000000000FFFFFFLL;  
static PNG_CONST ull _amask7_1_0  __attribute__((aligned(8))) = 0x00000000000000FFLL;  

static PNG_CONST ull _LBCarryMask __attribute__((used, aligned(8))) = 0x0101010101010101LL;
static PNG_CONST ull _HBClearMask __attribute__((used, aligned(8))) = 0x7f7f7f7f7f7f7f7fLL;
static PNG_CONST ull _amask0_8_0  __attribute__((used, aligned(8))) = 0xFFFFFFFFFFFFFFFFLL;
static PNG_CONST ull _amask6_2_0  __attribute__((used, aligned(8))) = 0x000000000000FFFFLL;

static PNG_CONST ull _amask4_4_0  __attribute__((used, aligned(8))) = 0x00000000FFFFFFFFLL;
static PNG_CONST ull _amask0_2_6  __attribute__((used, aligned(8))) = 0xFFFF000000000000LL;

static PNG_CONST ull _amask2_3_3  __attribute__((used, aligned(8))) = 0x0000FFFFFF000000LL;
static PNG_CONST ull _amask4_2_2  __attribute__((used, aligned(8))) = 0x00000000FFFF0000LL;

#define MASK8_0        "_mask8_0"
#define MASK16_0       "_mask16_0"
#define MASK16_1       "_mask16_1"
#define MASK24_0       "_mask24_0"
#define MASK24_1       "_mask24_1"
#define MASK24_2       "_mask24_2"
#define MASK32_0       "_mask32_0"
#define MASK32_1       "_mask32_1"
#define MASK32_2       "_mask32_2"
#define MASK32_3       "_mask32_3"
#define MASK48_0       "_mask48_0"
#define MASK48_1       "_mask48_1"
#define MASK48_2       "_mask48_2"
#define MASK48_3       "_mask48_3"
#define MASK48_4       "_mask48_4"
#define MASK48_5       "_mask48_5"
#define AMASK5_3_0     "_amask5_3_0"
#define AMASK7_1_0     "_amask7_1_0"
#define LB_CARRY_MASK  "_LBCarryMask"
#define HB_CLEAR_MASK  "_HBClearMask"
#define AMASK0_8_0     "_amask0_8_0"
#define AMASK6_2_0     "_amask6_2_0"
#define AMASK4_4_0     "_amask4_4_0"
#define AMASK0_2_6     "_amask0_2_6"
#define AMASK2_3_3     "_amask2_3_3"
#define AMASK4_2_2     "_amask4_2_2"

#endif 


#if defined(PNG_HAVE_MMX_READ_FILTER_ROW) || defined(PNG_HAVE_MMX_COMBINE_ROW)

#if defined(PNG_x86_64_USE_GOTPCREL)
#  define pa_TEMP                "%%r11d"
#  define pb_TEMP                "%%r12d"
#  define pc_TEMP                "%%r13d"
#  if defined(PNG_CLOBBER_x86_64_REGS_SUPPORTED)  
#    define SAVE_r11_r12_r13
#    define RESTORE_r11_r12_r13
#    define _CLOBBER_r11_r12_r13 ,"%r11", "%r12", "%r13"
#    define CLOBBER_r11_r12_r13  "%r11", "%r12", "%r13"
#  else 
#    define SAVE_r11_r12_r13     "pushq %%r11  \n\t" \
                                 "pushq %%r12  \n\t" \
                                 "pushq %%r13  \n\t"  
#    define RESTORE_r11_r12_r13  "popq  %%r13  \n\t" \
                                 "popq  %%r12  \n\t" \
                                 "popq  %%r11  \n\t"
#    define _CLOBBER_r11_r12_r13
#    define CLOBBER_r11_r12_r13
#  endif
#  define LOAD_GOT_rbp           "pushq %%rbp                        \n\t" \
                                 "movq  _c64@GOTPCREL(%%rip), %%rbp  \n\t"
#  define RESTORE_rbp            "popq  %%rbp                        \n\t"
#else 
#  if defined(PNG_THREAD_UNSAFE_OK)
     
     
     
     
     
     static int                     _patemp  __attribute__((used));
     static int                     _pbtemp  __attribute__((used));
     static int                     _pctemp  __attribute__((used));
#    define pa_TEMP                "_patemp"
#    define pb_TEMP                "_pbtemp"  
#    define pc_TEMP                "_pctemp"  
#    define SAVE_r11_r12_r13
#    define RESTORE_r11_r12_r13
#    define _CLOBBER_r11_r12_r13   
#    define CLOBBER_r11_r12_r13
#  endif 
#  define LOAD_GOT_rbp
#  define RESTORE_rbp
#endif

#if defined(__x86_64__)
#  define SAVE_ebp
#  define RESTORE_ebp
#  define _CLOBBER_ebp         ,"%ebp"
#  define CLOBBER_ebp          "%ebp"
#  define SAVE_FullLength      "movl %%eax, %%r15d  \n\t"
#  define RESTORE_FullLength   "movl %%r15d, "     
#  if defined(PNG_CLOBBER_x86_64_REGS_SUPPORTED)   
#    define SAVE_r15
#    define RESTORE_r15
#    define _CLOBBER_r15       ,"%r15"
#  else
#    define SAVE_r15           "pushq %%r15  \n\t"
#    define RESTORE_r15        "popq  %%r15  \n\t"
#    define _CLOBBER_r15
#  endif
#  define PBP                  "%%rbp"             
#  define PAX                  "%%rax"             
#  define PBX                  "%%rbx"             
#  define PCX                  "%%rcx"             
#  define PDX                  "%%rdx"             
#  define PSI                  "%%rsi"             
#  define CLEAR_BOTTOM_3_BITS  "and  $0xfffffffffffffff8, "
#else
#  define SAVE_ebp             "pushl %%ebp \n\t"  
#  define RESTORE_ebp          "popl  %%ebp \n\t"  
#  define _CLOBBER_ebp                             
#  define CLOBBER_ebp
#  define SAVE_FullLength      "pushl %%eax \n\t"
#  define RESTORE_FullLength   "popl "             
#  define SAVE_r15
#  define RESTORE_r15
#  define _CLOBBER_r15
#  define PBP                  "%%ebp"             
#  define PAX                  "%%eax"             
#  define PBX                  "%%ebx"             
#  define PCX                  "%%ecx"             
#  define PDX                  "%%edx"
#  define PSI                  "%%esi"
#  define CLEAR_BOTTOM_3_BITS  "and  $0xfffffff8, "
#endif

#if defined(__x86_64__) && !defined(__PIC__)
#  define CLOB_COMMA_ebx_ebp    , 
#else
#  define CLOB_COMMA_ebx_ebp
#endif

#if (!defined(__x86_64__) && defined(__PIC__)) || \
    !defined(PNG_x86_64_USE_GOTPCREL) || \
    !defined(PNG_CLOBBER_x86_64_REGS_SUPPORTED)
#  define CLOB_COMMA_ebX_r1X
#else
#  define CLOB_COMMA_ebX_r1X    , 
#endif

#if (!defined(__x86_64__) && defined(__PIC__))
#  define CLOB_COLON_ebx_ebp
#  if !(defined(PNG_x86_64_USE_GOTPCREL) && \
        defined(PNG_CLOBBER_x86_64_REGS_SUPPORTED))
#    define CLOB_COLON_ebx_ebp_r1X
#  else
#    define CLOB_COLON_ebx_ebp_r1X  : 
#  endif
#else
#  define CLOB_COLON_ebx_ebp        : 
#  define CLOB_COLON_ebx_ebp_r1X    : 
#endif

#endif 

#if defined(__PIC__)  
#  if defined(__x86_64__)
#    define SAVE_GOT_ebx     "pushq %%rbx \n\t"
#    define RESTORE_GOT_ebx  "popq  %%rbx \n\t"
#  else
#    define SAVE_GOT_ebx     "pushl %%ebx \n\t"
#    define RESTORE_GOT_ebx  "popl  %%ebx \n\t"
#  endif
#  define _CLOBBER_GOT_ebx   
#  define CLOBBER_GOT_ebx
#else
#  define SAVE_GOT_ebx
#  define RESTORE_GOT_ebx
#  define _CLOBBER_GOT_ebx   ,"%ebx"
#  define CLOBBER_GOT_ebx    "%ebx"
#endif

#if defined(PNG_HAVE_MMX_COMBINE_ROW) || defined(PNG_HAVE_MMX_READ_INTERLACE)
#  define BPP2  2
#  define BPP3  3  
#  define BPP4  4  
#  define BPP6  6
#  define BPP8  8
#endif



static int _mmx_supported = 2; 



int PNGAPI
png_mmx_support(void) __attribute__((noinline));

int PNGAPI
png_mmx_support(void)
{
#if defined(PNG_MMX_CODE_SUPPORTED)  
    int result;
    __asm__ __volatile__ (
#if defined(__x86_64__)
        "pushq %%rbx          \n\t"  
        "pushq %%rcx          \n\t"  
        "pushq %%rdx          \n\t"  
        "pushfq               \n\t"  
        "popq %%rax           \n\t"  
        "movq %%rax, %%rcx    \n\t"  
        "xorl $0x200000, %%eax \n\t" 
        "pushq %%rax          \n\t"  
        "popfq                \n\t"  
        "pushfq               \n\t"  
        "popq %%rax           \n\t"  
        "pushq %%rcx          \n\t"  
        "popfq                \n\t"  
#else
        "pushl %%ebx          \n\t"  
        "pushl %%ecx          \n\t"  
        "pushl %%edx          \n\t"  
        "pushfl               \n\t"  
        "popl %%eax           \n\t"  
        "movl %%eax, %%ecx    \n\t"  
        "xorl $0x200000, %%eax \n\t" 
        "pushl %%eax          \n\t"  
        "popfl                \n\t"  
        "pushfl               \n\t"  
        "popl %%eax           \n\t"  
        "pushl %%ecx          \n\t"  
        "popfl                \n\t"  
#endif
        "xorl %%ecx, %%eax    \n\t"  
        "jz 0f                \n\t"  

        "xorl %%eax, %%eax    \n\t"  
        "cpuid                \n\t"  
        "cmpl $1, %%eax       \n\t"  
        "jl 0f                \n\t"  

        "xorl %%eax, %%eax    \n\t"  
        "incl %%eax           \n\t"  
                                     
        "cpuid                \n\t"  
        "andl $0x800000, %%edx \n\t" 
        "cmpl $0, %%edx       \n\t"  
        "jz 0f                \n\t"  

        "movl $1, %%eax       \n\t"  
        "jmp  1f              \n\t"  

    "0:                       \n\t"  
        "movl $0, %%eax       \n\t"  
    "1:                       \n\t"  
#if defined(__x86_64__)
        "popq %%rdx           \n\t"  
        "popq %%rcx           \n\t"  
        "popq %%rbx           \n\t"  
#else
        "popl %%edx           \n\t"  
        "popl %%ecx           \n\t"  
        "popl %%ebx           \n\t"  
#endif

                                     

        : "=a" (result)              

        :                            

                                     
    );
    _mmx_supported = result;
#else
    _mmx_supported = 0;
#endif 

    return _mmx_supported;
}



#if defined(PNG_HAVE_MMX_COMBINE_ROW)



void 
png_combine_row(png_structp png_ptr, png_bytep row, int mask)
{
   int dummy_value_a;    
   int dummy_value_c;
   int dummy_value_d;
   png_bytep dummy_value_S;
   png_bytep dummy_value_D;

   png_debug(1, "in png_combine_row (pnggccrd.c)\n");

   if (_mmx_supported == 2) {
#if !defined(PNG_1_0_X)
       
       png_warning(png_ptr, "asm_flags may not have been initialized");
#endif
       png_mmx_support();
   }

   if (mask == 0xff)
   {
      png_debug(2,"mask == 0xff:  doing single png_memcpy()\n");
      png_memcpy(row, png_ptr->row_buf + 1,
       (png_size_t)PNG_ROWBYTES(png_ptr->row_info.pixel_depth,png_ptr->width));
   }
   else   
   {
      switch (png_ptr->row_info.pixel_depth)
      {
         case 24:       
         {
            png_bytep srcptr;
            png_bytep dstptr;

#if !defined(PNG_1_0_X)
            if (png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_COMBINE_ROW)
#else
            if (_mmx_supported)
#endif
            {
               png_uint_32 len;
               int diff;

               srcptr = png_ptr->row_buf + 1;
               dstptr = row;
               len  = png_ptr->width & ~7;          
               diff = (int) (png_ptr->width & 7);   

               __asm__ __volatile__ (
                  "not       %%edx            \n\t" 
                  "movd      %%edx, %%mm7     \n\t" 
                  "not       %%edx            \n\t" 
                  "psubb     %%mm6, %%mm6     \n\t" 
                  "punpcklbw %%mm7, %%mm7     \n\t"
                  "punpcklwd %%mm7, %%mm7     \n\t"
                  "punpckldq %%mm7, %%mm7     \n\t" 

                  LOAD_GOT_rbp
                  "movq   " MASK24_0 ", %%mm0 \n\t" 
                  "movq   " MASK24_1 ", %%mm1 \n\t" 
                  "movq   " MASK24_2 ", %%mm2 \n\t" 
                  RESTORE_rbp

                  "pand      %%mm7, %%mm0     \n\t"
                  "pand      %%mm7, %%mm1     \n\t"
                  "pand      %%mm7, %%mm2     \n\t"

                  "pcmpeqb   %%mm6, %%mm0     \n\t"
                  "pcmpeqb   %%mm6, %%mm1     \n\t"
                  "pcmpeqb   %%mm6, %%mm2     \n\t"


                  "cmpl      $0, %%ecx        \n\t"
                  "jz        mainloop24end    \n\t"

                "mainloop24:                  \n\t"
                  "movq      (%3), %%mm4      \n\t"
                  "pand      %%mm0, %%mm4     \n\t"
                  "movq      %%mm0, %%mm6     \n\t"
                  "movq      (%4), %%mm7      \n\t"
                  "pandn     %%mm7, %%mm6     \n\t"
                  "por       %%mm6, %%mm4     \n\t"
                  "movq      %%mm4, (%4)      \n\t"

                  "movq      8(%3), %%mm5     \n\t"
                  "pand      %%mm1, %%mm5     \n\t"
                  "movq      %%mm1, %%mm7     \n\t"
                  "movq      8(%4), %%mm6     \n\t"
                  "pandn     %%mm6, %%mm7     \n\t"
                  "por       %%mm7, %%mm5     \n\t"
                  "movq      %%mm5, 8(%4)     \n\t"

                  "movq      16(%3), %%mm6    \n\t"
                  "pand      %%mm2, %%mm6     \n\t"
                  "movq      %%mm2, %%mm4     \n\t"
                  "movq      16(%4), %%mm7    \n\t"
                  "pandn     %%mm7, %%mm4     \n\t"
                  "por       %%mm4, %%mm6     \n\t"
                  "movq      %%mm6, 16(%4)    \n\t"

                  "add       $24, %3          \n\t" 
                  "add       $24, %4          \n\t"
                  "subl      $8, %%ecx        \n\t" 

                  "ja        mainloop24       \n\t"

                "mainloop24end:               \n\t"
                  "movl      %%eax, %%ecx     \n\t"
                  "cmpl      $0, %%ecx        \n\t"
                  "jz        end24            \n\t"
                  "sall      $24, %%edx       \n\t" 

                "secondloop24:                \n\t"
                  "sall      %%edx            \n\t" 
                  "jnc       skip24           \n\t" 
                  "movw      (%3), %%ax       \n\t"
                  "movw      %%ax, (%4)       \n\t"
                  "xorl      %%eax, %%eax     \n\t"
                  "movb      2(%3), %%al      \n\t"
                  "movb      %%al, 2(%4)      \n\t"

                "skip24:                      \n\t"
                  "add       $3, %3           \n\t"
                  "add       $3, %4           \n\t"
                  "decl      %%ecx            \n\t"
                  "jnz       secondloop24     \n\t"

                "end24:                       \n\t"
                  "EMMS                       \n\t" 

                  : "=a" (dummy_value_a),           
                    "=d" (dummy_value_d),
                    "=c" (dummy_value_c),
                    "=S" (dummy_value_S),
                    "=D" (dummy_value_D)

                  : "0" (diff),        
                    "1" (mask),        
                    "2" (len),         
                    "3" (srcptr),      
                    "4" (dstptr)       

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                  : "%mm0", "%mm1", "%mm2"          
                  , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
               );
            }
            else 
            {
               register png_uint_32 i;
               png_uint_32 initial_val = BPP3 * png_pass_start[png_ptr->pass];
                 
               register int stride = BPP3 * png_pass_inc[png_ptr->pass];
                 
               register int rep_bytes = BPP3 * png_pass_width[png_ptr->pass];
                 
               png_uint_32 len = png_ptr->width &~7;  
               int diff = (int) (png_ptr->width & 7); 
               register png_uint_32 final_val = BPP3 * len;   

               srcptr = png_ptr->row_buf + 1 + initial_val;
               dstptr = row + initial_val;

               for (i = initial_val; i < final_val; i += stride)
               {
                  png_memcpy(dstptr, srcptr, rep_bytes);
                  srcptr += stride;
                  dstptr += stride;
               }
               if (diff)  
               {
                  final_val += diff*BPP3;
                  for (; i < final_val; i += stride)
                  {
                     if (rep_bytes > (int)(final_val-i))
                        rep_bytes = (int)(final_val-i);
                     png_memcpy(dstptr, srcptr, rep_bytes);
                     srcptr += stride;
                     dstptr += stride;
                  }
               }
            } 

            break;
         }       

         
         
         case 32:       
         {
            png_bytep srcptr;
            png_bytep dstptr;

#if !defined(PNG_1_0_X)
            if (png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_COMBINE_ROW)
#else
            if (_mmx_supported)
#endif
            {
               png_uint_32 len;
               int diff;

               srcptr = png_ptr->row_buf + 1;
               dstptr = row;
               len  = png_ptr->width & ~7;          
               diff = (int) (png_ptr->width & 7);   

               __asm__ __volatile__ (
                  "not       %%edx            \n\t" 
                  "movd      %%edx, %%mm7     \n\t" 
                  "not       %%edx            \n\t" 
                  "psubb     %%mm6, %%mm6     \n\t" 
                  "punpcklbw %%mm7, %%mm7     \n\t"
                  "punpcklwd %%mm7, %%mm7     \n\t"
                  "punpckldq %%mm7, %%mm7     \n\t" 

                  LOAD_GOT_rbp
                  "movq   " MASK32_0 ", %%mm0 \n\t" 
                  "movq   " MASK32_1 ", %%mm1 \n\t" 
                  "movq   " MASK32_2 ", %%mm2 \n\t" 
                  "movq   " MASK32_3 ", %%mm3 \n\t" 
                  RESTORE_rbp

                  "pand      %%mm7, %%mm0     \n\t"
                  "pand      %%mm7, %%mm1     \n\t"
                  "pand      %%mm7, %%mm2     \n\t"
                  "pand      %%mm7, %%mm3     \n\t"

                  "pcmpeqb   %%mm6, %%mm0     \n\t"
                  "pcmpeqb   %%mm6, %%mm1     \n\t"
                  "pcmpeqb   %%mm6, %%mm2     \n\t"
                  "pcmpeqb   %%mm6, %%mm3     \n\t"


                  "cmpl      $0, %%ecx        \n\t" 
                  "jz        mainloop32end    \n\t"

                "mainloop32:                  \n\t"
                  "movq      (%3), %%mm4      \n\t"
                  "pand      %%mm0, %%mm4     \n\t"
                  "movq      %%mm0, %%mm6     \n\t"
                  "movq      (%4), %%mm7      \n\t"
                  "pandn     %%mm7, %%mm6     \n\t"
                  "por       %%mm6, %%mm4     \n\t"
                  "movq      %%mm4, (%4)      \n\t"

                  "movq      8(%3), %%mm5     \n\t"
                  "pand      %%mm1, %%mm5     \n\t"
                  "movq      %%mm1, %%mm7     \n\t"
                  "movq      8(%4), %%mm6     \n\t"
                  "pandn     %%mm6, %%mm7     \n\t"
                  "por       %%mm7, %%mm5     \n\t"
                  "movq      %%mm5, 8(%4)     \n\t"

                  "movq      16(%3), %%mm6    \n\t"
                  "pand      %%mm2, %%mm6     \n\t"
                  "movq      %%mm2, %%mm4     \n\t"
                  "movq      16(%4), %%mm7    \n\t"
                  "pandn     %%mm7, %%mm4     \n\t"
                  "por       %%mm4, %%mm6     \n\t"
                  "movq      %%mm6, 16(%4)    \n\t"

                  "movq      24(%3), %%mm7    \n\t"
                  "pand      %%mm3, %%mm7     \n\t"
                  "movq      %%mm3, %%mm5     \n\t"
                  "movq      24(%4), %%mm4    \n\t"
                  "pandn     %%mm4, %%mm5     \n\t"
                  "por       %%mm5, %%mm7     \n\t"
                  "movq      %%mm7, 24(%4)    \n\t"

                  "add       $32, %3          \n\t" 
                  "add       $32, %4          \n\t"
                  "subl      $8, %%ecx        \n\t" 
                  "ja        mainloop32       \n\t"

                "mainloop32end:               \n\t"
                  "movl      %%eax, %%ecx     \n\t"
                  "cmpl      $0, %%ecx        \n\t"
                  "jz        end32            \n\t"
                  "sall      $24, %%edx       \n\t" 

                "secondloop32:                \n\t"
                  "sall      %%edx            \n\t" 
                  "jnc       skip32           \n\t" 
                  "movl      (%3), %%eax      \n\t"
                  "movl      %%eax, (%4)      \n\t"

                "skip32:                      \n\t"
                  "add       $4, %3           \n\t"
                  "add       $4, %4           \n\t"
                  "decl      %%ecx            \n\t"
                  "jnz       secondloop32     \n\t"

                "end32:                       \n\t"
                  "EMMS                       \n\t" 

                  : "=a" (dummy_value_a),           
                    "=d" (dummy_value_d),
                    "=c" (dummy_value_c),
                    "=S" (dummy_value_S),
                    "=D" (dummy_value_D)

                  : "0" (diff),        
                    "1" (mask),        
                    "2" (len),         
                    "3" (srcptr),      
                    "4" (dstptr)       

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                  : "%mm0", "%mm1", "%mm2", "%mm3"  
                  , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
               );
            }
            else 
            {
               register png_uint_32 i;
               png_uint_32 initial_val = BPP4 * png_pass_start[png_ptr->pass];
                 
               register int stride = BPP4 * png_pass_inc[png_ptr->pass];
                 
               register int rep_bytes = BPP4 * png_pass_width[png_ptr->pass];
                 
               png_uint_32 len = png_ptr->width &~7;  
               int diff = (int) (png_ptr->width & 7); 
               register png_uint_32 final_val = BPP4 * len;   

               srcptr = png_ptr->row_buf + 1 + initial_val;
               dstptr = row + initial_val;

               for (i = initial_val; i < final_val; i += stride)
               {
                  png_memcpy(dstptr, srcptr, rep_bytes);
                  srcptr += stride;
                  dstptr += stride;
               }
               if (diff)  
               {
                  final_val += diff*BPP4;
                  for (; i < final_val; i += stride)
                  {
                     if (rep_bytes > (int)(final_val-i))
                        rep_bytes = (int)(final_val-i);
                     png_memcpy(dstptr, srcptr, rep_bytes);
                     srcptr += stride;
                     dstptr += stride;
                  }
               }
            } 

            break;
         }       

         case 8:        
         {
            png_bytep srcptr;
            png_bytep dstptr;

#if !defined(PNG_1_0_X)
            if (png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_COMBINE_ROW)
#else
            if (_mmx_supported)
#endif
            {
               png_uint_32 len;
               int diff;

               srcptr = png_ptr->row_buf + 1;
               dstptr = row;
               len  = png_ptr->width & ~7;          
               diff = (int) (png_ptr->width & 7);   

               __asm__ __volatile__ (
                  "not       %%edx            \n\t" 
                  "movd      %%edx, %%mm7     \n\t" 
                  "not       %%edx            \n\t" 
                  "psubb     %%mm6, %%mm6     \n\t" 
                  "punpcklbw %%mm7, %%mm7     \n\t"
                  "punpcklwd %%mm7, %%mm7     \n\t"
                  "punpckldq %%mm7, %%mm7     \n\t" 

                  LOAD_GOT_rbp
                  "movq   " MASK8_0 ", %%mm0  \n\t" 
                  RESTORE_rbp

                  "pand      %%mm7, %%mm0     \n\t" 
                  "pcmpeqb   %%mm6, %%mm0     \n\t" 


                  "cmpl      $0, %%ecx        \n\t" 
                  "je        mainloop8end     \n\t"

                "mainloop8:                   \n\t"
                  "movq      (%3), %%mm4      \n\t" 
                  "pand      %%mm0, %%mm4     \n\t"
                  "movq      %%mm0, %%mm6     \n\t"
                  "pandn     (%4), %%mm6      \n\t" 
                  "por       %%mm6, %%mm4     \n\t"
                  "movq      %%mm4, (%4)      \n\t"
                  "add       $8, %3           \n\t" 
                  "add       $8, %4           \n\t"
                  "subl      $8, %%ecx        \n\t" 
                  "ja        mainloop8        \n\t"

                "mainloop8end:                \n\t"
                  "movl      %%eax, %%ecx     \n\t"
                  "cmpl      $0, %%ecx        \n\t"
                  "jz        end8             \n\t"
                  "sall      $24, %%edx       \n\t" 

                "secondloop8:                 \n\t"
                  "sall      %%edx            \n\t" 
                  "jnc       skip8            \n\t" 
                  "movb      (%3), %%al       \n\t"
                  "movb      %%al, (%4)       \n\t"

                "skip8:                       \n\t"
                  "inc       %3               \n\t"
                  "inc       %4               \n\t"
                  "decl      %%ecx            \n\t"
                  "jnz       secondloop8      \n\t"

                "end8:                        \n\t"
                  "EMMS                       \n\t" 

                  : "=a" (dummy_value_a),           
                    "=d" (dummy_value_d),
                    "=c" (dummy_value_c),
                    "=S" (dummy_value_S),
                    "=D" (dummy_value_D)

                  : "0" (diff),        
                    "1" (mask),        
                    "2" (len),         
                    "3" (srcptr),      
                    "4" (dstptr)       

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                  : "%mm0", "%mm4", "%mm6", "%mm7"  
#endif
               );
            }
            else 
            {
               register png_uint_32 i;
               png_uint_32 initial_val = png_pass_start[png_ptr->pass];
                 
               register int stride = png_pass_inc[png_ptr->pass];
                 
               register int rep_bytes = png_pass_width[png_ptr->pass];
                 
               png_uint_32 len = png_ptr->width &~7;  
               int diff = (int) (png_ptr->width & 7); 
               register png_uint_32 final_val = len;  

               srcptr = png_ptr->row_buf + 1 + initial_val;
               dstptr = row + initial_val;

               for (i = initial_val; i < final_val; i += stride)
               {
                  png_memcpy(dstptr, srcptr, rep_bytes);
                  srcptr += stride;
                  dstptr += stride;
               }
               if (diff)  
               {
                  final_val += diff  ;
                  for (; i < final_val; i += stride)
                  {
                     if (rep_bytes > (int)(final_val-i))
                        rep_bytes = (int)(final_val-i);
                     png_memcpy(dstptr, srcptr, rep_bytes);
                     srcptr += stride;
                     dstptr += stride;
                  }
               }

            } 

            break;
         }       

         case 1:        
         {
            png_bytep sp;
            png_bytep dp;
            int s_inc, s_start, s_end;
            int m;
            int shift;
            png_uint_32 i;

            sp = png_ptr->row_buf + 1;
            dp = row;
            m = 0x80;
#if defined(PNG_READ_PACKSWAP_SUPPORTED)
            if (png_ptr->transformations & PNG_PACKSWAP)
            {
               s_start = 0;
               s_end = 7;
               s_inc = 1;
            }
            else
#endif
            {
               s_start = 7;
               s_end = 0;
               s_inc = -1;
            }

            shift = s_start;

            for (i = 0; i < png_ptr->width; i++)
            {
               if (m & mask)
               {
                  int value;

                  value = (*sp >> shift) & 0x1;
                  *dp &= (png_byte)((0x7f7f >> (7 - shift)) & 0xff);
                  *dp |= (png_byte)(value << shift);
               }

               if (shift == s_end)
               {
                  shift = s_start;
                  sp++;
                  dp++;
               }
               else
                  shift += s_inc;

               if (m == 1)
                  m = 0x80;
               else
                  m >>= 1;
            }
            break;
         }       

         case 2:        
         {
            png_bytep sp;
            png_bytep dp;
            int s_start, s_end, s_inc;
            int m;
            int shift;
            png_uint_32 i;
            int value;

            sp = png_ptr->row_buf + 1;
            dp = row;
            m = 0x80;
#if defined(PNG_READ_PACKSWAP_SUPPORTED)
            if (png_ptr->transformations & PNG_PACKSWAP)
            {
               s_start = 0;
               s_end = 6;
               s_inc = 2;
            }
            else
#endif
            {
               s_start = 6;
               s_end = 0;
               s_inc = -2;
            }

            shift = s_start;

            for (i = 0; i < png_ptr->width; i++)
            {
               if (m & mask)
               {
                  value = (*sp >> shift) & 0x3;
                  *dp &= (png_byte)((0x3f3f >> (6 - shift)) & 0xff);
                  *dp |= (png_byte)(value << shift);
               }

               if (shift == s_end)
               {
                  shift = s_start;
                  sp++;
                  dp++;
               }
               else
                  shift += s_inc;
               if (m == 1)
                  m = 0x80;
               else
                  m >>= 1;
            }
            break;
         }       

         case 4:        
         {
            png_bytep sp;
            png_bytep dp;
            int s_start, s_end, s_inc;
            int m;
            int shift;
            png_uint_32 i;
            int value;

            sp = png_ptr->row_buf + 1;
            dp = row;
            m = 0x80;
#if defined(PNG_READ_PACKSWAP_SUPPORTED)
            if (png_ptr->transformations & PNG_PACKSWAP)
            {
               s_start = 0;
               s_end = 4;
               s_inc = 4;
            }
            else
#endif
            {
               s_start = 4;
               s_end = 0;
               s_inc = -4;
            }

            shift = s_start;

            for (i = 0; i < png_ptr->width; i++)
            {
               if (m & mask)
               {
                  value = (*sp >> shift) & 0xf;
                  *dp &= (png_byte)((0xf0f >> (4 - shift)) & 0xff);
                  *dp |= (png_byte)(value << shift);
               }

               if (shift == s_end)
               {
                  shift = s_start;
                  sp++;
                  dp++;
               }
               else
                  shift += s_inc;
               if (m == 1)
                  m = 0x80;
               else
                  m >>= 1;
            }
            break;
         }       

         case 16:       
         {
            png_bytep srcptr;
            png_bytep dstptr;

#if !defined(PNG_1_0_X)
            if (png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_COMBINE_ROW)
#else
            if (_mmx_supported)
#endif
            {
               png_uint_32 len;
               int diff;

               srcptr = png_ptr->row_buf + 1;
               dstptr = row;
               len  = png_ptr->width & ~7;          
               diff = (int) (png_ptr->width & 7);   

               __asm__ __volatile__ (
                  "not       %%edx            \n\t" 
                  "movd      %%edx, %%mm7     \n\t" 
                  "not       %%edx            \n\t" 
                  "psubb     %%mm6, %%mm6     \n\t" 
                  "punpcklbw %%mm7, %%mm7     \n\t"
                  "punpcklwd %%mm7, %%mm7     \n\t"
                  "punpckldq %%mm7, %%mm7     \n\t" 

                  LOAD_GOT_rbp
                  "movq   " MASK16_0 ", %%mm0 \n\t" 
                  "movq   " MASK16_1 ", %%mm1 \n\t" 
                  RESTORE_rbp

                  "pand      %%mm7, %%mm0     \n\t"
                  "pand      %%mm7, %%mm1     \n\t"

                  "pcmpeqb   %%mm6, %%mm0     \n\t"
                  "pcmpeqb   %%mm6, %%mm1     \n\t"


                  "cmpl      $0, %%ecx        \n\t"
                  "jz        mainloop16end    \n\t"

                "mainloop16:                  \n\t"
                  "movq      (%3), %%mm4      \n\t"
                  "pand      %%mm0, %%mm4     \n\t"
                  "movq      %%mm0, %%mm6     \n\t"
                  "movq      (%4), %%mm7      \n\t"
                  "pandn     %%mm7, %%mm6     \n\t"
                  "por       %%mm6, %%mm4     \n\t"
                  "movq      %%mm4, (%4)      \n\t"

                  "movq      8(%3), %%mm5     \n\t"
                  "pand      %%mm1, %%mm5     \n\t"
                  "movq      %%mm1, %%mm7     \n\t"
                  "movq      8(%4), %%mm6     \n\t"
                  "pandn     %%mm6, %%mm7     \n\t"
                  "por       %%mm7, %%mm5     \n\t"
                  "movq      %%mm5, 8(%4)     \n\t"

                  "add       $16, %3          \n\t" 
                  "add       $16, %4          \n\t"
                  "subl      $8, %%ecx        \n\t" 
                  "ja        mainloop16       \n\t"

                "mainloop16end:               \n\t"
                  "movl      %%eax, %%ecx     \n\t"
                  "cmpl      $0, %%ecx        \n\t"
                  "jz        end16            \n\t"
                  "sall      $24, %%edx       \n\t" 

                "secondloop16:                \n\t"
                  "sall      %%edx            \n\t" 
                  "jnc       skip16           \n\t" 
                  "movw      (%3), %%ax       \n\t"
                  "movw      %%ax, (%4)       \n\t"

                "skip16:                      \n\t"
                  "add       $2, %3           \n\t"
                  "add       $2, %4           \n\t"
                  "decl      %%ecx            \n\t"
                  "jnz       secondloop16     \n\t"

                "end16:                       \n\t"
                  "EMMS                       \n\t" 

                  : "=a" (dummy_value_a),           
                    "=d" (dummy_value_d),
                    "=c" (dummy_value_c),
                    "=S" (dummy_value_S),
                    "=D" (dummy_value_D)

                  : "0" (diff),        
                    "1" (mask),        
                    "2" (len),         
                    "3" (srcptr),      
                    "4" (dstptr)       

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                  : "%mm0", "%mm1", "%mm4"          
                  , "%mm5", "%mm6", "%mm7"
#endif
               );
            }
            else 
            {
               register png_uint_32 i;
               png_uint_32 initial_val = BPP2 * png_pass_start[png_ptr->pass];
                 
               register int stride = BPP2 * png_pass_inc[png_ptr->pass];
                 
               register int rep_bytes = BPP2 * png_pass_width[png_ptr->pass];
                 
               png_uint_32 len = png_ptr->width &~7;  
               int diff = (int) (png_ptr->width & 7); 
               register png_uint_32 final_val = BPP2 * len;   

               srcptr = png_ptr->row_buf + 1 + initial_val;
               dstptr = row + initial_val;

               for (i = initial_val; i < final_val; i += stride)
               {
                  png_memcpy(dstptr, srcptr, rep_bytes);
                  srcptr += stride;
                  dstptr += stride;
               }
               if (diff)  
               {
                  final_val += diff*BPP2;
                  for (; i < final_val; i += stride)
                  {
                     if (rep_bytes > (int)(final_val-i))
                        rep_bytes = (int)(final_val-i);
                     png_memcpy(dstptr, srcptr, rep_bytes);
                     srcptr += stride;
                     dstptr += stride;
                  }
               }
            } 

            break;
         }       

         case 48:       
         {
            png_bytep srcptr;
            png_bytep dstptr;

#if !defined(PNG_1_0_X)
            if (png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_COMBINE_ROW)
#else
            if (_mmx_supported)
#endif
            {
               png_uint_32 len;
               int diff;

               srcptr = png_ptr->row_buf + 1;
               dstptr = row;
               len  = png_ptr->width & ~7;          
               diff = (int) (png_ptr->width & 7);   

               __asm__ __volatile__ (
                  "not       %%edx            \n\t" 
                  "movd      %%edx, %%mm7     \n\t" 
                  "not       %%edx            \n\t" 
                  "psubb     %%mm6, %%mm6     \n\t" 
                  "punpcklbw %%mm7, %%mm7     \n\t"
                  "punpcklwd %%mm7, %%mm7     \n\t"
                  "punpckldq %%mm7, %%mm7     \n\t" 

                  LOAD_GOT_rbp
                  "movq   " MASK48_0 ", %%mm0 \n\t" 
                  "movq   " MASK48_1 ", %%mm1 \n\t" 
                  "movq   " MASK48_2 ", %%mm2 \n\t" 
                  "movq   " MASK48_3 ", %%mm3 \n\t" 
                  "movq   " MASK48_4 ", %%mm4 \n\t" 
                  "movq   " MASK48_5 ", %%mm5 \n\t" 
                  RESTORE_rbp

                  "pand      %%mm7, %%mm0     \n\t"
                  "pand      %%mm7, %%mm1     \n\t"
                  "pand      %%mm7, %%mm2     \n\t"
                  "pand      %%mm7, %%mm3     \n\t"
                  "pand      %%mm7, %%mm4     \n\t"
                  "pand      %%mm7, %%mm5     \n\t"

                  "pcmpeqb   %%mm6, %%mm0     \n\t"
                  "pcmpeqb   %%mm6, %%mm1     \n\t"
                  "pcmpeqb   %%mm6, %%mm2     \n\t"
                  "pcmpeqb   %%mm6, %%mm3     \n\t"
                  "pcmpeqb   %%mm6, %%mm4     \n\t"
                  "pcmpeqb   %%mm6, %%mm5     \n\t"


                  "cmpl      $0, %%ecx        \n\t"
                  "jz        mainloop48end    \n\t"

                "mainloop48:                  \n\t"
                  "movq      (%3), %%mm7      \n\t"
                  "pand      %%mm0, %%mm7     \n\t"
                  "movq      %%mm0, %%mm6     \n\t"
                  "pandn     (%4), %%mm6      \n\t"
                  "por       %%mm6, %%mm7     \n\t"
                  "movq      %%mm7, (%4)      \n\t"

                  "movq      8(%3), %%mm6     \n\t"
                  "pand      %%mm1, %%mm6     \n\t"
                  "movq      %%mm1, %%mm7     \n\t"
                  "pandn     8(%4), %%mm7     \n\t"
                  "por       %%mm7, %%mm6     \n\t"
                  "movq      %%mm6, 8(%4)     \n\t"

                  "movq      16(%3), %%mm6    \n\t"
                  "pand      %%mm2, %%mm6     \n\t"
                  "movq      %%mm2, %%mm7     \n\t"
                  "pandn     16(%4), %%mm7    \n\t"
                  "por       %%mm7, %%mm6     \n\t"
                  "movq      %%mm6, 16(%4)    \n\t"

                  "movq      24(%3), %%mm7    \n\t"
                  "pand      %%mm3, %%mm7     \n\t"
                  "movq      %%mm3, %%mm6     \n\t"
                  "pandn     24(%4), %%mm6    \n\t"
                  "por       %%mm6, %%mm7     \n\t"
                  "movq      %%mm7, 24(%4)    \n\t"

                  "movq      32(%3), %%mm6    \n\t"
                  "pand      %%mm4, %%mm6     \n\t"
                  "movq      %%mm4, %%mm7     \n\t"
                  "pandn     32(%4), %%mm7    \n\t"
                  "por       %%mm7, %%mm6     \n\t"
                  "movq      %%mm6, 32(%4)    \n\t"

                  "movq      40(%3), %%mm7    \n\t"
                  "pand      %%mm5, %%mm7     \n\t"
                  "movq      %%mm5, %%mm6     \n\t"
                  "pandn     40(%4), %%mm6    \n\t"
                  "por       %%mm6, %%mm7     \n\t"
                  "movq      %%mm7, 40(%4)    \n\t"

                  "add       $48, %3          \n\t" 
                  "add       $48, %4          \n\t"
                  "subl      $8, %%ecx        \n\t" 

                  "ja        mainloop48       \n\t"

                "mainloop48end:               \n\t"
                  "movl      %%eax, %%ecx     \n\t"
                  "cmpl      $0, %%ecx        \n\t"
                  "jz        end48            \n\t"
                  "sall      $24, %%edx       \n\t" 

                "secondloop48:                \n\t"
                  "sall      %%edx            \n\t" 
                  "jnc       skip48           \n\t" 
                  "movl      (%3), %%eax      \n\t"
                  "movl      %%eax, (%4)      \n\t"
                  "movw      4(%3), %%ax      \n\t" 
                  "movw      %%ax, 4(%4)      \n\t" 

                "skip48:                      \n\t"
                  "add       $6, %3           \n\t" 
                  "add       $6, %4           \n\t" 
                  "decl      %%ecx            \n\t"
                  "jnz       secondloop48     \n\t"

                "end48:                       \n\t"
                  "EMMS                       \n\t" 

                  : "=a" (dummy_value_a),           
                    "=d" (dummy_value_d),
                    "=c" (dummy_value_c),
                    "=S" (dummy_value_S),
                    "=D" (dummy_value_D)

                  : "0" (diff),        
                    "1" (mask),        
                    "2" (len),         
                    "3" (srcptr),      
                    "4" (dstptr)       

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                  : "%mm0", "%mm1", "%mm2", "%mm3"  
                  , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
               );
            }
            else 
            {
               register png_uint_32 i;
               png_uint_32 initial_val = BPP6 * png_pass_start[png_ptr->pass];
                 
               register int stride = BPP6 * png_pass_inc[png_ptr->pass];
                 
               register int rep_bytes = BPP6 * png_pass_width[png_ptr->pass];
                 
               png_uint_32 len = png_ptr->width &~7;  
               int diff = (int) (png_ptr->width & 7); 
               register png_uint_32 final_val = BPP6 * len;   

               srcptr = png_ptr->row_buf + 1 + initial_val;
               dstptr = row + initial_val;

               for (i = initial_val; i < final_val; i += stride)
               {
                  png_memcpy(dstptr, srcptr, rep_bytes);
                  srcptr += stride;
                  dstptr += stride;
               }
               if (diff)  
               {
                  final_val += diff*BPP6;
                  for (; i < final_val; i += stride)
                  {
                     if (rep_bytes > (int)(final_val-i))
                        rep_bytes = (int)(final_val-i);
                     png_memcpy(dstptr, srcptr, rep_bytes);
                     srcptr += stride;
                     dstptr += stride;
                  }
               }
            } 

            break;
         }       

         case 64:       
         {
            png_bytep srcptr;
            png_bytep dstptr;
            register png_uint_32 i;
            png_uint_32 initial_val = BPP8 * png_pass_start[png_ptr->pass];
              
            register int stride = BPP8 * png_pass_inc[png_ptr->pass];
              
            register int rep_bytes = BPP8 * png_pass_width[png_ptr->pass];
              
            png_uint_32 len = png_ptr->width &~7;  
            int diff = (int) (png_ptr->width & 7); 
            register png_uint_32 final_val = BPP8 * len;   

            srcptr = png_ptr->row_buf + 1 + initial_val;
            dstptr = row + initial_val;

            for (i = initial_val; i < final_val; i += stride)
            {
               png_memcpy(dstptr, srcptr, rep_bytes);
               srcptr += stride;
               dstptr += stride;
            }
            if (diff)  
            {
               final_val += diff*BPP8;
               for (; i < final_val; i += stride)
               {
                  if (rep_bytes > (int)(final_val-i))
                     rep_bytes = (int)(final_val-i);
                  png_memcpy(dstptr, srcptr, rep_bytes);
                  srcptr += stride;
                  dstptr += stride;
               }
            }

            break;
         }       

         default: 
         {
            
#if defined(PNG_DEBUG)
            png_debug(1, "Internal libpng logic error (GCC "
              "png_combine_row() pixel_depth)\n");
#endif
            break;
         }
      } 

   } 

} 

#endif 





#if defined(PNG_READ_INTERLACING_SUPPORTED)
#if defined(PNG_HAVE_MMX_READ_INTERLACE)


void 
png_do_read_interlace(png_structp png_ptr)
{
   png_row_infop row_info = &(png_ptr->row_info);
   png_bytep row = png_ptr->row_buf + 1;
   int pass = png_ptr->pass;
#if defined(PNG_READ_PACKSWAP_SUPPORTED)
   png_uint_32 transformations = png_ptr->transformations;
#endif

   png_debug(1, "in png_do_read_interlace (pnggccrd.c)\n");

   if (_mmx_supported == 2) {
#if !defined(PNG_1_0_X)
       
       png_warning(png_ptr, "asm_flags may not have been initialized");
#endif
       png_mmx_support();
   }

   if (row != NULL && row_info != NULL)
   {
      png_uint_32 final_width;

      final_width = row_info->width * png_pass_inc[pass];

      switch (row_info->pixel_depth)
      {
         case 1:
         {
            png_bytep sp, dp;
            int sshift, dshift;
            int s_start, s_end, s_inc;
            png_byte v;
            png_uint_32 i;
            int j;

            sp = row + (png_size_t)((row_info->width - 1) >> 3);
            dp = row + (png_size_t)((final_width - 1) >> 3);
#if defined(PNG_READ_PACKSWAP_SUPPORTED)
            if (transformations & PNG_PACKSWAP)
            {
               sshift = (int)((row_info->width + 7) & 7);
               dshift = (int)((final_width + 7) & 7);
               s_start = 7;
               s_end = 0;
               s_inc = -1;
            }
            else
#endif
            {
               sshift = 7 - (int)((row_info->width + 7) & 7);
               dshift = 7 - (int)((final_width + 7) & 7);
               s_start = 0;
               s_end = 7;
               s_inc = 1;
            }

            for (i = row_info->width; i; i--)
            {
               v = (png_byte)((*sp >> sshift) & 0x1);
               for (j = 0; j < png_pass_inc[pass]; j++)
               {
                  *dp &= (png_byte)((0x7f7f >> (7 - dshift)) & 0xff);
                  *dp |= (png_byte)(v << dshift);
                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }
                  else
                     dshift += s_inc;
               }
               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }
               else
                  sshift += s_inc;
            }
            break;
         }

         case 2:
         {
            png_bytep sp, dp;
            int sshift, dshift;
            int s_start, s_end, s_inc;
            png_uint_32 i;

            sp = row + (png_size_t)((row_info->width - 1) >> 2);
            dp = row + (png_size_t)((final_width - 1) >> 2);
#if defined(PNG_READ_PACKSWAP_SUPPORTED)
            if (transformations & PNG_PACKSWAP)
            {
               sshift = (png_size_t)(((row_info->width + 3) & 3) << 1);
               dshift = (png_size_t)(((final_width + 3) & 3) << 1);
               s_start = 6;
               s_end = 0;
               s_inc = -2;
            }
            else
#endif
            {
               sshift = (png_size_t)((3 - ((row_info->width + 3) & 3)) << 1);
               dshift = (png_size_t)((3 - ((final_width + 3) & 3)) << 1);
               s_start = 0;
               s_end = 6;
               s_inc = 2;
            }

            for (i = row_info->width; i; i--)
            {
               png_byte v;
               int j;

               v = (png_byte)((*sp >> sshift) & 0x3);
               for (j = 0; j < png_pass_inc[pass]; j++)
               {
                  *dp &= (png_byte)((0x3f3f >> (6 - dshift)) & 0xff);
                  *dp |= (png_byte)(v << dshift);
                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }
                  else
                     dshift += s_inc;
               }
               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }
               else
                  sshift += s_inc;
            }
            break;
         }

         case 4:
         {
            png_bytep sp, dp;
            int sshift, dshift;
            int s_start, s_end, s_inc;
            png_uint_32 i;

            sp = row + (png_size_t)((row_info->width - 1) >> 1);
            dp = row + (png_size_t)((final_width - 1) >> 1);
#if defined(PNG_READ_PACKSWAP_SUPPORTED)
            if (transformations & PNG_PACKSWAP)
            {
               sshift = (png_size_t)(((row_info->width + 1) & 1) << 2);
               dshift = (png_size_t)(((final_width + 1) & 1) << 2);
               s_start = 4;
               s_end = 0;
               s_inc = -4;
            }
            else
#endif
            {
               sshift = (png_size_t)((1 - ((row_info->width + 1) & 1)) << 2);
               dshift = (png_size_t)((1 - ((final_width + 1) & 1)) << 2);
               s_start = 0;
               s_end = 4;
               s_inc = 4;
            }

            for (i = row_info->width; i; i--)
            {
               png_byte v;
               int j;

               v = (png_byte)((*sp >> sshift) & 0xf);
               for (j = 0; j < png_pass_inc[pass]; j++)
               {
                  *dp &= (png_byte)((0xf0f >> (4 - dshift)) & 0xff);
                  *dp |= (png_byte)(v << dshift);
                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }
                  else
                     dshift += s_inc;
               }
               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }
               else
                  sshift += s_inc;
            }
            break;
         }

       

         default: 
         {
            png_bytep sptr, dp;
            png_uint_32 i;
            png_size_t pixel_bytes;
            int width = (int)row_info->width;

            pixel_bytes = (row_info->pixel_depth >> 3);

            
            sptr = row + (width - 1) * pixel_bytes;

            
            dp = row + (final_width - 1) * pixel_bytes;

            

#if !defined(PNG_1_0_X)
            if (png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_INTERLACE)
#else
            if (_mmx_supported)
#endif
            {
               int dummy_value_c;        
               png_bytep dummy_value_S;
               png_bytep dummy_value_D;
               png_bytep dummy_value_a;
               png_bytep dummy_value_d;

               
               if (pixel_bytes == BPP3)
               {
                  if (((pass == 4) || (pass == 5)) && width)
                  {
                     int width_mmx = ((width >> 1) << 1) - 8;   
                     if (width_mmx < 0)
                         width_mmx = 0;
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        
                        
                        
                        __asm__ __volatile__ (
                           "sub  $3, %1             \n\t"
                           "sub  $9, %2             \n\t"
                                        

                        ".loop3_pass4:              \n\t"
                           "movq (%1), %%mm0        \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "movq %%mm0, %%mm2       \n\t" 
                           "psllq $24, %%mm0        \n\t" 
                           "pand (%3), %%mm1        \n\t" 
                           "psrlq $24, %%mm2        \n\t" 
                           "por %%mm1, %%mm0        \n\t" 
                           "movq %%mm2, %%mm3       \n\t" 
                           "psllq $8, %%mm2         \n\t" 
                           "movq %%mm0, (%2)        \n\t"
                           "psrlq $16, %%mm3        \n\t" 
                           "pand (%4), %%mm3        \n\t" 
                           "por %%mm3, %%mm2        \n\t" 
                           "sub  $6, %1             \n\t"
                           "movd %%mm2, 8(%2)       \n\t"
                           "sub  $12, %2            \n\t"
                           "subl $2, %%ecx          \n\t"
                           "jnz .loop3_pass4        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D),
                             "=a" (dummy_value_a),
                             "=d" (dummy_value_d)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp),            
#if defined(PNG_x86_64_USE_GOTPCREL)     
                             "3" (&_c64._amask5_3_0), 
                             "4" (&_c64._amask7_1_0)  
#else
                             "3" (&_amask5_3_0),  
                             "4" (&_amask7_1_0)   
#endif

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1"               
                           , "%mm2", "%mm3"
#endif
                        );
                     }

                     sptr -= width_mmx*BPP3;
                     dp -= width_mmx*2*BPP3;
                     for (i = width; i; i--)
                     {
                        png_byte v[8];
                        int j;

                        png_memcpy(v, sptr, BPP3);
                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           png_memcpy(dp, v, BPP3);
                           dp -= BPP3;
                        }
                        sptr -= BPP3;
                     }
                  }
                  else if (((pass == 2) || (pass == 3)) && width)
                  {
                     __asm__ __volatile__ (
                        "sub  $9, %2             \n\t"
                                     

                     ".loop3_pass2:              \n\t"
                        "movd (%1), %%mm0        \n\t" 
                        "pand (%3), %%mm0        \n\t" 
                        "movq %%mm0, %%mm1       \n\t" 
                        "psllq $16, %%mm0        \n\t" 
                        "movq %%mm0, %%mm2       \n\t" 
                        "psllq $24, %%mm0        \n\t" 
                        "psrlq $8, %%mm1         \n\t" 
                        "por %%mm2, %%mm0        \n\t" 
                        "por %%mm1, %%mm0        \n\t" 
                        "movq %%mm0, 4(%2)       \n\t"
                        "psrlq $16, %%mm0        \n\t" 
                        "sub  $3, %1             \n\t"
                        "movd %%mm0, (%2)        \n\t"
                        "sub  $12, %2            \n\t"
                        "decl %%ecx              \n\t"
                        "jnz .loop3_pass2        \n\t"
                        "EMMS                    \n\t" 

                        : "=c" (dummy_value_c),        
                          "=S" (dummy_value_S),
                          "=D" (dummy_value_D),
                          "=a" (dummy_value_a)

                        : "0" (width),         
                          "1" (sptr),          
                          "2" (dp),            
#if defined(PNG_x86_64_USE_GOTPCREL)           
                          "3" (&_c64._amask5_3_0)  
#else
                          "3" (&_amask5_3_0)   
#endif

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
                        : "%mm0", "%mm1", "%mm2"       
#endif
                     );
                  }
                  else if (width)  
                  {
                     __asm__ __volatile__ (
                        "sub  $21, %2            \n\t"
                                     

                     ".loop3_pass0:              \n\t"
                        "movd (%1), %%mm0        \n\t" 
                        "pand (%3), %%mm0        \n\t" 
                        "movq %%mm0, %%mm1       \n\t" 
                        "psllq $16, %%mm0        \n\t" 
                        "movq %%mm0, %%mm2       \n\t" 
                        "psllq $24, %%mm0        \n\t" 
                        "psrlq $8, %%mm1         \n\t" 
                        "por %%mm2, %%mm0        \n\t" 
                        "por %%mm1, %%mm0        \n\t" 
                        "movq %%mm0, %%mm3       \n\t" 
                        "psllq $16, %%mm0        \n\t" 
                        "movq %%mm3, %%mm4       \n\t" 
                        "punpckhdq %%mm0, %%mm3  \n\t" 
                        "movq %%mm4, 16(%2)      \n\t"
                        "psrlq $32, %%mm0        \n\t" 
                        "movq %%mm3, 8(%2)       \n\t"
                        "punpckldq %%mm4, %%mm0  \n\t" 
                        "sub  $3, %1             \n\t"
                        "movq %%mm0, (%2)        \n\t"
                        "sub  $24, %2            \n\t"
                        "decl %%ecx              \n\t"
                        "jnz .loop3_pass0        \n\t"
                        "EMMS                    \n\t" 

                        : "=c" (dummy_value_c),        
                          "=S" (dummy_value_S),
                          "=D" (dummy_value_D),
                          "=a" (dummy_value_a)

                        : "0" (width),         
                          "1" (sptr),          
                          "2" (dp),            
#if defined(PNG_x86_64_USE_GOTPCREL)           
                          "3" (&_c64._amask5_3_0)  
#else
                          "3" (&_amask5_3_0)   
#endif

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                        : "%mm0", "%mm1", "%mm2"       
                        , "%mm3", "%mm4"
#endif
                     );
                  }
               } 

               
               else if (pixel_bytes == BPP4)
               {
                  if (((pass == 4) || (pass == 5)) && width)
                  {
                     int width_mmx = ((width >> 1) << 1) ;
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $4, %1             \n\t"
                           "sub  $12, %2            \n\t"

                        ".loop4_pass4:              \n\t"
                           "movq (%1), %%mm0        \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "punpckldq %%mm0, %%mm0  \n\t" 
                           "punpckhdq %%mm1, %%mm1  \n\t" 
                           "movq %%mm0, (%2)        \n\t"
                           "sub  $8, %1             \n\t"
                           "movq %%mm1, 8(%2)       \n\t"
                           "sub  $16, %2            \n\t"
                           "subl $2, %%ecx          \n\t"
                           "jnz .loop4_pass4        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1"               
#endif
                        );
                     }

                     sptr -= (width_mmx*BPP4 - BPP4); 
                     dp -= (width_mmx*2*BPP4 - BPP4); 
                     for (i = width; i; i--)
                     {
                        png_byte v[8];
                        int j;
                        sptr -= BPP4;
                        png_memcpy(v, sptr, BPP4);
                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           dp -= BPP4;
                           png_memcpy(dp, v, BPP4);
                        }
                     }
                  }
                  else if (((pass == 2) || (pass == 3)) && width)
                  {
                     int width_mmx = ((width >> 1) << 1);
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $4, %1             \n\t"
                           "sub  $28, %2            \n\t"

                        ".loop4_pass2:              \n\t"
                           "movq (%1), %%mm0        \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "punpckldq %%mm0, %%mm0  \n\t" 
                           "punpckhdq %%mm1, %%mm1  \n\t" 
                           "movq %%mm0, (%2)        \n\t"
                           "movq %%mm0, 8(%2)       \n\t"
                           "movq %%mm1, 16(%2)      \n\t"
                           "movq %%mm1, 24(%2)      \n\t"
                           "sub  $8, %1             \n\t"
                           "sub  $32, %2            \n\t"
                           "subl $2, %%ecx          \n\t"
                           "jnz .loop4_pass2        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1"               
#endif
                        );
                     }

                     sptr -= (width_mmx*4 - 4); 
                     dp -= (width_mmx*16 - 4);  
                     for (i = width; i; i--)
                     {
                        png_byte v[8];
                        int j;
                        sptr -= 4;
                        png_memcpy(v, sptr, 4);
                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           dp -= 4;
                           png_memcpy(dp, v, 4);
                        }
                     }
                  }
                  else if (width)  
                  {
                     int width_mmx = ((width >> 1) << 1);
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $4, %1             \n\t"
                           "sub  $60, %2            \n\t"

                        ".loop4_pass0:              \n\t"
                           "movq (%1), %%mm0        \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "punpckldq %%mm0, %%mm0  \n\t" 
                           "punpckhdq %%mm1, %%mm1  \n\t" 
                           "movq %%mm0, (%2)        \n\t"
                           "movq %%mm0, 8(%2)       \n\t"
                           "movq %%mm0, 16(%2)      \n\t"
                           "movq %%mm0, 24(%2)      \n\t"
                           "movq %%mm1, 32(%2)      \n\t"
                           "movq %%mm1, 40(%2)      \n\t"
                           "movq %%mm1, 48(%2)      \n\t"
                           "sub  $8, %1             \n\t"
                           "movq %%mm1, 56(%2)      \n\t"
                           "sub  $64, %2            \n\t"
                           "subl $2, %%ecx          \n\t"
                           "jnz .loop4_pass0        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1"               
#endif
                        );
                     }

                     sptr -= (width_mmx*4 - 4); 
                     dp -= (width_mmx*32 - 4);  
                     for (i = width; i; i--)
                     {
                        png_byte v[8];
                        int j;
                        sptr -= 4;
                        png_memcpy(v, sptr, 4);
                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           dp -= 4;
                           png_memcpy(dp, v, 4);
                        }
                     }
                  }
               } 

               
               else if (pixel_bytes == 1)
               {
                  if (((pass == 4) || (pass == 5)) && width)
                  {
                     int width_mmx = ((width >> 3) << 3);
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $7, %1             \n\t"
                           "sub  $15, %2            \n\t"

                        ".loop1_pass4:              \n\t"
                           "movq (%1), %%mm0        \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "punpcklbw %%mm0, %%mm0  \n\t" 
                           "punpckhbw %%mm1, %%mm1  \n\t" 
                           "movq %%mm1, 8(%2)       \n\t"
                           "sub  $8, %1             \n\t"
                           "movq %%mm0, (%2)        \n\t"
                           "sub  $16, %2            \n\t"
                           "subl $8, %%ecx          \n\t"
                           "jnz .loop1_pass4        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1"               
#endif
                        );
                     }

                     sptr -= width_mmx;
                     dp -= width_mmx*2;
                     for (i = width; i; i--)
                     {
                        int j;

                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           *dp-- = *sptr;
                        }
                        --sptr;
                     }
                  }
                  else if (((pass == 2) || (pass == 3)) && width)
                  {
                     int width_mmx = ((width >> 2) << 2);
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $3, %1             \n\t"
                           "sub  $15, %2            \n\t"

                        ".loop1_pass2:              \n\t"
                           "movd (%1), %%mm0        \n\t" 
                           "punpcklbw %%mm0, %%mm0  \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "punpcklwd %%mm0, %%mm0  \n\t" 
                           "punpckhwd %%mm1, %%mm1  \n\t" 
                           "movq %%mm0, (%2)        \n\t"
                           "sub  $4, %1             \n\t"
                           "movq %%mm1, 8(%2)       \n\t"
                           "sub  $16, %2            \n\t"
                           "subl $4, %%ecx          \n\t"
                           "jnz .loop1_pass2        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1"               
#endif
                        );
                     }

                     sptr -= width_mmx;
                     dp -= width_mmx*4;
                     for (i = width; i; i--)
                     {
                        int j;

                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           *dp-- = *sptr;
                        }
                        --sptr;
                     }
                  }
                  else if (width)  
                  {
                     int width_mmx = ((width >> 2) << 2);
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $3, %1             \n\t"
                           "sub  $31, %2            \n\t"

                        ".loop1_pass0:              \n\t"
                           "movd (%1), %%mm0        \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "punpcklbw %%mm0, %%mm0  \n\t" 
                           "movq %%mm0, %%mm2       \n\t" 
                           "punpcklwd %%mm0, %%mm0  \n\t" 
                           "movq %%mm0, %%mm3       \n\t" 
                           "punpckldq %%mm0, %%mm0  \n\t" 
                           "punpckhdq %%mm3, %%mm3  \n\t" 
                           "movq %%mm0, (%2)        \n\t"
                           "punpckhwd %%mm2, %%mm2  \n\t" 
                           "movq %%mm3, 8(%2)       \n\t"
                           "movq %%mm2, %%mm4       \n\t" 
                           "punpckldq %%mm2, %%mm2  \n\t" 
                           "punpckhdq %%mm4, %%mm4  \n\t" 
                           "movq %%mm2, 16(%2)      \n\t"
                           "sub  $4, %1             \n\t"
                           "movq %%mm4, 24(%2)      \n\t"
                           "sub  $32, %2            \n\t"
                           "subl $4, %%ecx          \n\t"
                           "jnz .loop1_pass0        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1", "%mm2"       
                           , "%mm3", "%mm4"
#endif
                        );
                     }

                     sptr -= width_mmx;
                     dp -= width_mmx*8;
                     for (i = width; i; i--)
                     {
                        int j;


                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           *dp-- = *sptr;
                        }
                        --sptr;
                     }
                  }
               } 

               
               else if (pixel_bytes == BPP2)
               {
                  if (((pass == 4) || (pass == 5)) && width)
                  {
                     int width_mmx = ((width >> 1) << 1) ;
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $2, %1             \n\t"
                           "sub  $6, %2             \n\t"

                        ".loop2_pass4:              \n\t"
                           "movd (%1), %%mm0        \n\t" 
                           "punpcklwd %%mm0, %%mm0  \n\t" 
                           "sub  $4, %1             \n\t"
                           "movq %%mm0, (%2)        \n\t"
                           "sub  $8, %2             \n\t"
                           "subl $2, %%ecx          \n\t"
                           "jnz .loop2_pass4        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0"                       
#endif
                        );
                     }

                     sptr -= (width_mmx*BPP2 - BPP2); 
                     dp -= (width_mmx*2*BPP2 - BPP2); 
                     for (i = width; i; i--)
                     {
                        png_byte v[8];
                        int j;
                        sptr -= BPP2;
                        png_memcpy(v, sptr, BPP2);
                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           dp -= BPP2;
                           png_memcpy(dp, v, BPP2);
                        }
                     }
                  }
                  else if (((pass == 2) || (pass == 3)) && width)
                  {
                     int width_mmx = ((width >> 1) << 1) ;
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $2, %1             \n\t"
                           "sub  $14, %2            \n\t"

                        ".loop2_pass2:              \n\t"
                           "movd (%1), %%mm0        \n\t" 
                           "punpcklwd %%mm0, %%mm0  \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "punpckldq %%mm0, %%mm0  \n\t" 
                           "punpckhdq %%mm1, %%mm1  \n\t" 
                           "movq %%mm0, (%2)        \n\t"
                           "sub  $4, %1             \n\t"
                           "movq %%mm1, 8(%2)       \n\t"
                           "sub  $16, %2            \n\t"
                           "subl $2, %%ecx          \n\t"
                           "jnz .loop2_pass2        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1"               
#endif
                        );
                     }

                     sptr -= (width_mmx*2 - 2); 
                     dp -= (width_mmx*8 - 2);   
                     for (i = width; i; i--)
                     {
                        png_byte v[8];
                        int j;
                        sptr -= 2;
                        png_memcpy(v, sptr, 2);
                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           dp -= 2;
                           png_memcpy(dp, v, 2);
                        }
                     }
                  }
                  else if (width)  
                  {
                     int width_mmx = ((width >> 1) << 1);
                     width -= width_mmx;        
                     if (width_mmx)
                     {
                        __asm__ __volatile__ (
                           "sub  $2, %1             \n\t"
                           "sub  $30, %2            \n\t"

                        ".loop2_pass0:              \n\t"
                           "movd (%1), %%mm0        \n\t" 
                           "punpcklwd %%mm0, %%mm0  \n\t" 
                           "movq %%mm0, %%mm1       \n\t" 
                           "punpckldq %%mm0, %%mm0  \n\t" 
                           "punpckhdq %%mm1, %%mm1  \n\t" 
                           "movq %%mm0, (%2)        \n\t"
                           "movq %%mm0, 8(%2)       \n\t"
                           "movq %%mm1, 16(%2)      \n\t"
                           "sub  $4, %1             \n\t"
                           "movq %%mm1, 24(%2)      \n\t"
                           "sub  $32, %2            \n\t"
                           "subl $2, %%ecx          \n\t"
                           "jnz .loop2_pass0        \n\t"
                           "EMMS                    \n\t" 

                           : "=c" (dummy_value_c),        
                             "=S" (dummy_value_S),
                             "=D" (dummy_value_D)

                           : "0" (width_mmx),     
                             "1" (sptr),          
                             "2" (dp)             

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
                           : "%mm0", "%mm1"               
#endif
                        );
                     }

                     sptr -= (width_mmx*2 - 2); 
                     dp -= (width_mmx*16 - 2);  
                     for (i = width; i; i--)
                     {
                        png_byte v[8];
                        int j;
                        sptr -= 2;
                        png_memcpy(v, sptr, 2);
                        for (j = 0; j < png_pass_inc[pass]; j++)
                        {
                           dp -= 2;
                           png_memcpy(dp, v, 2);
                        }
                     }
                  }
               } 

               
               else if (pixel_bytes == BPP8)
               {
                  
                  if (((pass == 4) || (pass == 5)) && width)
                  {
                     
                     
                     __asm__ __volatile__ (
                        "sub  $8, %2             \n\t" 

                     ".loop8_pass4:              \n\t"
                        "movq (%1), %%mm0        \n\t" 
                        "movq %%mm0, (%2)        \n\t"
                        "sub  $8, %1             \n\t"
                        "movq %%mm0, 8(%2)       \n\t"
                        "sub  $16, %2            \n\t"
                        "decl %%ecx              \n\t"
                        "jnz .loop8_pass4        \n\t"
                        "EMMS                    \n\t" 

                        : "=c" (dummy_value_c),        
                          "=S" (dummy_value_S),
                          "=D" (dummy_value_D)

                        : "0" (width),         
                          "1" (sptr),          
                          "2" (dp)             

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
                        : "%mm0"                       
#endif
                     );
                  }
                  else if (((pass == 2) || (pass == 3)) && width)
                  {
                     
                     
                     
                     
                     __asm__ __volatile__ (
                        "sub  $24, %2            \n\t" 

                     ".loop8_pass2:              \n\t"
                        "movq (%1), %%mm0        \n\t" 
                        "movq %%mm0, (%2)        \n\t"
                        "movq %%mm0, 8(%2)       \n\t"
                        "movq %%mm0, 16(%2)      \n\t"
                        "sub  $8, %1             \n\t"
                        "movq %%mm0, 24(%2)      \n\t"
                        "sub  $32, %2            \n\t"
                        "decl %%ecx              \n\t"
                        "jnz .loop8_pass2        \n\t"
                        "EMMS                    \n\t" 

                        : "=c" (dummy_value_c),        
                          "=S" (dummy_value_S),
                          "=D" (dummy_value_D)

                        : "0" (width),         
                          "1" (sptr),          
                          "2" (dp)             

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                        : "%mm0"                       
#endif
                     );
                  }
                  else if (width)  
                  {
                     
                     
                     __asm__ __volatile__ (
                        "sub  $56, %2            \n\t" 

                     ".loop8_pass0:              \n\t"
                        "movq (%1), %%mm0        \n\t" 
                        "movq %%mm0, (%2)        \n\t"
                        "movq %%mm0, 8(%2)       \n\t"
                        "movq %%mm0, 16(%2)      \n\t"
                        "movq %%mm0, 24(%2)      \n\t"
                        "movq %%mm0, 32(%2)      \n\t"
                        "movq %%mm0, 40(%2)      \n\t"
                        "movq %%mm0, 48(%2)      \n\t"
                        "sub  $8, %1             \n\t"
                        "movq %%mm0, 56(%2)      \n\t"
                        "sub  $64, %2            \n\t"
                        "decl %%ecx              \n\t"
                        "jnz .loop8_pass0        \n\t"
                        "EMMS                    \n\t" 

                        : "=c" (dummy_value_c),        
                          "=S" (dummy_value_S),
                          "=D" (dummy_value_D)

                        : "0" (width),         
                          "1" (sptr),          
                          "2" (dp)             

#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
                        : "%mm0"                       
#endif
                     );
                  }
               } 

               
               else if (pixel_bytes == BPP6)   
               {
                  for (i = width; i; i--)
                  {
                     png_byte v[8];
                     int j;
                     png_memcpy(v, sptr, BPP6);
                     for (j = 0; j < png_pass_inc[pass]; j++)
                     {
                        png_memcpy(dp, v, BPP6);
                        dp -= BPP6;
                     }
                     sptr -= BPP6;
                  }
               } 

               
               else
               {
                  
#if defined(PNG_DEBUG)
                  png_debug(1, "Internal libpng logic error (GCC "
                    "png_do_read_interlace() _mmx_supported)\n");
#endif
               }

            } 

            else 
            {
               if (pixel_bytes == BPP3)
               {
                  for (i = width; i; i--)
                  {
                     png_byte v[8];
                     int j;
                     png_memcpy(v, sptr, BPP3);
                     for (j = 0; j < png_pass_inc[pass]; j++)
                     {
                        png_memcpy(dp, v, BPP3);
                        dp -= BPP3;
                     }
                     sptr -= BPP3;
                  }
               }
               else if (pixel_bytes == BPP4)
               {
                  for (i = width; i; i--)
                  {
                     png_byte v[8];
                     int j;
                     png_memcpy(v, sptr, BPP4);
                     for (j = 0; j < png_pass_inc[pass]; j++)
                     {
#if defined(PNG_DEBUG) && defined(PNG_1_0_X)  
                        if (dp < row || dp+3 > row+png_ptr->row_buf_size)
                        {
                           printf("dp out of bounds: row=%10p, dp=%10p, "
                             "rp=%10p\n", row, dp, row+png_ptr->row_buf_size);
                           printf("row_buf_size=%lu\n", png_ptr->row_buf_size);
                        }
#endif
                        png_memcpy(dp, v, BPP4);
                        dp -= BPP4;
                     }
                     sptr -= BPP4;
                  }
               }
               else if (pixel_bytes == 1)
               {
                  for (i = width; i; i--)
                  {
                     int j;
                     for (j = 0; j < png_pass_inc[pass]; j++)
                     {
                        *dp-- = *sptr;
                     }
                     --sptr;
                  }
               }
               else if (pixel_bytes == BPP2)
               {
                  for (i = width; i; i--)
                  {
                     png_byte v[8];
                     int j;
                     png_memcpy(v, sptr, BPP2);
                     for (j = 0; j < png_pass_inc[pass]; j++)
                     {
                        png_memcpy(dp, v, BPP2);
                        dp -= BPP2;
                     }
                     sptr -= BPP2;
                  }
               }
               else if (pixel_bytes == BPP6)
               {
                  for (i = width; i; i--)
                  {
                     png_byte v[8];
                     int j;
                     png_memcpy(v, sptr, BPP6);
                     for (j = 0; j < png_pass_inc[pass]; j++)
                     {
                        png_memcpy(dp, v, BPP6);
                        dp -= BPP6;
                     }
                     sptr -= BPP6;
                  }
               }
               else if (pixel_bytes == BPP8)
               {
                  for (i = width; i; i--)
                  {
                     png_byte v[8];
                     int j;
                     png_memcpy(v, sptr, BPP8);
                     for (j = 0; j < png_pass_inc[pass]; j++)
                     {
                        png_memcpy(dp, v, BPP8);
                        dp -= BPP8;
                     }
                     sptr -= BPP8;
                  }
               }
               else
               {
                  
#if defined(PNG_DEBUG)
                  png_debug(1, "Internal libpng logic error (GCC "
                    "png_do_read_interlace() !_mmx_supported)\n");
#endif
               }

            } 
            break;
         } 
      } 

      row_info->width = final_width;

      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth,final_width);
   }

} 

#endif 
#endif 



#if defined(PNG_HAVE_MMX_READ_FILTER_ROW)
#if defined(PNG_MMX_READ_FILTER_AVG_SUPPORTED)



static void 
png_read_filter_row_mmx_avg(png_row_infop row_info, png_bytep row,
                            png_bytep prev_row)
{
   unsigned FullLength, MMXLength;  
   int bpp;
   int dummy_value_a;
   int dummy_value_c;   
   int dummy_value_d;
   png_bytep dummy_value_S;
   png_bytep dummy_value_D;
   int diff; 

   bpp = (row_info->pixel_depth + 7) >> 3;  
   FullLength = row_info->rowbytes;         

   __asm__ __volatile__ (
   "avg_top:                       \n\t"
      SAVE_GOT_ebx
      SAVE_r15
      SAVE_ebp
      
      "xorl %%ebx, %%ebx           \n\t" 
      "mov  %5, " PDX "            \n\t" 
      "sub  " PCX "," PDX "        \n\t" 
      SAVE_FullLength                    
      "xorl %%eax, %%eax           \n\t"

      
      
   "avg_rlp:                       \n\t"
      "movb (%4," PBX ",), %%al    \n\t" 
      "incl %%ebx                  \n\t"
      "shrb %%al                   \n\t" 
      "addb -1(%5," PBX ",), %%al  \n\t" 
      "cmpl %%ecx, %%ebx           \n\t"
      "movb %%al, -1(%5," PBX ",)  \n\t" 
      "jb avg_rlp                  \n\t" 

      
      
      
      "mov  %5, " PBP "            \n\t" 
      "add  " PBX "," PBP "        \n\t" 
      "add  $0xf, " PBP "          \n\t" 
      CLEAR_BOTTOM_3_BITS  PBP    "\n\t" 
      "sub  %5, " PBP "            \n\t" 
      "jz avg_go                   \n\t" 

      "xorl %%ecx, %%ecx           \n\t"

      
      
      
   "avg_lp1:                       \n\t"
      "xorl %%eax, %%eax           \n\t"
      "movb (%4," PBX ",), %%cl    \n\t" 
      "movb (" PDX "," PBX ",), %%al \n\t" 
      "addw %%cx, %%ax             \n\t"
      "incl %%ebx                  \n\t"
      "shrw %%ax                   \n\t" 
      "addb -1(%5," PBX ",), %%al  \n\t" 
      "cmpl %%ebp, %%ebx           \n\t" 
      "movb %%al, -1(%5," PBX ",)  \n\t" 
      "jb avg_lp1                  \n\t" 

   "avg_go:                        \n\t"
      RESTORE_FullLength "%%eax    \n\t" 
      "movl %%eax, %%ecx           \n\t" 
      "subl %%ebx, %%eax           \n\t" 
      "andl $0x00000007, %%eax     \n\t" 
      "subl %%eax, %%ecx           \n\t" 
      "movl %%ebp, %%eax           \n\t" 
      RESTORE_ebp                        
      RESTORE_r15
      RESTORE_GOT_ebx




      
      
      : "=c" (MMXLength),      
        "=a" (diff)            

      
      
      
      : "0" (bpp),         
        "1" (FullLength),  
        "S" (prev_row),    
        "D" (row)          

      : "%edx"                           
        _CLOBBER_r15
        _CLOBBER_ebp
        _CLOBBER_GOT_ebx
   );

   
   switch (bpp)
   {
      case 3:
      {

         __asm__ __volatile__ (
            
            LOAD_GOT_rbp
            "movq " AMASK5_3_0 ", %%mm7    \n\t" 
                                                 
            "movq " LB_CARRY_MASK ", %%mm5 \n\t" 
            "movq " HB_CLEAR_MASK ", %%mm4 \n\t" 
            RESTORE_rbp

            
            "movq  -8(%1," PCX ",), %%mm2 \n\t"
                                               
         "avg_3lp:                        \n\t"
            "movq  (%1," PCX ",), %%mm0   \n\t" 
            "movq  %%mm5, %%mm3           \n\t"
            "psrlq $40, %%mm2             \n\t" 
                                                
            "movq  (%0," PCX ",), %%mm1   \n\t" 
            "movq  %%mm7, %%mm6           \n\t"
            "pand  %%mm1, %%mm3           \n\t" 
            "psrlq $1, %%mm1              \n\t" 
            "pand  %%mm4, %%mm1           \n\t" 
                                                
            "paddb %%mm1, %%mm0           \n\t" 
                                                
            
            "movq  %%mm3, %%mm1           \n\t" 
                                                
            "pand  %%mm2, %%mm1           \n\t" 
                                                
                                                
            "psrlq $1, %%mm2              \n\t" 
            "pand  %%mm4, %%mm2           \n\t" 
                                                
            "paddb %%mm1, %%mm2           \n\t" 
                                                
            "pand  %%mm6, %%mm2           \n\t" 
                                                
            "paddb %%mm2, %%mm0           \n\t" 
                                                
            
            "psllq $24, %%mm6             \n\t" 
                                                
            "movq  %%mm0, %%mm2           \n\t" 
            "psllq $24, %%mm2             \n\t" 
            "movq  %%mm3, %%mm1           \n\t" 
                                                
            "pand  %%mm2, %%mm1           \n\t" 
                                                
                                                
            "psrlq $1, %%mm2              \n\t" 
            "pand  %%mm4, %%mm2           \n\t" 
                                                
            "paddb %%mm1, %%mm2           \n\t" 
                                                
            "pand  %%mm6, %%mm2           \n\t" 
                                                
            "paddb %%mm2, %%mm0           \n\t" 
                                                

            
            "psllq $24, %%mm6             \n\t" 
                                                
            "movq  %%mm0, %%mm2           \n\t" 
            "psllq $24, %%mm2             \n\t" 
                              
                              
            "movq  %%mm3, %%mm1           \n\t" 
                                                
            "pand  %%mm2, %%mm1           \n\t" 
                                                
                              
            "psrlq $1, %%mm2              \n\t" 
            "pand  %%mm4, %%mm2           \n\t" 
                                                
            "paddb %%mm1, %%mm2           \n\t" 
                                                
            "pand  %%mm6, %%mm2           \n\t" 
                                                
            "addl  $8, %%ecx              \n\t"
            "paddb %%mm2, %%mm0           \n\t" 
                                                
            
            "movq  %%mm0, -8(%1," PCX ",) \n\t"
            
            "cmpl  %%eax, %%ecx           \n\t" 
            "movq  %%mm0, %%mm2           \n\t" 
            "jb avg_3lp                   \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),    
              "1" (row),         
              "2" (diff),        
              "3" (MMXLength)    

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2", "%mm3"   
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      case 4:   
      {         
                

         __asm__ __volatile__ (
            LOAD_GOT_rbp
            "movq " HB_CLEAR_MASK ", %%mm4 \n\t" 
            "movq " LB_CARRY_MASK ", %%mm5 \n\t" 
            
                                                 
            "movq " AMASK0_8_0 ", %%mm7    \n\t" 
            RESTORE_rbp

            
            "psrlq $32, %%mm7            \n\t" 
            "movq  %%mm7, %%mm6          \n\t"
            "psllq $32, %%mm6            \n\t" 

            
            "movq -8(%1," PCX ",), %%mm2 \n\t" 
                                             
         "avg_4lp:                       \n\t"
            "movq (%1," PCX ",), %%mm0   \n\t"
            "psrlq $32, %%mm2            \n\t" 
            "movq (%0," PCX ",), %%mm1   \n\t"
            
            "movq %%mm5, %%mm3           \n\t"
            "pand %%mm1, %%mm3           \n\t" 
            "psrlq $1, %%mm1             \n\t" 
            "pand  %%mm4, %%mm1          \n\t" 
                                               
            "paddb %%mm1, %%mm0          \n\t" 
                                               
            
            "movq %%mm3, %%mm1           \n\t" 
                                               
            "pand %%mm2, %%mm1           \n\t" 
                                               
                              
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
                                               
            "paddb %%mm1, %%mm2          \n\t" 
                                               
            "pand %%mm7, %%mm2           \n\t" 
                                               
            "paddb %%mm2, %%mm0          \n\t" 
                                               
                              
            
            "movq %%mm0, %%mm2           \n\t" 
            "psllq $32, %%mm2            \n\t" 
            "addl $8, %%ecx              \n\t"
            "movq %%mm3, %%mm1           \n\t" 
                                               
            "pand %%mm2, %%mm1           \n\t" 
                                               
                              
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
                                               
            "paddb %%mm1, %%mm2          \n\t" 
                                               
            "pand %%mm6, %%mm2           \n\t" 
                                               
            "paddb %%mm2, %%mm0          \n\t" 
                                               
            "cmpl %%eax, %%ecx           \n\t" 
            
            "movq %%mm0, -8(%1," PCX ",) \n\t"
            
            "movq %%mm0, %%mm2           \n\t" 
            "jb avg_4lp                  \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),    
              "1" (row),         
              "2" (diff),        
              "3" (MMXLength)    

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2", "%mm3"   
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      case 1:
      {
         __asm__ __volatile__ (
            
            "cmpl %%eax, %%ecx           \n\t" 
            "jnb avg_1end                \n\t"

            SAVE_ebp

            
            "mov  %1, " PBP "            \n\t" 
            "dec  " PBP "                \n\t" 
            "xorl %%edx, %%edx           \n\t" 
                                               
            SAVE_GOT_ebx

         "avg_1lp:                       \n\t"
            
            "xorl %%ebx, %%ebx           \n\t"
            "movb (%0," PCX ",), %%dl    \n\t" 
            "movb (" PBP "," PCX ",), %%bl \n\t" 
            "addw %%dx, %%bx             \n\t"
            "incl %%ecx                  \n\t"
            "shrw %%bx                   \n\t" 
            "addb -1(%1," PCX ",), %%bl  \n\t" 
                                               
            "cmpl %%eax, %%ecx           \n\t" 
            "movb %%bl, -1(%1," PCX ",)  \n\t" 
                         
            "jb avg_1lp                  \n\t"

            RESTORE_GOT_ebx
            RESTORE_ebp

         "avg_1end:                      \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),    
              "1" (row),         
              "2" (diff),        
              "3" (FullLength)   

            : "%edx"                           
              _CLOBBER_GOT_ebx
              _CLOBBER_ebp
         );
      }
      return;  

      case 2:
      {

         __asm__ __volatile__ (
            LOAD_GOT_rbp
            
            "movq " AMASK6_2_0 ", %%mm7    \n\t" 
            
                                                 
            "movq " LB_CARRY_MASK ", %%mm5 \n\t" 
            "movq " HB_CLEAR_MASK ", %%mm4 \n\t" 
            RESTORE_rbp

            
            "movq -8(%1," PCX ",), %%mm2 \n\t" 
                                             
         "avg_2lp:                       \n\t"
            "movq (%1," PCX ",), %%mm0   \n\t"
            "psrlq $48, %%mm2            \n\t" 
            "movq (%0," PCX ",), %%mm1   \n\t" 
            
            "movq %%mm5, %%mm3           \n\t"
            "pand %%mm1, %%mm3           \n\t" 
            "psrlq $1, %%mm1             \n\t" 
            "pand  %%mm4, %%mm1          \n\t" 
                                               
            "movq %%mm7, %%mm6           \n\t"
            "paddb %%mm1, %%mm0          \n\t" 
                                               

            
            "movq %%mm3, %%mm1           \n\t" 
                                               
            "pand %%mm2, %%mm1           \n\t" 
                                               
                                               
                                               
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
                                               
            "paddb %%mm1, %%mm2          \n\t" 
                                               
            "pand %%mm6, %%mm2           \n\t" 
                                               
            "paddb %%mm2, %%mm0          \n\t" 
                                               

            
            "psllq $16, %%mm6            \n\t" 
                                               
            "movq %%mm0, %%mm2           \n\t" 
            "psllq $16, %%mm2            \n\t" 
            "movq %%mm3, %%mm1           \n\t" 
                                               
            "pand %%mm2, %%mm1           \n\t" 
                                               
                                               
                                               
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
                                               
            "paddb %%mm1, %%mm2          \n\t" 
                                               
            "pand %%mm6, %%mm2           \n\t" 
                                               
            "paddb %%mm2, %%mm0          \n\t" 
                                               

            
            "psllq $16, %%mm6            \n\t" 
                                               
            "movq %%mm0, %%mm2           \n\t" 
            "psllq $16, %%mm2            \n\t" 
            "movq %%mm3, %%mm1           \n\t" 
                                               
            "pand %%mm2, %%mm1           \n\t" 
                                               
                                               
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
                                               
            "paddb %%mm1, %%mm2          \n\t" 
                                               
            "pand %%mm6, %%mm2           \n\t" 
                                               
            "paddb %%mm2, %%mm0          \n\t" 
                                               

            
            "psllq $16, %%mm6            \n\t" 
                                               
            "movq %%mm0, %%mm2           \n\t" 
            "psllq $16, %%mm2            \n\t" 
            "addl $8, %%ecx              \n\t"
            "movq %%mm3, %%mm1           \n\t" 
                                               
            "pand %%mm2, %%mm1           \n\t" 
                                               
                                               
                                               
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
                                               
            "paddb %%mm1, %%mm2          \n\t" 
                                               
            "pand %%mm6, %%mm2           \n\t" 
                                               
            "paddb %%mm2, %%mm0          \n\t" 
                                               
            "cmpl %%eax, %%ecx           \n\t" 
            
            "movq %%mm0, -8(%1," PCX ",) \n\t"
            
            "movq %%mm0, %%mm2           \n\t" 
            "jb avg_2lp                  \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),    
              "1" (row),         
              "2" (diff),        
              "3" (MMXLength)    

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2", "%mm3"   
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      case 6:   
      {

         __asm__ __volatile__ (
            LOAD_GOT_rbp
            "movq " HB_CLEAR_MASK ", %%mm4 \n\t" 
            "movq " LB_CARRY_MASK ", %%mm5 \n\t" 
            
                                                 
            "movq " AMASK0_8_0 ", %%mm7    \n\t" 
            RESTORE_rbp

            
            "psrlq $16, %%mm7            \n\t"
            "movq  %%mm7, %%mm6          \n\t"
            "psllq $48, %%mm6            \n\t" 

            
            "movq -8(%1," PCX ",), %%mm2 \n\t" 
                                             
         "avg_6lp:                       \n\t"
            "movq (%1," PCX ",), %%mm0   \n\t"
            "psrlq $16, %%mm2            \n\t" 
            "movq (%0," PCX ",), %%mm1   \n\t"
            
            "movq %%mm5, %%mm3           \n\t"
            "pand %%mm1, %%mm3           \n\t" 
            "psrlq $1, %%mm1             \n\t" 
            "pand  %%mm4, %%mm1          \n\t" 
                                               
            "paddb %%mm1, %%mm0          \n\t" 
                                               
            
            "movq %%mm3, %%mm1           \n\t" 
                                               
            "pand %%mm2, %%mm1           \n\t" 
                                               
                              
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
                                               
            "paddb %%mm1, %%mm2          \n\t" 
                                               
            "pand %%mm7, %%mm2           \n\t" 
                                               
            "paddb %%mm2, %%mm0          \n\t" 
                                               
                              
            
            "movq %%mm0, %%mm2           \n\t" 
            "psllq $48, %%mm2            \n\t" 
            "addl $8, %%ecx              \n\t"
            "movq %%mm3, %%mm1           \n\t" 
                                               
            "pand %%mm2, %%mm1           \n\t" 
                                               
                              
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
                                               
            "paddb %%mm1, %%mm2          \n\t" 
                                               
            "pand %%mm6, %%mm2           \n\t" 
                                               
            "paddb %%mm2, %%mm0          \n\t" 
                                               
            "cmpl %%eax, %%ecx           \n\t" 
            
            "movq %%mm0, -8(%1," PCX ",) \n\t"
            
            "movq %%mm0, %%mm2           \n\t" 
            "jb avg_6lp                  \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),    
              "1" (row),         
              "2" (diff),        
              "3" (MMXLength)    

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2", "%mm3"   
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      case 8:
      {
         __asm__ __volatile__ (
            
                                                 
            LOAD_GOT_rbp
            "movq " LB_CARRY_MASK ", %%mm5 \n\t" 
            "movq " HB_CLEAR_MASK ", %%mm4 \n\t" 
            RESTORE_rbp

            
            "movq -8(%1," PCX ",), %%mm2 \n\t" 
                                      

         "avg_8lp:                       \n\t"
            "movq (%1," PCX ",), %%mm0   \n\t"
            "movq %%mm5, %%mm3           \n\t"
            "movq (%0," PCX ",), %%mm1   \n\t"
            "addl $8, %%ecx              \n\t"
            "pand %%mm1, %%mm3           \n\t" 
            "psrlq $1, %%mm1             \n\t" 
            "pand %%mm2, %%mm3           \n\t" 
                                               
            "psrlq $1, %%mm2             \n\t" 
            "pand  %%mm4, %%mm1          \n\t" 
            "paddb %%mm3, %%mm0          \n\t" 
            "pand  %%mm4, %%mm2          \n\t" 
            "paddb %%mm1, %%mm0          \n\t" 
            "paddb %%mm2, %%mm0          \n\t" 
            "cmpl %%eax, %%ecx           \n\t" 
            "movq %%mm0, -8(%1," PCX ",) \n\t"
            "movq %%mm0, %%mm2           \n\t" 
            "jb avg_8lp                  \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),    
              "1" (row),         
              "2" (diff),        
              "3" (MMXLength)    

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2"           
            , "%mm3", "%mm4", "%mm5"
#endif
         );
      }
      break;  

      default:                
      {
         
#if defined(PNG_DEBUG)
         png_debug(1, "Internal libpng logic error (GCC "
           "png_read_filter_row_mmx_avg())\n");
#endif
      }
      break;

   } 

   __asm__ __volatile__ (
      
      
      "cmpl %%edx, %%eax           \n\t" 
      "jnb avg_end                 \n\t"

      SAVE_ebp

      
      "mov  %2, " PBP "            \n\t" 
      "sub  " PCX "," PBP "        \n\t" 
      "xorl %%ecx, %%ecx           \n\t" 

      SAVE_GOT_ebx

   "avg_lp2:                       \n\t"
      
      "xorl %%ebx, %%ebx           \n\t"
      "movb (%1," PAX ",), %%cl    \n\t" 
      "movb (" PBP "," PAX ",), %%bl \n\t" 
      "addw %%cx, %%bx             \n\t"
      "incl %%eax                  \n\t"
      "shrw %%bx                   \n\t" 
      "addb -1(%2," PAX ",), %%bl  \n\t" 
      "cmpl %%edx, %%eax           \n\t" 
      "movb %%bl, -1(%2," PAX ",)  \n\t" 
      "jb avg_lp2                  \n\t" 

      RESTORE_GOT_ebx
      RESTORE_ebp

   "avg_end:                       \n\t"
      "EMMS                        \n\t" 

      : "=c" (dummy_value_c),            
        "=S" (dummy_value_S),
        "=D" (dummy_value_D),
        "=a" (dummy_value_a),
        "=d" (dummy_value_d)

      : "0" (bpp),         
        "1" (prev_row),    
        "2" (row),         
        "3" (MMXLength),   
        "4" (FullLength)   

      CLOB_COLON_ebx_ebp                 
        CLOBBER_GOT_ebx
        CLOB_COMMA_ebx_ebp
        CLOBBER_ebp
   );

} 

#endif 



#if defined(PNG_MMX_READ_FILTER_PAETH_SUPPORTED)
#if defined(PNG_x86_64_USE_GOTPCREL) || defined(PNG_THREAD_UNSAFE_OK)



static void 
png_read_filter_row_mmx_paeth(png_row_infop row_info, png_bytep row,
                              png_bytep prev_row)
{
   unsigned FullLength, MMXLength;  
   int bpp;
   int dummy_value_a;
   int dummy_value_c;   
   int dummy_value_d;
   png_charp dummy_value_S;
   png_charp dummy_value_D;
   int diff; 

   bpp = (row_info->pixel_depth + 7) >> 3;  
   FullLength = row_info->rowbytes;         

   __asm__ __volatile__ (
      SAVE_GOT_ebx
      SAVE_r15
      SAVE_ebp
      "xorl %%ebx, %%ebx           \n\t" 
      "xorl %%edx, %%edx           \n\t" 
      SAVE_FullLength                    
      "xorl %%eax, %%eax           \n\t"

      
      
      
   "paeth_rlp:                     \n\t"
      "movb (%2," PBX ",), %%al    \n\t"
      "addb (%1," PBX ",), %%al    \n\t"
      "incl %%ebx                  \n\t"
      "cmpl %%ecx, %%ebx           \n\t"
      "movb %%al, -1(%2," PBX ",)  \n\t"
      "jb paeth_rlp                \n\t"

      
      
      "mov  %2, " PBP "            \n\t" 
      "add  " PBX "," PBP "        \n\t" 
      "add  $0xf, " PBP "          \n\t" 
      CLEAR_BOTTOM_3_BITS  PBP    "\n\t" 
      "sub  %2, " PBP "            \n\t" 
      "jz paeth_go                 \n\t" 

      "xorl %%ecx, %%ecx           \n\t"

      SAVE_r11_r12_r13

      
   "paeth_lp1:                     \n\t"
      "xorl %%eax, %%eax           \n\t"
      
      "movb (%1," PBX ",), %%al    \n\t" 
      "movb (%1," PDX ",), %%cl    \n\t" 
      "subl %%ecx, %%eax           \n\t" 
      "movl %%eax, " pa_TEMP "     \n\t" 
      "xorl %%eax, %%eax           \n\t"
      
      "movb (%2," PDX ",), %%al    \n\t" 
      "subl %%ecx, %%eax           \n\t" 
      "movl %%eax, %%ecx           \n\t"
      
      "addl " pa_TEMP ", %%eax     \n\t" 
      
      "testl $0x80000000, %%eax    \n\t"
      "jz paeth_pca                \n\t"
      "negl %%eax                  \n\t" 

   "paeth_pca:                     \n\t"
      "movl %%eax, " pc_TEMP "     \n\t" 
      
      "testl $0x80000000, %%ecx    \n\t"
      "jz paeth_pba                \n\t"
      "negl %%ecx                  \n\t" 

   "paeth_pba:                     \n\t"
      "movl %%ecx, " pb_TEMP "     \n\t" 
      
      "movl " pa_TEMP ", %%eax     \n\t"
      "testl $0x80000000, %%eax    \n\t"
      "jz paeth_paa                \n\t"
      "negl %%eax                  \n\t" 

   "paeth_paa:                     \n\t"
      "movl %%eax, " pa_TEMP "     \n\t" 
      
      "cmpl %%ecx, %%eax           \n\t"
      "jna paeth_abb               \n\t"
      
      "cmpl " pc_TEMP ", %%ecx     \n\t"
      "jna paeth_bbc               \n\t"
      
      "movb (%1," PDX ",), %%cl    \n\t" 
      "jmp paeth_paeth             \n\t"

   "paeth_bbc:                     \n\t"
      
      "movb (%1," PBX ",), %%cl    \n\t" 
      "jmp paeth_paeth             \n\t"

   "paeth_abb:                     \n\t"
      
      "cmpl " pc_TEMP ", %%eax     \n\t"
      "jna paeth_abc               \n\t"
      
      "movb (%1," PDX ",), %%cl    \n\t" 
      "jmp paeth_paeth             \n\t"

   "paeth_abc:                     \n\t"
      
      "movb (%2," PDX ",), %%cl    \n\t" 

   "paeth_paeth:                   \n\t"
      "incl %%ebx                  \n\t"
      "incl %%edx                  \n\t"
      
      "addb %%cl, -1(%2," PBX ",)  \n\t"
      "cmpl %%ebp, %%ebx           \n\t"
      "jb paeth_lp1                \n\t"

      RESTORE_r11_r12_r13

   "paeth_go:                      \n\t"
      RESTORE_FullLength "%%ecx    \n\t" 
      "movl %%ecx, %%eax           \n\t"
      "subl %%ebx, %%eax           \n\t" 
      "andl $0x00000007, %%eax     \n\t" 
      "subl %%eax, %%ecx           \n\t" 
      "movl %%ebp, %%eax           \n\t" 
      RESTORE_ebp                        
      RESTORE_r15
      RESTORE_GOT_ebx

      : "=c" (MMXLength),                
        "=S" (dummy_value_S),
        "=D" (dummy_value_D),
        "=a" (diff)

      : "0" (bpp),         
        "1" (prev_row),    
        "2" (row),         
        "3" (FullLength)   

      : "%edx"                           
        _CLOBBER_r11_r12_r13
        _CLOBBER_r15
        _CLOBBER_ebp
        _CLOBBER_GOT_ebx
   );

   
   switch (bpp)
   {
      case 3:
      {

         __asm__ __volatile__ (
            LOAD_GOT_rbp
            "pxor %%mm0, %%mm0           \n\t"

            
            "movq -8(%1," PCX ",), %%mm1 \n\t"
         "paeth_3lp:                     \n\t"
            "psrlq $40, %%mm1            \n\t" 
                                               
            "movq (%0," PCX ",), %%mm2   \n\t" 
            "punpcklbw %%mm0, %%mm1      \n\t" 
            "movq -8(%0," PCX ",), %%mm3 \n\t" 
            "punpcklbw %%mm0, %%mm2      \n\t" 
            "psrlq $40, %%mm3            \n\t" 
                                               
            
            "movq %%mm2, %%mm4           \n\t"
            "punpcklbw %%mm0, %%mm3      \n\t" 
            
            "movq %%mm1, %%mm5           \n\t"
            "psubw %%mm3, %%mm4          \n\t"
            "pxor %%mm7, %%mm7           \n\t"
            
            "movq %%mm4, %%mm6           \n\t"
            "psubw %%mm3, %%mm5          \n\t"

            
            
            
            "pcmpgtw %%mm4, %%mm0        \n\t" 
            "paddw %%mm5, %%mm6          \n\t"
            "pand %%mm4, %%mm0           \n\t" 
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "pand %%mm5, %%mm7           \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm7, %%mm5          \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "pxor %%mm1, %%mm1           \n\t"
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "packuswb %%mm1, %%mm7       \n\t"
            "movq (%0," PCX ",), %%mm3   \n\t" 
            "pand " AMASK5_3_0 ", %%mm7  \n\t" 
            "movq %%mm3, %%mm2           \n\t" 
            "paddb (%1," PCX ",), %%mm7  \n\t" 
            "punpcklbw %%mm0, %%mm3      \n\t" 
            "movq %%mm7, (%1," PCX ",)   \n\t" 
            "movq %%mm7, %%mm1           \n\t" 
                                               
            
            "psrlq $24, %%mm2            \n\t" 
            "punpcklbw %%mm0, %%mm1      \n\t" 
            "pxor %%mm7, %%mm7           \n\t"
            "punpcklbw %%mm0, %%mm2      \n\t" 
            
            "movq %%mm1, %%mm5           \n\t"
            
            "movq %%mm2, %%mm4           \n\t"
            "psubw %%mm3, %%mm5          \n\t"
            "psubw %%mm3, %%mm4          \n\t"
            
            
            "movq %%mm5, %%mm6           \n\t"
            "paddw %%mm4, %%mm6          \n\t"

            
            
            
            "pcmpgtw %%mm5, %%mm0        \n\t" 
            "pcmpgtw %%mm4, %%mm7        \n\t" 
            "pand %%mm5, %%mm0           \n\t" 
            "pand %%mm4, %%mm7           \n\t" 
            "psubw %%mm0, %%mm5          \n\t"
            "psubw %%mm7, %%mm4          \n\t"
            "psubw %%mm0, %%mm5          \n\t"
            "psubw %%mm7, %%mm4          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "movq (%0," PCX ",), %%mm2   \n\t" 
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "pxor %%mm1, %%mm1           \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "packuswb %%mm1, %%mm7       \n\t"
            "movq %%mm2, %%mm3           \n\t" 
            "pand " AMASK5_3_0 ", %%mm7  \n\t" 
            "punpckhbw %%mm0, %%mm2      \n\t" 
            "psllq $24, %%mm7            \n\t" 
                                               
             
            "movq %%mm2, %%mm4           \n\t"
            "paddb (%1," PCX ",), %%mm7  \n\t" 
            "psllq $24, %%mm3            \n\t" 
            "movq %%mm7, (%1," PCX ",)   \n\t" 
            "movq %%mm7, %%mm1           \n\t"
            "punpckhbw %%mm0, %%mm3      \n\t" 
            "psllq $24, %%mm1            \n\t" 
                                    
            
            "pxor %%mm7, %%mm7           \n\t"
            "punpckhbw %%mm0, %%mm1      \n\t" 
            "psubw %%mm3, %%mm4          \n\t"
            
            "movq %%mm1, %%mm5           \n\t"
            
            "movq %%mm4, %%mm6           \n\t"
            "psubw %%mm3, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "paddw %%mm5, %%mm6          \n\t"

            
            
            
            "pcmpgtw %%mm4, %%mm0        \n\t" 
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "pand %%mm4, %%mm0           \n\t" 
            "pand %%mm5, %%mm7           \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm1, %%mm1           \n\t"
            "packuswb %%mm7, %%mm1       \n\t"
            
            "addl $8, %%ecx              \n\t"
            "pand " AMASK0_2_6 ", %%mm1  \n\t" 
            "paddb -8(%1," PCX ",), %%mm1 \n\t" 
            "cmpl %%eax, %%ecx           \n\t" 
            "pxor %%mm0, %%mm0           \n\t" 
            "movq %%mm1, -8(%1," PCX ",) \n\t" 
                                 
                           
            "jb paeth_3lp                \n\t"
            RESTORE_rbp

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),  
              "1" (row),       
              "2" (diff),      
              "3" (MMXLength)  

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2", "%mm3"   
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      case 4:
      {
         __asm__ __volatile__ (
            "pxor %%mm0, %%mm0           \n\t"
            
            "movq -8(%1," PCX ",), %%mm1 \n\t" 
                                               
         "paeth_4lp:                     \n\t"
            
            "movq -8(%0," PCX ",), %%mm3 \n\t" 
            "punpckhbw %%mm0, %%mm1      \n\t" 
            "movq (%0," PCX ",), %%mm2   \n\t" 
            "punpcklbw %%mm0, %%mm2      \n\t" 
            
            "movq %%mm2, %%mm4           \n\t"
            "punpckhbw %%mm0, %%mm3      \n\t" 
            
            "movq %%mm1, %%mm5           \n\t"
            "psubw %%mm3, %%mm4          \n\t"
            "pxor %%mm7, %%mm7           \n\t"
            
            "movq %%mm4, %%mm6           \n\t"
            "psubw %%mm3, %%mm5          \n\t"
            
            
            
            "pcmpgtw %%mm4, %%mm0        \n\t" 
            "paddw %%mm5, %%mm6          \n\t"
            "pand %%mm4, %%mm0           \n\t" 
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "pand %%mm5, %%mm7           \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm7, %%mm5          \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "pxor %%mm1, %%mm1           \n\t"
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "packuswb %%mm1, %%mm7       \n\t"
            "movq (%0," PCX ",), %%mm3   \n\t" 
            LOAD_GOT_rbp
            "pand " AMASK4_4_0 ", %%mm7  \n\t" 
            RESTORE_rbp
            "movq %%mm3, %%mm2           \n\t" 
            "paddb (%1," PCX ",), %%mm7  \n\t" 
            "punpcklbw %%mm0, %%mm3      \n\t" 
            "movq %%mm7, (%1," PCX ",)   \n\t" 
            "movq %%mm7, %%mm1           \n\t" 
                                               
            
            "punpckhbw %%mm0, %%mm2      \n\t" 
            "punpcklbw %%mm0, %%mm1      \n\t" 
            
            "movq %%mm2, %%mm4           \n\t"
            
            "movq %%mm1, %%mm5           \n\t"
            "psubw %%mm3, %%mm4          \n\t"
            "pxor %%mm7, %%mm7           \n\t"
            
            "movq %%mm4, %%mm6           \n\t"
            "psubw %%mm3, %%mm5          \n\t"
            
            
            
            "pcmpgtw %%mm4, %%mm0        \n\t" 
            "paddw %%mm5, %%mm6          \n\t"
            "pand %%mm4, %%mm0           \n\t" 
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "pand %%mm5, %%mm7           \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm7, %%mm5          \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "pxor %%mm1, %%mm1           \n\t"
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "pxor %%mm1, %%mm1           \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            
            "addl $8, %%ecx              \n\t"
            "packuswb %%mm7, %%mm1       \n\t"
            "paddb -8(%1," PCX ",), %%mm1 \n\t" 
            "cmpl %%eax, %%ecx           \n\t" 
            "movq %%mm1, -8(%1," PCX ",) \n\t" 
                                 
            "jb paeth_4lp                \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),  
              "1" (row),       
              "2" (diff),      
              "3" (MMXLength)  

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2", "%mm3"   
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      case 1:
      case 2:
      {
         __asm__ __volatile__ (
            "cmpl %%edx, %%eax           \n\t"
            "jnb paeth_dend              \n\t"

            SAVE_ebp

            
            "movl %%eax, %%ebp           \n\t"
            "subl %%ecx, %%ebp           \n\t" 
            "xorl %%ecx, %%ecx           \n\t" 

            SAVE_GOT_ebx
            SAVE_r11_r12_r13

         "paeth_dlp:                     \n\t"
            "xorl %%ebx, %%ebx           \n\t"
            
            "movb (%1," PAX ",), %%bl    \n\t" 
            "movb (%1," PBP ",), %%cl    \n\t" 
            "subl %%ecx, %%ebx           \n\t" 
            "movl %%ebx, " pa_TEMP "     \n\t" 
            "xorl %%ebx, %%ebx           \n\t"
            
            "movb (%2," PBP ",), %%bl    \n\t" 
            "subl %%ecx, %%ebx           \n\t" 
            "movl %%ebx, %%ecx           \n\t"
            
            "addl " pa_TEMP ", %%ebx     \n\t" 
            
            "testl $0x80000000, %%ebx    \n\t"
            "jz paeth_dpca               \n\t"
            "negl %%ebx                  \n\t" 

         "paeth_dpca:                    \n\t"
            "movl %%ebx, " pc_TEMP "     \n\t" 
            
            "testl $0x80000000, %%ecx    \n\t"
            "jz paeth_dpba               \n\t"
            "negl %%ecx                  \n\t" 

         "paeth_dpba:                    \n\t"
            "movl %%ecx, " pb_TEMP "     \n\t" 
            
            "movl " pa_TEMP ", %%ebx     \n\t"
            "testl $0x80000000, %%ebx    \n\t"
            "jz paeth_dpaa               \n\t"
            "negl %%ebx                  \n\t" 

         "paeth_dpaa:                    \n\t"
            "movl %%ebx, " pa_TEMP "     \n\t" 
            
            "cmpl %%ecx, %%ebx           \n\t"
            "jna paeth_dabb              \n\t"
            
            "cmpl " pc_TEMP ", %%ecx     \n\t"
            "jna paeth_dbbc              \n\t"
            
            "movb (%1," PBP ",), %%cl    \n\t" 
            "jmp paeth_dpaeth            \n\t"

         "paeth_dbbc:                    \n\t"
            
            "movb (%1," PAX ",), %%cl    \n\t" 
            "jmp paeth_dpaeth            \n\t"

         "paeth_dabb:                    \n\t"
            
            "cmpl " pc_TEMP ", %%ebx     \n\t"
            "jna paeth_dabc              \n\t"
            
            "movb (%1," PBP ",), %%cl   \n\t" 
            "jmp paeth_dpaeth            \n\t"

         "paeth_dabc:                    \n\t"
            
            "movb (%2," PBP ",), %%cl    \n\t" 

         "paeth_dpaeth:                  \n\t"
            "incl %%eax                  \n\t"
            "incl %%ebp                  \n\t"
            
            "addb %%cl, -1(%2," PAX ",)  \n\t"
            "cmpl %%edx, %%eax           \n\t" 
            "jb paeth_dlp                \n\t"

            RESTORE_r11_r12_r13
            RESTORE_GOT_ebx
            RESTORE_ebp

         "paeth_dend:                    \n\t"

            : "=c" (dummy_value_c),            
              "=S" (dummy_value_S),
              "=D" (dummy_value_D),
              "=a" (dummy_value_a),
              "=d" (dummy_value_d)

            : "0" (bpp),         
              "1" (prev_row),    
              "2" (row),         
              "3" (diff),        
              "4" (FullLength)   

            CLOB_COLON_ebx_ebp_r1X             
              CLOBBER_GOT_ebx
              CLOB_COMMA_ebx_ebp
              CLOBBER_ebp
              CLOB_COMMA_ebX_r1X
              CLOBBER_r11_r12_r13
         );
      }
      return; 

      case 6:
      {

         __asm__ __volatile__ (
            
            "movq -8(%1," PCX ",), %%mm1 \n\t"
            "pxor %%mm0, %%mm0           \n\t"

         "paeth_6lp:                     \n\t"
            
            "psrlq $16, %%mm1            \n\t" 
            
            "movq -8(%0," PCX ",), %%mm3 \n\t" 
            "punpcklbw %%mm0, %%mm1      \n\t" 
            "movq (%0," PCX ",), %%mm2   \n\t" 
            "punpcklbw %%mm0, %%mm2      \n\t" 
            
            "psrlq $16, %%mm3            \n\t" 
            
            "movq %%mm2, %%mm4           \n\t"
            "punpcklbw %%mm0, %%mm3      \n\t" 
            
            "movq %%mm1, %%mm5           \n\t"
            "psubw %%mm3, %%mm4          \n\t"
            "pxor %%mm7, %%mm7           \n\t"
            
            "movq %%mm4, %%mm6           \n\t"
            "psubw %%mm3, %%mm5          \n\t"
            
            
            
            "pcmpgtw %%mm4, %%mm0        \n\t" 
            "paddw %%mm5, %%mm6          \n\t"
            "pand %%mm4, %%mm0           \n\t" 
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "pand %%mm5, %%mm7           \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm7, %%mm5          \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "pxor %%mm1, %%mm1           \n\t"
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "packuswb %%mm1, %%mm7       \n\t"
            "movq -8(%0," PCX ",), %%mm3 \n\t" 
            LOAD_GOT_rbp
            "pand " AMASK4_4_0 ", %%mm7  \n\t" 
            RESTORE_rbp
            "psrlq $16, %%mm3            \n\t"
            "movq (%0," PCX ",), %%mm2   \n\t" 
            "paddb (%1," PCX ",), %%mm7  \n\t" 
            "movq %%mm2, %%mm6           \n\t"
            "movq %%mm7, (%1," PCX ",)   \n\t" 
            "movq -8(%1," PCX ",), %%mm1 \n\t"
            "psllq $48, %%mm6            \n\t" 
            "movq %%mm7, %%mm5           \n\t"
            "psrlq $16, %%mm1            \n\t" 
            "por %%mm6, %%mm3            \n\t"
            "psllq $48, %%mm5            \n\t" 
            "punpckhbw %%mm0, %%mm3      \n\t" 
            "por %%mm5, %%mm1            \n\t"
            
            "punpckhbw %%mm0, %%mm2      \n\t" 
            "punpckhbw %%mm0, %%mm1      \n\t" 
            
            "movq %%mm2, %%mm4           \n\t"
            
            "movq %%mm1, %%mm5           \n\t"
            "psubw %%mm3, %%mm4          \n\t"
            "pxor %%mm7, %%mm7           \n\t"
            
            "movq %%mm4, %%mm6           \n\t"
            "psubw %%mm3, %%mm5          \n\t"
            
            
            
            "pcmpgtw %%mm4, %%mm0        \n\t" 
            "paddw %%mm5, %%mm6          \n\t"
            "pand %%mm4, %%mm0           \n\t" 
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "pand %%mm5, %%mm7           \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm7, %%mm5          \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "pxor %%mm1, %%mm1           \n\t"
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "pxor %%mm1, %%mm1           \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            
            "addl $8, %%ecx              \n\t"
            "packuswb %%mm7, %%mm1       \n\t"
            "paddb -8(%1," PCX ",), %%mm1 \n\t" 
            "cmpl %%eax, %%ecx           \n\t" 
            "movq %%mm1, -8(%1," PCX ",) \n\t" 
                                 
            "jb paeth_6lp                \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),  
              "1" (row),       
              "2" (diff),      
              "3" (MMXLength)  

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2", "%mm3"   
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      case 8:                          
      {
         __asm__ __volatile__ (
            "pxor %%mm0, %%mm0           \n\t"
            
            "movq -8(%1," PCX ",), %%mm1 \n\t" 
                                               
         "paeth_8lp:                     \n\t"
            
            "movq -8(%0," PCX ",), %%mm3 \n\t" 
            "punpcklbw %%mm0, %%mm1      \n\t" 
            "movq (%0," PCX ",), %%mm2   \n\t" 
            "punpcklbw %%mm0, %%mm2      \n\t" 
            
            "movq %%mm2, %%mm4           \n\t"
            "punpcklbw %%mm0, %%mm3      \n\t" 
            
            "movq %%mm1, %%mm5           \n\t"
            "psubw %%mm3, %%mm4          \n\t"
            "pxor %%mm7, %%mm7           \n\t"
            
            "movq %%mm4, %%mm6           \n\t"
            "psubw %%mm3, %%mm5          \n\t"
            
            
            
            "pcmpgtw %%mm4, %%mm0        \n\t" 
            "paddw %%mm5, %%mm6          \n\t"
            "pand %%mm4, %%mm0           \n\t" 
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "pand %%mm5, %%mm7           \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm7, %%mm5          \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "pxor %%mm1, %%mm1           \n\t"
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "packuswb %%mm1, %%mm7       \n\t"
            "movq -8(%0," PCX ",), %%mm3 \n\t" 
            LOAD_GOT_rbp
            "pand " AMASK4_4_0 ", %%mm7  \n\t" 
            RESTORE_rbp
            "movq (%0," PCX ",), %%mm2   \n\t" 
            "paddb (%1," PCX ",), %%mm7  \n\t" 
            "punpckhbw %%mm0, %%mm3      \n\t" 
            "movq %%mm7, (%1," PCX ",)   \n\t" 
            "movq -8(%1," PCX ",), %%mm1 \n\t" 

            
            "punpckhbw %%mm0, %%mm2      \n\t" 
            "punpckhbw %%mm0, %%mm1      \n\t" 
            
            "movq %%mm2, %%mm4           \n\t"
            
            "movq %%mm1, %%mm5           \n\t"
            "psubw %%mm3, %%mm4          \n\t"
            "pxor %%mm7, %%mm7           \n\t"
            
            "movq %%mm4, %%mm6           \n\t"
            "psubw %%mm3, %%mm5          \n\t"
            
            
            
            "pcmpgtw %%mm4, %%mm0        \n\t" 
            "paddw %%mm5, %%mm6          \n\t"
            "pand %%mm4, %%mm0           \n\t" 
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "pand %%mm5, %%mm7           \n\t" 
            "psubw %%mm0, %%mm4          \n\t"
            "psubw %%mm7, %%mm5          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            "pcmpgtw %%mm6, %%mm0        \n\t" 
            "pand %%mm6, %%mm0           \n\t" 
            "psubw %%mm7, %%mm5          \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            
            "movq %%mm4, %%mm7           \n\t"
            "psubw %%mm0, %%mm6          \n\t"
            "pcmpgtw %%mm5, %%mm7        \n\t" 
            "movq %%mm7, %%mm0           \n\t"
            
            "pand %%mm7, %%mm5           \n\t"
            
            "pand %%mm0, %%mm2           \n\t"
            "pandn %%mm4, %%mm7          \n\t"
            "pandn %%mm1, %%mm0          \n\t"
            "paddw %%mm5, %%mm7          \n\t"
            "paddw %%mm2, %%mm0          \n\t"
            
            "pcmpgtw %%mm6, %%mm7        \n\t" 
            "pxor %%mm1, %%mm1           \n\t"
            "pand %%mm7, %%mm3           \n\t"
            "pandn %%mm0, %%mm7          \n\t"
            "pxor %%mm1, %%mm1           \n\t"
            "paddw %%mm3, %%mm7          \n\t"
            "pxor %%mm0, %%mm0           \n\t"
            
            "addl $8, %%ecx              \n\t"
            "packuswb %%mm7, %%mm1       \n\t"
            "paddb -8(%1," PCX ",), %%mm1 \n\t" 
            "cmpl %%eax, %%ecx           \n\t" 
            "movq %%mm1, -8(%1," PCX ",) \n\t" 
                                 
            "jb paeth_8lp                \n\t"

            : "=S" (dummy_value_S),            
              "=D" (dummy_value_D),
              "=c" (dummy_value_c),
              "=a" (dummy_value_a)

            : "0" (prev_row),  
              "1" (row),       
              "2" (diff),      
              "3" (MMXLength)  

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm2", "%mm3"   
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      default:                
      {
         
#if defined(PNG_DEBUG)
         png_debug(1, "Internal libpng logic error (GCC "
           "png_read_filter_row_mmx_paeth())\n");
#endif
      }
      break;

   } 

   __asm__ __volatile__ (
      
      
      "cmpl %%edx, %%eax           \n\t"
      "jnb paeth_end               \n\t"

      SAVE_ebp

      
      "movl %%eax, %%ebp           \n\t"
      "subl %%ecx, %%ebp           \n\t" 
      "xorl %%ecx, %%ecx           \n\t" 

      SAVE_GOT_ebx
      SAVE_r11_r12_r13

   "paeth_lp2:                     \n\t"
      "xorl %%ebx, %%ebx           \n\t"
      
      "movb (%1," PAX ",), %%bl    \n\t" 
      "movb (%1," PBP ",), %%cl    \n\t" 
      "subl %%ecx, %%ebx           \n\t" 
      "movl %%ebx, " pa_TEMP "     \n\t" 
      "xorl %%ebx, %%ebx           \n\t"
      
      "movb (%2," PBP ",), %%bl    \n\t" 
      "subl %%ecx, %%ebx           \n\t" 
      "movl %%ebx, %%ecx           \n\t"
      
      "addl " pa_TEMP ", %%ebx     \n\t" 
      
      "testl $0x80000000, %%ebx    \n\t"
      "jz paeth_pca2               \n\t"
      "negl %%ebx                  \n\t" 

   "paeth_pca2:                    \n\t"
      "movl %%ebx, " pc_TEMP "     \n\t" 
      
      "testl $0x80000000, %%ecx    \n\t"
      "jz paeth_pba2               \n\t"
      "negl %%ecx                  \n\t" 

   "paeth_pba2:                    \n\t"
      "movl %%ecx, " pb_TEMP "     \n\t" 
      
      "movl " pa_TEMP ", %%ebx     \n\t"
      "testl $0x80000000, %%ebx    \n\t"
      "jz paeth_paa2               \n\t"
      "negl %%ebx                  \n\t" 

   "paeth_paa2:                    \n\t"
      "movl %%ebx, " pa_TEMP "     \n\t" 
      
      "cmpl %%ecx, %%ebx           \n\t"
      "jna paeth_abb2              \n\t"
      
      "cmpl " pc_TEMP ", %%ecx     \n\t"
      "jna paeth_bbc2              \n\t"
      
      "movb (%1," PBP ",), %%cl    \n\t" 
      "jmp paeth_paeth2            \n\t"

   "paeth_bbc2:                    \n\t"
      
      "movb (%1," PAX ",), %%cl    \n\t" 
      "jmp paeth_paeth2            \n\t"

   "paeth_abb2:                    \n\t"
      
      "cmpl " pc_TEMP ", %%ebx     \n\t"
      "jna paeth_abc2              \n\t"
      
      "movb (%1," PBP ",), %%cl    \n\t" 
      "jmp paeth_paeth2            \n\t"

   "paeth_abc2:                    \n\t"
      
      "movb (%2," PBP ",), %%cl    \n\t" 

   "paeth_paeth2:                  \n\t"
      "incl %%eax                  \n\t"
      "incl %%ebp                  \n\t"
      
      "addb %%cl, -1(%2," PAX ",)  \n\t"
      "cmpl %%edx, %%eax           \n\t" 
      "jb paeth_lp2                \n\t"

      RESTORE_r11_r12_r13
      RESTORE_GOT_ebx
      RESTORE_ebp

   "paeth_end:                     \n\t"
      "EMMS                        \n\t" 

      : "=c" (dummy_value_c),            
        "=S" (dummy_value_S),
        "=D" (dummy_value_D),
        "=a" (dummy_value_a),
        "=d" (dummy_value_d)

      : "0" (bpp),         
        "1" (prev_row),    
        "2" (row),         
        "3" (MMXLength),   
        "4" (FullLength)   

      CLOB_COLON_ebx_ebp_r1X             
        CLOBBER_GOT_ebx
        CLOB_COMMA_ebx_ebp
        CLOBBER_ebp
        CLOB_COMMA_ebX_r1X
        CLOBBER_r11_r12_r13
   );

} 

#endif 
#endif 




#if defined(PNG_MMX_READ_FILTER_SUB_SUPPORTED)



static void 
png_read_filter_row_mmx_sub(png_row_infop row_info, png_bytep row)
{
   unsigned FullLength, MMXLength;  
   int bpp;
   int dummy_value_a;
   int dummy_value_c;
   int dummy_value_d;
   png_bytep dummy_value_D;
   int diff; 

   bpp = (row_info->pixel_depth + 7) >> 3;  
   FullLength = row_info->rowbytes - bpp;   
     

   __asm__ __volatile__ (
      SAVE_r15
      SAVE_ebp
      "mov  %1, " PSI "            \n\t" 
      "add  " PCX ", %1            \n\t" 
      SAVE_FullLength                    

      "xorl %%eax, %%eax           \n\t"

      
      
      "mov  %1, " PBP "            \n\t" 
      "add  $0xf, " PBP "          \n\t" 
      CLEAR_BOTTOM_3_BITS  PBP    "\n\t" 
      "sub  %1, " PBP "            \n\t" 
      "jz sub_go                   \n\t" 

   "sub_lp1:                       \n\t" 
      "movb (" PSI "," PAX ",), %%cl \n\t"
      "addb %%cl, (%1," PAX ",)    \n\t"
      "incl %%eax                  \n\t"
      "cmpl %%ebp, %%eax           \n\t"
      "jb sub_lp1                  \n\t"

   "sub_go:                        \n\t"
      RESTORE_FullLength "%%ecx    \n\t" 
      "movl %%ecx, %%edx           \n\t"
      "subl %%eax, %%edx           \n\t" 
      "andl $0x00000007, %%edx     \n\t" 
      "subl %%edx, %%ecx           \n\t" 
      "movl %%ebp, %%eax           \n\t" 
      RESTORE_ebp                        
      RESTORE_r15                        

      : "=c" (MMXLength),       
        "=D" (dummy_value_D),   
        "=a" (diff)             

      : "0" (bpp),              
        "1" (row),              
        "2" (FullLength)        

      : "%esi", "%edx"                   
        _CLOBBER_r15
        _CLOBBER_ebp
   );

   
   switch (bpp)
   {
      case 3:
      {

         __asm__ __volatile__ (
            LOAD_GOT_rbp
            
            "movq " AMASK2_3_3 ", %%mm7   \n\t" 
            RESTORE_rbp

            "add  " PCX ", %1             \n\t" 
            "movq %%mm7, %%mm6            \n\t"
            "psllq $24, %%mm6             \n\t" 
                                                
            
            "movq -8(%1," PDX ",), %%mm1  \n\t"

         "sub_3lp:                        \n\t" 
            "psrlq $40, %%mm1             \n\t" 
                                                
            
            "movq (%1," PDX ",), %%mm0    \n\t"
            "paddb %%mm1, %%mm0           \n\t"

            
            "movq %%mm0, %%mm1            \n\t" 
            "psllq $24, %%mm1             \n\t" 
            "pand %%mm7, %%mm1            \n\t" 
            "paddb %%mm1, %%mm0           \n\t"

            
            "movq %%mm0, %%mm1            \n\t" 
            "psllq $24, %%mm1             \n\t" 
            "pand %%mm6, %%mm1            \n\t" 
            "addl $8, %%edx               \n\t"
            "paddb %%mm1, %%mm0           \n\t"

            "cmpl %%eax, %%edx            \n\t" 
            "movq %%mm0, -8(%1," PDX ",)  \n\t" 
            "movq %%mm0, %%mm1            \n\t" 
            "jb sub_3lp                   \n\t"

            : "=c" (dummy_value_c),   
              "=D" (dummy_value_D),   
              "=d" (dummy_value_d),   
              "=a" (dummy_value_a)    

            : "0" (bpp),              
              "1" (row),              
              "2" (diff),             
              "3" (MMXLength)         

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm6", "%mm7"    
#endif
         );
      }
      break;  

      case 4:   
      {         
                

         __asm__ __volatile__ (
            "add  " PCX ", %1             \n\t" 

            
            "movq -8(%1," PDX ",), %%mm1  \n\t"

         "sub_4lp:                        \n\t" 
            "psrlq $32, %%mm1             \n\t" 
                                                
            "movq (%1," PDX ",), %%mm0    \n\t"
            "paddb %%mm1, %%mm0           \n\t"

            
            "movq %%mm0, %%mm1            \n\t" 
            "psllq $32, %%mm1             \n\t" 
            "addl $8, %%edx               \n\t"
            "paddb %%mm1, %%mm0           \n\t"

            "cmpl %%eax, %%edx            \n\t" 
            "movq %%mm0, -8(%1," PDX ",)  \n\t" 
            "movq %%mm0, %%mm1            \n\t" 
            "jb sub_4lp                   \n\t"

            : "=c" (dummy_value_c),   
              "=D" (dummy_value_D),   
              "=d" (dummy_value_d),   
              "=a" (dummy_value_a)    

            : "0" (bpp),              
              "1" (row),              
              "2" (diff),             
              "3" (MMXLength)         

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1"                    
#endif
         );
      }
      break;  

      case 1:
      {
         __asm__ __volatile__ (
            "cmpl %%eax, %%edx             \n\t"
            "jnb sub_1end                  \n\t"
            "mov  %1, " PSI "              \n\t" 
            "add  " PCX ", %1              \n\t" 

         "sub_1lp:                         \n\t"
            "movb (" PSI "," PDX ",), %%cl \n\t"
            "addb %%cl, (%1," PDX ",)      \n\t"
            "incl %%edx                    \n\t"
            "cmpl %%eax, %%edx             \n\t" 
            "jb sub_1lp                    \n\t"

         "sub_1end:                        \n\t"

            : "=c" (dummy_value_c),   
              "=D" (dummy_value_D),   
              "=d" (dummy_value_d),   
              "=a" (dummy_value_a)    

            : "0" (bpp),              
              "1" (row),              
              "2" (diff),             
              "3" (FullLength)        

            : "%esi"                            
         );
      }
      return;  

      case 2:
      {

         __asm__ __volatile__ (
            LOAD_GOT_rbp
            
            "movq " AMASK4_2_2 ", %%mm7   \n\t" 
            RESTORE_rbp
            "movq %%mm7, %%mm6            \n\t"
            "psllq $16, %%mm6             \n\t" 
                                                
            "movq %%mm6, %%mm5            \n\t"
            "add  " PCX ", %1             \n\t" 
            "psllq $16, %%mm5             \n\t" 
                                                
            
            "movq -8(%1," PDX ",), %%mm1  \n\t"

         "sub_2lp:                        \n\t" 
            "psrlq $48, %%mm1             \n\t" 
                                                
            
            "movq (%1," PDX ",), %%mm0    \n\t"
            "paddb %%mm1, %%mm0           \n\t"

            
            "movq %%mm0, %%mm1            \n\t" 
            "psllq $16, %%mm1             \n\t" 
            "pand %%mm7, %%mm1            \n\t" 
            "paddb %%mm1, %%mm0           \n\t"

            
            "movq %%mm0, %%mm1            \n\t" 
            "psllq $16, %%mm1             \n\t" 
            "pand %%mm6, %%mm1            \n\t" 
            "paddb %%mm1, %%mm0           \n\t"

            
            "movq %%mm0, %%mm1            \n\t" 
            "psllq $16, %%mm1             \n\t" 
            "pand %%mm5, %%mm1            \n\t" 
            "addl $8, %%edx               \n\t"
            "paddb %%mm1, %%mm0           \n\t"
            "cmpl %%eax, %%edx            \n\t" 
            "movq %%mm0, -8(%1," PDX ",)  \n\t" 
            "movq %%mm0, %%mm1            \n\t" 
            "jb sub_2lp                   \n\t"

            : "=c" (dummy_value_c),   
              "=D" (dummy_value_D),   
              "=d" (dummy_value_d),   
              "=a" (dummy_value_a)    

            : "0" (bpp),              
              "1" (row),              
              "2" (diff),             
              "3" (MMXLength)         

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1", "%mm5", "%mm6"    
            , "%mm7"
#endif
         );
      }
      break;  

      case 6:   
      {

         __asm__ __volatile__ (
            "add  " PCX ", %1             \n\t" 

            
            "movq -8(%1," PDX ",), %%mm1  \n\t"

         "sub_6lp:                        \n\t" 
            "psrlq $16, %%mm1             \n\t" 
                                                
            "movq (%1," PDX ",), %%mm0    \n\t"
            "paddb %%mm1, %%mm0           \n\t"

            
            "movq %%mm0, %%mm1            \n\t" 
            "psllq $48, %%mm1             \n\t" 
            "addl $8, %%edx               \n\t"
            "paddb %%mm1, %%mm0           \n\t"

            "cmpl %%eax, %%edx            \n\t" 
            "movq %%mm0, -8(%1," PDX ",)  \n\t" 
            "movq %%mm0, %%mm1            \n\t" 
            "jb sub_6lp                   \n\t"

            : "=c" (dummy_value_c),   
              "=D" (dummy_value_D),   
              "=d" (dummy_value_d),   
              "=a" (dummy_value_a)    

            : "0" (bpp),              
              "1" (row),              
              "2" (diff),             
              "3" (MMXLength)         

#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            : "%mm0", "%mm1"                    
#endif
         );
      }
      break;  

      case 8:
      {
         __asm__ __volatile__ (
            "add  " PCX ", %1             \n\t" 

            
            "movq -8(%1," PDX ",), %%mm7  \n\t"
            "movl %%eax, %%esi            \n\t" 
            "andl $0x0000003f, %%esi      \n\t" 

         "sub_8lp:                        \n\t"
            "movq (%1," PDX ",), %%mm0    \n\t" 
            "paddb %%mm7, %%mm0           \n\t"
            "movq 8(%1," PDX ",), %%mm1   \n\t" 
            "movq %%mm0, (%1," PDX ",)    \n\t" 

            
            
            
            

            "paddb %%mm0, %%mm1           \n\t"
            "movq 16(%1," PDX ",), %%mm2  \n\t" 
            "movq %%mm1, 8(%1," PDX ",)   \n\t" 
            "paddb %%mm1, %%mm2           \n\t"
            "movq 24(%1," PDX ",), %%mm3  \n\t" 
            "movq %%mm2, 16(%1," PDX ",)  \n\t" 
            "paddb %%mm2, %%mm3           \n\t"
            "movq 32(%1," PDX ",), %%mm4  \n\t" 
            "movq %%mm3, 24(%1," PDX ",)  \n\t" 
            "paddb %%mm3, %%mm4           \n\t"
            "movq 40(%1," PDX ",), %%mm5  \n\t" 
            "movq %%mm4, 32(%1," PDX ",)  \n\t" 
            "paddb %%mm4, %%mm5           \n\t"
            "movq 48(%1," PDX ",), %%mm6  \n\t" 
            "movq %%mm5, 40(%1," PDX ",)  \n\t" 
            "paddb %%mm5, %%mm6           \n\t"
            "movq 56(%1," PDX ",), %%mm7  \n\t" 
            "movq %%mm6, 48(%1," PDX ",)  \n\t" 
            "addl $64, %%edx              \n\t"
            "paddb %%mm6, %%mm7           \n\t"
            "cmpl %%esi, %%edx            \n\t" 
            "movq %%mm7, -8(%1," PDX ",)  \n\t" 
            "jb sub_8lp                   \n\t"

            "cmpl %%eax, %%edx            \n\t" 
            "jnb sub_8lt8                 \n\t"

         "sub_8lpA:                       \n\t"
            "movq (%1," PDX ",), %%mm0    \n\t"
            "addl $8, %%edx               \n\t"
            "paddb %%mm7, %%mm0           \n\t"
            "cmpl %%eax, %%edx            \n\t" 
            "movq %%mm0, -8(%1," PDX ",)  \n\t" 
            "movq %%mm0, %%mm7            \n\t" 
            "jb sub_8lpA                  \n\t" 
                                                
         "sub_8lt8:                       \n\t"

            : "=c" (dummy_value_c),   
              "=D" (dummy_value_D),   
              "=d" (dummy_value_d),   
              "=a" (dummy_value_a)    

            : "0" (bpp),              
              "1" (row),              
              "2" (diff),             
              "3" (MMXLength)         

            : "%esi"                            
#if defined(CLOBBER_MMX_REGS_SUPPORTED)
            , "%mm0", "%mm1", "%mm2", "%mm3"
            , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
         );
      }
      break;  

      default:                
      {
         
#if defined(PNG_DEBUG)
         png_debug(1, "Internal libpng logic error (GCC "
           "png_read_filter_row_mmx_sub())\n");
#endif
      }
      break;

   } 

   __asm__ __volatile__ (
      "cmpl %%edx, %%eax             \n\t"
      "jnb sub_end                   \n\t"

      "mov  %1, " PSI "              \n\t" 
      "add  " PCX ", %1              \n\t" 
      "xorl %%ecx, %%ecx             \n\t"

   "sub_lp2:                         \n\t"
      "movb (" PSI "," PAX ",), %%cl \n\t"
      "addb %%cl, (%1," PAX ",)      \n\t"
      "incl %%eax                    \n\t"
      "cmpl %%edx, %%eax             \n\t" 
      "jb sub_lp2                    \n\t"

   "sub_end:                         \n\t"
      "EMMS                          \n\t" 

      : "=c" (dummy_value_c),   
        "=D" (dummy_value_D),   
        "=a" (dummy_value_a),   
        "=d" (dummy_value_d)    

      : "0" (bpp),              
        "1" (row),              
        "2" (MMXLength),        
        "3" (FullLength)        

      : "%esi"                            
   );

} 

#endif 




#if defined(PNG_MMX_READ_FILTER_UP_SUPPORTED)



static void 
png_read_filter_row_mmx_up(png_row_infop row_info, png_bytep row,
                           png_bytep prev_row)
{
   unsigned len;        
   int dummy_value_d;   
   png_bytep dummy_value_S;
   png_bytep dummy_value_D;

   len = row_info->rowbytes;              

   __asm__ __volatile__ (
      SAVE_GOT_ebx

      "xorl %%ebx, %%ebx           \n\t"
      "xorl %%eax, %%eax           \n\t"

      
      
      "mov  %2, " PCX "            \n\t" 
      "add  $0x7, " PCX "          \n\t" 
      CLEAR_BOTTOM_3_BITS  PCX    "\n\t" 
      "sub  %2, " PCX "            \n\t" 
      "jz up_go                    \n\t" 

   "up_lp1:                        \n\t" 
      "movb (%2," PBX ",), %%al    \n\t"
      "addb (%1," PBX ",), %%al    \n\t"
      "incl %%ebx                  \n\t"
      "cmpl %%ecx, %%ebx           \n\t"
      "movb %%al, -1(%2," PBX ",)  \n\t" 
      "jb up_lp1                   \n\t" 

   "up_go:                         \n\t"
      "movl %%edx, %%ecx           \n\t"
      "subl %%ebx, %%edx           \n\t" 
      "andl $0x0000003f, %%edx     \n\t" 
      "subl %%edx, %%ecx           \n\t" 

      
      
   "up_loop:                       \n\t"
      "movq (%1," PBX ",), %%mm1   \n\t"
      "movq (%2," PBX ",), %%mm0   \n\t"
      "movq 8(%1," PBX ",), %%mm3  \n\t"
      "paddb %%mm1, %%mm0          \n\t"
      "movq 8(%2," PBX ",), %%mm2  \n\t"
      "movq %%mm0, (%2," PBX ",)   \n\t"
      "paddb %%mm3, %%mm2          \n\t"
      "movq 16(%1," PBX ",), %%mm5 \n\t"
      "movq %%mm2, 8(%2," PBX ",)  \n\t"
      "movq 16(%2," PBX ",), %%mm4 \n\t"
      "movq 24(%1," PBX ",), %%mm7 \n\t"
      "paddb %%mm5, %%mm4          \n\t"
      "movq 24(%2," PBX ",), %%mm6 \n\t"
      "movq %%mm4, 16(%2," PBX ",) \n\t"
      "paddb %%mm7, %%mm6          \n\t"
      "movq 32(%1," PBX ",), %%mm1 \n\t"
      "movq %%mm6, 24(%2," PBX ",) \n\t"
      "movq 32(%2," PBX ",), %%mm0 \n\t"
      "movq 40(%1," PBX ",), %%mm3 \n\t"
      "paddb %%mm1, %%mm0          \n\t"
      "movq 40(%2," PBX ",), %%mm2 \n\t"
      "movq %%mm0, 32(%2," PBX ",) \n\t"
      "paddb %%mm3, %%mm2          \n\t"
      "movq 48(%1," PBX ",), %%mm5 \n\t"
      "movq %%mm2, 40(%2," PBX ",) \n\t"
      "movq 48(%2," PBX ",), %%mm4 \n\t"
      "movq 56(%1," PBX ",), %%mm7 \n\t"
      "paddb %%mm5, %%mm4          \n\t"
      "movq 56(%2," PBX ",), %%mm6 \n\t"
      "movq %%mm4, 48(%2," PBX ",) \n\t"
      "addl $64, %%ebx             \n\t"
      "paddb %%mm7, %%mm6          \n\t"
      "cmpl %%ecx, %%ebx           \n\t"
      "movq %%mm6, -8(%2," PBX ",) \n\t" 
      "jb up_loop                  \n\t" 

      "cmpl $0, %%edx              \n\t" 
      "jz up_end                   \n\t"

      "cmpl $8, %%edx              \n\t" 
      "jb up_lt8                   \n\t" 

      "addl %%edx, %%ecx           \n\t"
      "andl $0x00000007, %%edx     \n\t" 
      "subl %%edx, %%ecx           \n\t" 
      "jz up_lt8                   \n\t"

   "up_lpA:                        \n\t" 
      "movq (%1," PBX ",), %%mm1   \n\t"
      "movq (%2," PBX ",), %%mm0   \n\t"
      "addl $8, %%ebx              \n\t"
      "paddb %%mm1, %%mm0          \n\t"
      "cmpl %%ecx, %%ebx           \n\t"
      "movq %%mm0, -8(%2," PBX ",) \n\t" 
      "jb up_lpA                   \n\t" 
      "cmpl $0, %%edx              \n\t" 
      "jz up_end                   \n\t"

   "up_lt8:                        \n\t"
      "xorl %%eax, %%eax           \n\t"
      "addl %%edx, %%ecx           \n\t" 

   "up_lp2:                        \n\t" 
      "movb (%2," PBX ",), %%al    \n\t"
      "addb (%1," PBX ",), %%al    \n\t"
      "incl %%ebx                  \n\t"
      "cmpl %%ecx, %%ebx           \n\t"
      "movb %%al, -1(%2," PBX ",)  \n\t" 
      "jb up_lp2                   \n\t" 

   "up_end:                        \n\t"
      "EMMS                        \n\t" 
      RESTORE_GOT_ebx

      : "=d" (dummy_value_d),   
        "=S" (dummy_value_S),   
        "=D" (dummy_value_D)    

      : "0" (len),              
        "1" (prev_row),         
        "2" (row)               

      : "%eax", "%ecx"                   
        _CLOBBER_GOT_ebx
#if defined(PNG_CLOBBER_MMX_REGS_SUPPORTED)
      , "%mm0", "%mm1", "%mm2", "%mm3"
      , "%mm4", "%mm5", "%mm6", "%mm7"
#endif
   );

} 

#endif 






void 
png_read_filter_row(png_structp png_ptr, png_row_infop row_info, png_bytep
   row, png_bytep prev_row, int filter)
{
#if defined(PNG_DEBUG)
   char filtname[10];
#endif

   if (_mmx_supported == 2) {
#if !defined(PNG_1_0_X)
       
       png_warning(png_ptr, "asm_flags may not have been initialized");
#endif
       png_mmx_support();
   }

#if defined(PNG_DEBUG)
   png_debug(1, "in png_read_filter_row (pnggccrd.c)\n");
   switch (filter)
   {
      case 0:
         png_snprintf(filtname, 10, "none");
         break;

      case 1:
         png_snprintf(filtname, 10, "sub-%s",
#ifdef PNG_MMX_READ_FILTER_SUB_SUPPORTED
#if !defined(PNG_1_0_X)
           ((png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_FILTER_SUB) &&
            (row_info->pixel_depth >= png_ptr->mmx_bitdepth_threshold) &&
            (row_info->rowbytes >= png_ptr->mmx_rowbytes_threshold))
#else
           _mmx_supported
#endif
           ? "MMX" :
#endif
           "C");
         break;

      case 2:
         png_snprintf(filtname, 10, "up-%s",
#ifdef PNG_MMX_READ_FILTER_UP_SUPPORTED
#if !defined(PNG_1_0_X)
           ((png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_FILTER_UP) &&
            (row_info->pixel_depth >= png_ptr->mmx_bitdepth_threshold) &&
            (row_info->rowbytes >= png_ptr->mmx_rowbytes_threshold))
#else
           _mmx_supported
#endif
           ? "MMX" :
#endif
           "C");
         break;

      case 3:
         png_snprintf(filtname, 10, "avg-%s",
#ifdef PNG_MMX_READ_FILTER_AVG_SUPPORTED
#if !defined(PNG_1_0_X)
           ((png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_FILTER_AVG) &&
            (row_info->pixel_depth >= png_ptr->mmx_bitdepth_threshold) &&
            (row_info->rowbytes >= png_ptr->mmx_rowbytes_threshold))
#else
           _mmx_supported
#endif
           ? "MMX" :
#endif
           "C");
         break;

      case 4:
         png_snprintf(filtname, 10, "paeth-%s",
#ifdef PNG_MMX_READ_FILTER_PAETH_SUPPORTED
#if defined(PNG_x86_64_USE_GOTPCREL) || defined(PNG_THREAD_UNSAFE_OK)
#if !defined(PNG_1_0_X)
           ((png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_FILTER_PAETH) &&
            (row_info->pixel_depth >= png_ptr->mmx_bitdepth_threshold) &&
            (row_info->rowbytes >= png_ptr->mmx_rowbytes_threshold))
#else
           _mmx_supported
#endif
           ? "MMX" :
#endif 
#endif
           "C");
         break;

      default:
         png_snprintf(filtname, 10, "unknown");
         break;
   }
   png_debug2(2, "row_number=%ld, %s, ", png_ptr->row_number, filtname);
   
   
   png_debug1(0, "row=%10p, ", row);
   png_debug2(0, "pixdepth=%d, bytes=%d, ", (int)row_info->pixel_depth,
      (int)((row_info->pixel_depth + 7) >> 3));
   png_debug1(0, "rowbytes=%ld\n", row_info->rowbytes);
#endif 

   switch (filter)
   {
      case PNG_FILTER_VALUE_NONE:
         break;

      case PNG_FILTER_VALUE_SUB:
#ifdef PNG_MMX_READ_FILTER_SUB_SUPPORTED
#if !defined(PNG_1_0_X)
         if ((png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_FILTER_SUB) &&
             (row_info->pixel_depth >= png_ptr->mmx_bitdepth_threshold) &&
             (row_info->rowbytes >= png_ptr->mmx_rowbytes_threshold))
#else
         if (_mmx_supported)
#endif
         {
            png_read_filter_row_mmx_sub(row_info, row);
         }
         else
#endif
         {
            png_uint_32 i;
            png_uint_32 istop = row_info->rowbytes;
            png_uint_32 bpp = (row_info->pixel_depth + 7) >> 3;
            png_bytep rp = row + bpp;
            png_bytep lp = row;

            for (i = bpp; i < istop; i++)
            {
               *rp = (png_byte)(((int)(*rp) + (int)(*lp++)) & 0xff);
               rp++;
            }
         }  
         break;

      case PNG_FILTER_VALUE_UP:
#ifdef PNG_MMX_READ_FILTER_UP_SUPPORTED
#if !defined(PNG_1_0_X)
         if ((png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_FILTER_UP) &&
             (row_info->pixel_depth >= png_ptr->mmx_bitdepth_threshold) &&
             (row_info->rowbytes >= png_ptr->mmx_rowbytes_threshold))
#else
         if (_mmx_supported)
#endif
         {
            png_read_filter_row_mmx_up(row_info, row, prev_row);
         }
          else
#endif
         {
            png_uint_32 i;
            png_uint_32 istop = row_info->rowbytes;
            png_bytep rp = row;
            png_bytep pp = prev_row;

            for (i = 0; i < istop; ++i)
            {
               *rp = (png_byte)(((int)(*rp) + (int)(*pp++)) & 0xff);
               rp++;
            }
         }  
         break;

      case PNG_FILTER_VALUE_AVG:
#ifdef PNG_MMX_READ_FILTER_AVG_SUPPORTED
#if !defined(PNG_1_0_X)
         if ((png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_FILTER_AVG) &&
             (row_info->pixel_depth >= png_ptr->mmx_bitdepth_threshold) &&
             (row_info->rowbytes >= png_ptr->mmx_rowbytes_threshold))
#else
         if (_mmx_supported)
#endif
         {
            png_read_filter_row_mmx_avg(row_info, row, prev_row);
         }
         else
#endif
         {
            png_uint_32 i;
            png_bytep rp = row;
            png_bytep pp = prev_row;
            png_bytep lp = row;
            png_uint_32 bpp = (row_info->pixel_depth + 7) >> 3;
            png_uint_32 istop = row_info->rowbytes - bpp;

            for (i = 0; i < bpp; i++)
            {
               *rp = (png_byte)(((int)(*rp) +
                  ((int)(*pp++) >> 1)) & 0xff);
               rp++;
            }

            for (i = 0; i < istop; i++)
            {
               *rp = (png_byte)(((int)(*rp) +
                  ((int)(*pp++ + *lp++) >> 1)) & 0xff);
               rp++;
            }
         }  
         break;

      case PNG_FILTER_VALUE_PAETH:
#ifdef PNG_MMX_READ_FILTER_PAETH_SUPPORTED
#if defined(PNG_x86_64_USE_GOTPCREL) || defined(PNG_THREAD_UNSAFE_OK)
#if !defined(PNG_1_0_X)
         if ((png_ptr->asm_flags & PNG_ASM_FLAG_MMX_READ_FILTER_PAETH) &&
             (row_info->pixel_depth >= png_ptr->mmx_bitdepth_threshold) &&
             (row_info->rowbytes >= png_ptr->mmx_rowbytes_threshold))
#else
         if (_mmx_supported)
#endif
         {
            png_read_filter_row_mmx_paeth(row_info, row, prev_row);
         }
         else
#endif 
#endif
         {
            png_uint_32 i;
            png_bytep rp = row;
            png_bytep pp = prev_row;
            png_bytep lp = row;
            png_bytep cp = prev_row;
            png_uint_32 bpp = (row_info->pixel_depth + 7) >> 3;
            png_uint_32 istop = row_info->rowbytes - bpp;

            for (i = 0; i < bpp; i++)
            {
               *rp = (png_byte)(((int)(*rp) + (int)(*pp++)) & 0xff);
               rp++;
            }

            for (i = 0; i < istop; i++)   
            {
               int a, b, c, pa, pb, pc, p;

               a = *lp++;
               b = *pp++;
               c = *cp++;

               p = b - c;
               pc = a - c;

#if defined(PNG_USE_ABS)
               pa = abs(p);
               pb = abs(pc);
               pc = abs(p + pc);
#else
               pa = p < 0 ? -p : p;
               pb = pc < 0 ? -pc : pc;
               pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#endif


               p = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;

               *rp = (png_byte)(((int)(*rp) + p) & 0xff);
               rp++;
            }
         }  
         break;

      default:
         png_warning(png_ptr, "Ignoring bad row-filter type");
         *row=0;
         break;
   }
}

#endif 


#endif 
#endif 
