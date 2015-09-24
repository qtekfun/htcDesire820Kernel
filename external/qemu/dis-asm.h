/* Interface between the opcode library and its callers.
   Written by Cygnus Support, 1993.

   The opcode library (libopcodes.a) provides instruction decoders for
   a large variety of instruction sets, callable with an identical
   interface, for making instruction-processing programs more independent
   of the instruction set being processed.  */

#ifndef DIS_ASM_H
#define DIS_ASM_H

#include "qemu-common.h"

typedef void *PTR;
typedef uint64_t bfd_vma;
typedef int64_t bfd_signed_vma;
typedef uint8_t bfd_byte;
#define sprintf_vma(s,x) sprintf (s, "%0" PRIx64, x)
#define snprintf_vma(s,ss,x) snprintf (s, ss, "%0" PRIx64, x)

#define BFD64

enum bfd_flavour {
  bfd_target_unknown_flavour,
  bfd_target_aout_flavour,
  bfd_target_coff_flavour,
  bfd_target_ecoff_flavour,
  bfd_target_elf_flavour,
  bfd_target_ieee_flavour,
  bfd_target_nlm_flavour,
  bfd_target_oasys_flavour,
  bfd_target_tekhex_flavour,
  bfd_target_srec_flavour,
  bfd_target_ihex_flavour,
  bfd_target_som_flavour,
  bfd_target_os9k_flavour,
  bfd_target_versados_flavour,
  bfd_target_msdos_flavour,
  bfd_target_evax_flavour
};

enum bfd_endian { BFD_ENDIAN_BIG, BFD_ENDIAN_LITTLE, BFD_ENDIAN_UNKNOWN };

enum bfd_architecture
{
  bfd_arch_unknown,    
  bfd_arch_obscure,    
  bfd_arch_m68k,       
#define bfd_mach_m68000 1
#define bfd_mach_m68008 2
#define bfd_mach_m68010 3
#define bfd_mach_m68020 4
#define bfd_mach_m68030 5
#define bfd_mach_m68040 6
#define bfd_mach_m68060 7
#define bfd_mach_cpu32  8
#define bfd_mach_mcf5200  9
#define bfd_mach_mcf5206e 10
#define bfd_mach_mcf5307  11
#define bfd_mach_mcf5407  12
#define bfd_mach_mcf528x  13
#define bfd_mach_mcfv4e   14
#define bfd_mach_mcf521x   15
#define bfd_mach_mcf5249   16
#define bfd_mach_mcf547x   17
#define bfd_mach_mcf548x   18
  bfd_arch_vax,        
  bfd_arch_i960,       

#define bfd_mach_i960_core      1
#define bfd_mach_i960_ka_sa     2
#define bfd_mach_i960_kb_sb     3
#define bfd_mach_i960_mc        4
#define bfd_mach_i960_xa        5
#define bfd_mach_i960_ca        6
#define bfd_mach_i960_jx        7
#define bfd_mach_i960_hx        8

  bfd_arch_a29k,       
  bfd_arch_sparc,      
#define bfd_mach_sparc                 1
#define bfd_mach_sparc_sparclet        2
#define bfd_mach_sparc_sparclite       3
#define bfd_mach_sparc_v8plus          4
#define bfd_mach_sparc_v8plusa         5 
#define bfd_mach_sparc_sparclite_le    6
#define bfd_mach_sparc_v9              7
#define bfd_mach_sparc_v9a             8 
#define bfd_mach_sparc_v8plusb         9 
#define bfd_mach_sparc_v9b             10 
#define bfd_mach_sparc_v9_p(mach) \
  ((mach) >= bfd_mach_sparc_v8plus && (mach) <= bfd_mach_sparc_v9b \
   && (mach) != bfd_mach_sparc_sparclite_le)
  bfd_arch_mips,       
#define bfd_mach_mips3000              3000
#define bfd_mach_mips3900              3900
#define bfd_mach_mips4000              4000
#define bfd_mach_mips4010              4010
#define bfd_mach_mips4100              4100
#define bfd_mach_mips4300              4300
#define bfd_mach_mips4400              4400
#define bfd_mach_mips4600              4600
#define bfd_mach_mips4650              4650
#define bfd_mach_mips5000              5000
#define bfd_mach_mips6000              6000
#define bfd_mach_mips8000              8000
#define bfd_mach_mips10000             10000
#define bfd_mach_mips16                16
  bfd_arch_i386,       
#define bfd_mach_i386_i386 0
#define bfd_mach_i386_i8086 1
#define bfd_mach_i386_i386_intel_syntax 2
#define bfd_mach_x86_64 3
#define bfd_mach_x86_64_intel_syntax 4
  bfd_arch_we32k,      
  bfd_arch_tahoe,      
  bfd_arch_i860,       
  bfd_arch_romp,       
  bfd_arch_alliant,    
  bfd_arch_convex,     
  bfd_arch_m88k,       
  bfd_arch_pyramid,    
  bfd_arch_h8300,      
#define bfd_mach_h8300   1
#define bfd_mach_h8300h  2
#define bfd_mach_h8300s  3
  bfd_arch_powerpc,    
#define bfd_mach_ppc           0
#define bfd_mach_ppc64         1
#define bfd_mach_ppc_403       403
#define bfd_mach_ppc_403gc     4030
#define bfd_mach_ppc_e500      500
#define bfd_mach_ppc_505       505
#define bfd_mach_ppc_601       601
#define bfd_mach_ppc_602       602
#define bfd_mach_ppc_603       603
#define bfd_mach_ppc_ec603e    6031
#define bfd_mach_ppc_604       604
#define bfd_mach_ppc_620       620
#define bfd_mach_ppc_630       630
#define bfd_mach_ppc_750       750
#define bfd_mach_ppc_860       860
#define bfd_mach_ppc_a35       35
#define bfd_mach_ppc_rs64ii    642
#define bfd_mach_ppc_rs64iii   643
#define bfd_mach_ppc_7400      7400
  bfd_arch_rs6000,     
  bfd_arch_hppa,       
#define bfd_mach_hppa10        10
#define bfd_mach_hppa11        11
#define bfd_mach_hppa20        20
#define bfd_mach_hppa20w       25
  bfd_arch_d10v,       
  bfd_arch_z8k,        
#define bfd_mach_z8001         1
#define bfd_mach_z8002         2
  bfd_arch_h8500,      
  bfd_arch_sh,         
#define bfd_mach_sh            1
#define bfd_mach_sh2        0x20
#define bfd_mach_sh_dsp     0x2d
#define bfd_mach_sh2a       0x2a
#define bfd_mach_sh2a_nofpu 0x2b
#define bfd_mach_sh2e       0x2e
#define bfd_mach_sh3        0x30
#define bfd_mach_sh3_nommu  0x31
#define bfd_mach_sh3_dsp    0x3d
#define bfd_mach_sh3e       0x3e
#define bfd_mach_sh4        0x40
#define bfd_mach_sh4_nofpu  0x41
#define bfd_mach_sh4_nommu_nofpu  0x42
#define bfd_mach_sh4a       0x4a
#define bfd_mach_sh4a_nofpu 0x4b
#define bfd_mach_sh4al_dsp  0x4d
#define bfd_mach_sh5        0x50
  bfd_arch_alpha,      
#define bfd_mach_alpha 1
  bfd_arch_arm,        
#define bfd_mach_arm_unknown	0
#define bfd_mach_arm_2		1
#define bfd_mach_arm_2a		2
#define bfd_mach_arm_3		3
#define bfd_mach_arm_3M 	4
#define bfd_mach_arm_4 		5
#define bfd_mach_arm_4T 	6
#define bfd_mach_arm_5 		7
#define bfd_mach_arm_5T		8
#define bfd_mach_arm_5TE	9
#define bfd_mach_arm_XScale	10
#define bfd_mach_arm_ep9312	11
#define bfd_mach_arm_iWMMXt	12
#define bfd_mach_arm_iWMMXt2	13
  bfd_arch_ns32k,      
  bfd_arch_w65,        
  bfd_arch_tic30,      
  bfd_arch_v850,       
#define bfd_mach_v850          0
  bfd_arch_arc,        
#define bfd_mach_arc_base 0
  bfd_arch_m32r,       
#define bfd_mach_m32r          0  
  bfd_arch_mn10200,    
  bfd_arch_mn10300,    
  bfd_arch_cris,       
#define bfd_mach_cris_v0_v10   255
#define bfd_mach_cris_v32      32
#define bfd_mach_cris_v10_v32  1032
  bfd_arch_microblaze, 
  bfd_arch_ia64,      
#define bfd_mach_ia64_elf64    64
#define bfd_mach_ia64_elf32    32
  bfd_arch_last
  };
#define bfd_mach_s390_31 31
#define bfd_mach_s390_64 64

typedef struct symbol_cache_entry
{
    const char *name;
    union
    {
        PTR p;
        bfd_vma i;
    } udata;
} asymbol;

enum dis_insn_type {
  dis_noninsn,			
  dis_nonbranch,		
  dis_branch,			
  dis_condbranch,		
  dis_jsr,			
  dis_condjsr,			
  dis_dref,			
  dis_dref2			
};


typedef struct disassemble_info {
  fprintf_function fprintf_func;
  FILE *stream;
  PTR application_data;

  
  enum bfd_flavour flavour;
  
  enum bfd_architecture arch;
  
  unsigned long mach;
  
  enum bfd_endian endian;

  asymbol **symbols;
  
  int num_symbols;

  unsigned long flags;
#define INSN_HAS_RELOC	0x80000000
  PTR private_data;

  int (*read_memory_func)
    (bfd_vma memaddr, bfd_byte *myaddr, int length,
	     struct disassemble_info *info);

  void (*memory_error_func)
    (int status, bfd_vma memaddr, struct disassemble_info *info);

  
  void (*print_address_func)
    (bfd_vma addr, struct disassemble_info *info);

  int (* symbol_at_address_func)
    (bfd_vma addr, struct disassemble_info * info);

  
  bfd_byte *buffer;
  bfd_vma buffer_vma;
  int buffer_length;

  int bytes_per_line;

  
  
  int bytes_per_chunk;
  enum bfd_endian display_endian;


  char insn_info_valid;		
  char branch_delay_insns;	
  char data_size;		
  enum dis_insn_type insn_type;	
  bfd_vma target;		
  bfd_vma target2;		

  
  char * disassembler_options;

} disassemble_info;


typedef int (*disassembler_ftype) (bfd_vma, disassemble_info *);

int print_insn_big_mips         (bfd_vma, disassemble_info*);
int print_insn_little_mips      (bfd_vma, disassemble_info*);
int print_insn_i386             (bfd_vma, disassemble_info*);
int print_insn_m68k             (bfd_vma, disassemble_info*);
int print_insn_z8001            (bfd_vma, disassemble_info*);
int print_insn_z8002            (bfd_vma, disassemble_info*);
int print_insn_h8300            (bfd_vma, disassemble_info*);
int print_insn_h8300h           (bfd_vma, disassemble_info*);
int print_insn_h8300s           (bfd_vma, disassemble_info*);
int print_insn_h8500            (bfd_vma, disassemble_info*);
int print_insn_alpha            (bfd_vma, disassemble_info*);
disassembler_ftype arc_get_disassembler (int, int);
int print_insn_arm              (bfd_vma, disassemble_info*);
int print_insn_sparc            (bfd_vma, disassemble_info*);
int print_insn_big_a29k         (bfd_vma, disassemble_info*);
int print_insn_little_a29k      (bfd_vma, disassemble_info*);
int print_insn_i960             (bfd_vma, disassemble_info*);
int print_insn_sh               (bfd_vma, disassemble_info*);
int print_insn_shl              (bfd_vma, disassemble_info*);
int print_insn_hppa             (bfd_vma, disassemble_info*);
int print_insn_m32r             (bfd_vma, disassemble_info*);
int print_insn_m88k             (bfd_vma, disassemble_info*);
int print_insn_mn10200          (bfd_vma, disassemble_info*);
int print_insn_mn10300          (bfd_vma, disassemble_info*);
int print_insn_ns32k            (bfd_vma, disassemble_info*);
int print_insn_big_powerpc      (bfd_vma, disassemble_info*);
int print_insn_little_powerpc   (bfd_vma, disassemble_info*);
int print_insn_rs6000           (bfd_vma, disassemble_info*);
int print_insn_w65              (bfd_vma, disassemble_info*);
int print_insn_d10v             (bfd_vma, disassemble_info*);
int print_insn_v850             (bfd_vma, disassemble_info*);
int print_insn_tic30            (bfd_vma, disassemble_info*);
int print_insn_ppc              (bfd_vma, disassemble_info*);
int print_insn_s390             (bfd_vma, disassemble_info*);
int print_insn_crisv32          (bfd_vma, disassemble_info*);
int print_insn_crisv10          (bfd_vma, disassemble_info*);
int print_insn_microblaze       (bfd_vma, disassemble_info*);
int print_insn_ia64             (bfd_vma, disassemble_info*);

#if 0
disassembler_ftype disassembler(bfd *);
#endif



int buffer_read_memory(bfd_vma, bfd_byte *, int, struct disassemble_info *);

void perror_memory(int, bfd_vma, struct disassemble_info *);


void generic_print_address(bfd_vma, struct disassemble_info *);

int generic_symbol_at_address(bfd_vma, struct disassemble_info *);

#define INIT_DISASSEMBLE_INFO(INFO, STREAM, FPRINTF_FUNC) \
  (INFO).flavour = bfd_target_unknown_flavour, \
  (INFO).arch = bfd_arch_unknown, \
  (INFO).mach = 0, \
  (INFO).endian = BFD_ENDIAN_UNKNOWN, \
  INIT_DISASSEMBLE_INFO_NO_ARCH(INFO, STREAM, FPRINTF_FUNC)


#define INIT_DISASSEMBLE_INFO_NO_ARCH(INFO, STREAM, FPRINTF_FUNC) \
  (INFO).fprintf_func = (FPRINTF_FUNC), \
  (INFO).stream = (STREAM), \
  (INFO).symbols = NULL, \
  (INFO).num_symbols = 0, \
  (INFO).private_data = NULL, \
  (INFO).buffer = NULL, \
  (INFO).buffer_vma = 0, \
  (INFO).buffer_length = 0, \
  (INFO).read_memory_func = buffer_read_memory, \
  (INFO).memory_error_func = perror_memory, \
  (INFO).print_address_func = generic_print_address, \
  (INFO).symbol_at_address_func = generic_symbol_at_address, \
  (INFO).flags = 0, \
  (INFO).bytes_per_line = 0, \
  (INFO).bytes_per_chunk = 0, \
  (INFO).display_endian = BFD_ENDIAN_UNKNOWN, \
  (INFO).disassembler_options = NULL, \
  (INFO).insn_info_valid = 0

#define _(x) x
#define ATTRIBUTE_UNUSED __attribute__((unused))


bfd_vma bfd_getl64 (const bfd_byte *addr);
bfd_vma bfd_getl32 (const bfd_byte *addr);
bfd_vma bfd_getb32 (const bfd_byte *addr);
bfd_vma bfd_getl16 (const bfd_byte *addr);
bfd_vma bfd_getb16 (const bfd_byte *addr);
typedef bool bfd_boolean;

#endif 
