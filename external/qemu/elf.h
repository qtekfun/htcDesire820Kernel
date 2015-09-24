#ifndef _QEMU_ELF_H
#define _QEMU_ELF_H

#include <inttypes.h>

typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf32_Word;

typedef uint64_t Elf64_Addr;
typedef uint16_t Elf64_Half;
typedef int16_t	 Elf64_SHalf;
typedef uint64_t Elf64_Off;
typedef int32_t	 Elf64_Sword;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef int64_t  Elf64_Sxword;

#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff
#define PT_MIPS_REGINFO		0x70000000
#define PT_MIPS_OPTIONS		0x70000001

#define EF_MIPS_ARCH_1		0x00000000	
#define EF_MIPS_ARCH_2		0x10000000	
#define EF_MIPS_ARCH_3		0x20000000	
#define EF_MIPS_ARCH_4		0x30000000	
#define EF_MIPS_ARCH_5		0x40000000	
#define EF_MIPS_ARCH_32		0x50000000	
#define EF_MIPS_ARCH_64		0x60000000	

#define EF_MIPS_ABI_O32		0x00001000	
#define EF_MIPS_ABI_O64		0x00002000	

#define EF_MIPS_NOREORDER 0x00000001
#define EF_MIPS_PIC       0x00000002
#define EF_MIPS_CPIC      0x00000004
#define EF_MIPS_ABI2		0x00000020
#define EF_MIPS_OPTIONS_FIRST	0x00000080
#define EF_MIPS_32BITMODE	0x00000100
#define EF_MIPS_ABI		0x0000f000
#define EF_MIPS_ARCH      0xf0000000

#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

#define EM_NONE  0
#define EM_M32   1
#define EM_SPARC 2
#define EM_386   3
#define EM_68K   4
#define EM_88K   5
#define EM_486   6   
#define EM_860   7

#define EM_MIPS		8	

#define EM_MIPS_RS4_BE 10	

#define EM_PARISC      15	

#define EM_SPARC32PLUS 18	

#define EM_PPC	       20	
#define EM_PPC64       21       

#define EM_ARM		40		

#define EM_SH	       42	

#define EM_SPARCV9     43	

#define EM_IA_64	50	

#define EM_X86_64	62	

#define EM_S390		22	

#define EM_CRIS         76      

#define EM_V850		87	

#define EM_H8_300H      47      
#define EM_H8S          48      
#define EM_LATTICEMICO32 138    

#define EM_UNICORE32    110     

#define EM_ALPHA	0x9026

#define EM_CYGNUS_V850	0x9080

#define EM_S390_OLD     0xA390

#define EM_MICROBLAZE      189
#define EM_MICROBLAZE_OLD  0xBAAB

#define DT_NULL		0
#define DT_NEEDED	1
#define DT_PLTRELSZ	2
#define DT_PLTGOT	3
#define DT_HASH		4
#define DT_STRTAB	5
#define DT_SYMTAB	6
#define DT_RELA		7
#define DT_RELASZ	8
#define DT_RELAENT	9
#define DT_STRSZ	10
#define DT_SYMENT	11
#define DT_INIT		12
#define DT_FINI		13
#define DT_SONAME	14
#define DT_RPATH 	15
#define DT_SYMBOLIC	16
#define DT_REL	        17
#define DT_RELSZ	18
#define DT_RELENT	19
#define DT_PLTREL	20
#define DT_DEBUG	21
#define DT_TEXTREL	22
#define DT_JMPREL	23
#define DT_BINDNOW	24
#define DT_INIT_ARRAY	25
#define DT_FINI_ARRAY	26
#define DT_INIT_ARRAYSZ	27
#define DT_FINI_ARRAYSZ	28
#define DT_RUNPATH	29
#define DT_FLAGS	30
#define DT_LOOS		0x6000000d
#define DT_HIOS		0x6ffff000
#define DT_LOPROC	0x70000000
#define DT_HIPROC	0x7fffffff

#define DT_VALRNGLO	0x6ffffd00
#define DT_VALRNGHI	0x6ffffdff

#define DT_ADDRRNGLO	0x6ffffe00
#define DT_ADDRRNGHI	0x6ffffeff

#define	DT_VERSYM	0x6ffffff0
#define DT_RELACOUNT	0x6ffffff9
#define DT_RELCOUNT	0x6ffffffa
#define DT_FLAGS_1	0x6ffffffb
#define DT_VERDEF	0x6ffffffc
#define DT_VERDEFNUM	0x6ffffffd
#define DT_VERNEED	0x6ffffffe
#define DT_VERNEEDNUM	0x6fffffff

#define DT_MIPS_RLD_VERSION	0x70000001
#define DT_MIPS_TIME_STAMP	0x70000002
#define DT_MIPS_ICHECKSUM	0x70000003
#define DT_MIPS_IVERSION	0x70000004
#define DT_MIPS_FLAGS		0x70000005
  #define RHF_NONE		  0
  #define RHF_HARDWAY		  1
  #define RHF_NOTPOT		  2
#define DT_MIPS_BASE_ADDRESS	0x70000006
#define DT_MIPS_CONFLICT	0x70000008
#define DT_MIPS_LIBLIST		0x70000009
#define DT_MIPS_LOCAL_GOTNO	0x7000000a
#define DT_MIPS_CONFLICTNO	0x7000000b
#define DT_MIPS_LIBLISTNO	0x70000010
#define DT_MIPS_SYMTABNO	0x70000011
#define DT_MIPS_UNREFEXTNO	0x70000012
#define DT_MIPS_GOTSYM		0x70000013
#define DT_MIPS_HIPAGENO	0x70000014
#define DT_MIPS_RLD_MAP		0x70000016

#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4

#define ELF_ST_BIND(x)		((x) >> 4)
#define ELF_ST_TYPE(x)		(((unsigned int) x) & 0xf)
#define ELF32_ST_BIND(x)	ELF_ST_BIND(x)
#define ELF32_ST_TYPE(x)	ELF_ST_TYPE(x)
#define ELF64_ST_BIND(x)	ELF_ST_BIND(x)
#define ELF64_ST_TYPE(x)	ELF_ST_TYPE(x)

#define AT_NULL   0	
#define AT_IGNORE 1	
#define AT_EXECFD 2	
#define AT_PHDR   3	
#define AT_PHENT  4	
#define AT_PHNUM  5	
#define AT_PAGESZ 6	
#define AT_BASE   7	
#define AT_FLAGS  8	
#define AT_ENTRY  9	
#define AT_NOTELF 10	
#define AT_UID    11	
#define AT_EUID   12	
#define AT_GID    13	
#define AT_EGID   14	
#define AT_PLATFORM 15  
#define AT_HWCAP  16    
#define AT_CLKTCK 17	
#define AT_FPUCW  18	
#define AT_DCACHEBSIZE	19	
#define AT_ICACHEBSIZE	20	
#define AT_UCACHEBSIZE	21	
#define AT_IGNOREPPC	22	
#define AT_SECURE	23	
#define AT_BASE_PLATFORM 24	
#define AT_RANDOM	25	
#define AT_EXECFN	31	
#define AT_SYSINFO	32	
#define AT_SYSINFO_EHDR	33	
#define AT_L1I_CACHESHAPE 34	
#define AT_L1D_CACHESHAPE 35	
#define AT_L2_CACHESHAPE  36	
#define AT_L3_CACHESHAPE  37	

typedef struct dynamic{
  Elf32_Sword d_tag;
  union{
    Elf32_Sword	d_val;
    Elf32_Addr	d_ptr;
  } d_un;
} Elf32_Dyn;

typedef struct {
  Elf64_Sxword d_tag;		
  union {
    Elf64_Xword d_val;
    Elf64_Addr d_ptr;
  } d_un;
} Elf64_Dyn;

#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)

#define ELF64_R_SYM(i)			((i) >> 32)
#define ELF64_R_TYPE(i)			((i) & 0xffffffff)
#define ELF64_R_TYPE_DATA(i)            (((ELF64_R_TYPE(i) >> 8) ^ 0x00800000) - 0x00800000)

#define R_386_NONE	0
#define R_386_32	1
#define R_386_PC32	2
#define R_386_GOT32	3
#define R_386_PLT32	4
#define R_386_COPY	5
#define R_386_GLOB_DAT	6
#define R_386_JMP_SLOT	7
#define R_386_RELATIVE	8
#define R_386_GOTOFF	9
#define R_386_GOTPC	10
#define R_386_NUM	11
#define R_386_PC8	23

#define R_MIPS_NONE		0
#define R_MIPS_16		1
#define R_MIPS_32		2
#define R_MIPS_REL32		3
#define R_MIPS_26		4
#define R_MIPS_HI16		5
#define R_MIPS_LO16		6
#define R_MIPS_GPREL16		7
#define R_MIPS_LITERAL		8
#define R_MIPS_GOT16		9
#define R_MIPS_PC16		10
#define R_MIPS_CALL16		11
#define R_MIPS_GPREL32		12
#define R_MIPS_UNUSED1		13
#define R_MIPS_UNUSED2		14
#define R_MIPS_UNUSED3		15
#define R_MIPS_SHIFT5		16
#define R_MIPS_SHIFT6		17
#define R_MIPS_64		18
#define R_MIPS_GOT_DISP		19
#define R_MIPS_GOT_PAGE		20
#define R_MIPS_GOT_OFST		21
#define R_MIPS_GOTHI16		22
#define R_MIPS_GOTLO16		23
#define R_MIPS_SUB		24
#define R_MIPS_INSERT_A		25
#define R_MIPS_INSERT_B		26
#define R_MIPS_DELETE		27
#define R_MIPS_HIGHER		28
#define R_MIPS_HIGHEST		29
#define R_MIPS_CALLHI16		30
#define R_MIPS_CALLLO16		31
#define R_MIPS_LOVENDOR		100
#define R_MIPS_HIVENDOR		127


#define	R_SPARC_NONE		0
#define	R_SPARC_8		1
#define	R_SPARC_16		2
#define	R_SPARC_32		3
#define	R_SPARC_DISP8		4
#define	R_SPARC_DISP16		5
#define	R_SPARC_DISP32		6
#define	R_SPARC_WDISP30		7
#define	R_SPARC_WDISP22		8
#define	R_SPARC_HI22		9
#define	R_SPARC_22		10
#define	R_SPARC_13		11
#define	R_SPARC_LO10		12
#define	R_SPARC_GOT10		13
#define	R_SPARC_GOT13		14
#define	R_SPARC_GOT22		15
#define	R_SPARC_PC10		16
#define	R_SPARC_PC22		17
#define	R_SPARC_WPLT30		18
#define	R_SPARC_COPY		19
#define	R_SPARC_GLOB_DAT	20
#define	R_SPARC_JMP_SLOT	21
#define	R_SPARC_RELATIVE	22
#define	R_SPARC_UA32		23
#define R_SPARC_PLT32		24
#define R_SPARC_HIPLT22		25
#define R_SPARC_LOPLT10		26
#define R_SPARC_PCPLT32		27
#define R_SPARC_PCPLT22		28
#define R_SPARC_PCPLT10		29
#define R_SPARC_10		30
#define R_SPARC_11		31
#define R_SPARC_64		32
#define R_SPARC_OLO10           33
#define R_SPARC_HH22            34
#define R_SPARC_HM10            35
#define R_SPARC_LM22            36
#define R_SPARC_WDISP16		40
#define R_SPARC_WDISP19		41
#define R_SPARC_7		43
#define R_SPARC_5		44
#define R_SPARC_6		45


#define HWCAP_SPARC_FLUSH       1    
#define HWCAP_SPARC_STBAR       2
#define HWCAP_SPARC_SWAP        4
#define HWCAP_SPARC_MULDIV      8
#define HWCAP_SPARC_V9		16
#define HWCAP_SPARC_ULTRA3	32

#define R_68K_NONE	0
#define R_68K_32	1
#define R_68K_16	2
#define R_68K_8		3
#define R_68K_PC32	4
#define R_68K_PC16	5
#define R_68K_PC8	6
#define R_68K_GOT32	7
#define R_68K_GOT16	8
#define R_68K_GOT8	9
#define R_68K_GOT32O	10
#define R_68K_GOT16O	11
#define R_68K_GOT8O	12
#define R_68K_PLT32	13
#define R_68K_PLT16	14
#define R_68K_PLT8	15
#define R_68K_PLT32O	16
#define R_68K_PLT16O	17
#define R_68K_PLT8O	18
#define R_68K_COPY	19
#define R_68K_GLOB_DAT	20
#define R_68K_JMP_SLOT	21
#define R_68K_RELATIVE	22

#define R_ALPHA_NONE            0       
#define R_ALPHA_REFLONG         1       
#define R_ALPHA_REFQUAD         2       
#define R_ALPHA_GPREL32         3       
#define R_ALPHA_LITERAL         4       
#define R_ALPHA_LITUSE          5       
#define R_ALPHA_GPDISP          6       
#define R_ALPHA_BRADDR          7       
#define R_ALPHA_HINT            8       
#define R_ALPHA_SREL16          9       
#define R_ALPHA_SREL32          10      
#define R_ALPHA_SREL64          11      
#define R_ALPHA_GPRELHIGH       17      
#define R_ALPHA_GPRELLOW        18      
#define R_ALPHA_GPREL16         19      
#define R_ALPHA_COPY            24      
#define R_ALPHA_GLOB_DAT        25      
#define R_ALPHA_JMP_SLOT        26      
#define R_ALPHA_RELATIVE        27      
#define R_ALPHA_BRSGP		28
#define R_ALPHA_TLSGD           29
#define R_ALPHA_TLS_LDM         30
#define R_ALPHA_DTPMOD64        31
#define R_ALPHA_GOTDTPREL       32
#define R_ALPHA_DTPREL64        33
#define R_ALPHA_DTPRELHI        34
#define R_ALPHA_DTPRELLO        35
#define R_ALPHA_DTPREL16        36
#define R_ALPHA_GOTTPREL        37
#define R_ALPHA_TPREL64         38
#define R_ALPHA_TPRELHI         39
#define R_ALPHA_TPRELLO         40
#define R_ALPHA_TPREL16         41

#define SHF_ALPHA_GPREL		0x10000000


#define R_PPC_NONE		0
#define R_PPC_ADDR32		1	
#define R_PPC_ADDR24		2	
#define R_PPC_ADDR16		3	
#define R_PPC_ADDR16_LO		4	
#define R_PPC_ADDR16_HI		5	
#define R_PPC_ADDR16_HA		6	
#define R_PPC_ADDR14		7	
#define R_PPC_ADDR14_BRTAKEN	8
#define R_PPC_ADDR14_BRNTAKEN	9
#define R_PPC_REL24		10	
#define R_PPC_REL14		11	
#define R_PPC_REL14_BRTAKEN	12
#define R_PPC_REL14_BRNTAKEN	13
#define R_PPC_GOT16		14
#define R_PPC_GOT16_LO		15
#define R_PPC_GOT16_HI		16
#define R_PPC_GOT16_HA		17
#define R_PPC_PLTREL24		18
#define R_PPC_COPY		19
#define R_PPC_GLOB_DAT		20
#define R_PPC_JMP_SLOT		21
#define R_PPC_RELATIVE		22
#define R_PPC_LOCAL24PC		23
#define R_PPC_UADDR32		24
#define R_PPC_UADDR16		25
#define R_PPC_REL32		26
#define R_PPC_PLT32		27
#define R_PPC_PLTREL32		28
#define R_PPC_PLT16_LO		29
#define R_PPC_PLT16_HI		30
#define R_PPC_PLT16_HA		31
#define R_PPC_SDAREL16		32
#define R_PPC_SECTOFF		33
#define R_PPC_SECTOFF_LO	34
#define R_PPC_SECTOFF_HI	35
#define R_PPC_SECTOFF_HA	36
#ifndef R_PPC_NUM
#define R_PPC_NUM		37
#endif


#define EF_ARM_RELEXEC     0x01
#define EF_ARM_HASENTRY    0x02
#define EF_ARM_INTERWORK   0x04
#define EF_ARM_APCS_26     0x08
#define EF_ARM_APCS_FLOAT  0x10
#define EF_ARM_PIC         0x20
#define EF_ALIGN8          0x40		
#define EF_NEW_ABI         0x80
#define EF_OLD_ABI         0x100

#define STT_ARM_TFUNC      0xd

#define SHF_ARM_ENTRYSECT  0x10000000   
#define SHF_ARM_COMDEF     0x80000000   

#define PF_ARM_SB          0x10000000   

#define R_ARM_NONE		0	
#define R_ARM_PC24		1	
#define R_ARM_ABS32		2	
#define R_ARM_REL32		3	
#define R_ARM_PC13		4
#define R_ARM_ABS16		5	
#define R_ARM_ABS12		6	
#define R_ARM_THM_ABS5		7
#define R_ARM_ABS8		8	
#define R_ARM_SBREL32		9
#define R_ARM_THM_PC22		10
#define R_ARM_THM_PC8		11
#define R_ARM_AMP_VCALL9	12
#define R_ARM_SWI24		13
#define R_ARM_THM_SWI8		14
#define R_ARM_XPC25		15
#define R_ARM_THM_XPC22		16
#define R_ARM_COPY		20	
#define R_ARM_GLOB_DAT		21	
#define R_ARM_JUMP_SLOT		22	
#define R_ARM_RELATIVE		23	
#define R_ARM_GOTOFF		24	
#define R_ARM_GOTPC		25	
#define R_ARM_GOT32		26	
#define R_ARM_PLT32		27	
#define R_ARM_CALL              28
#define R_ARM_JUMP24            29
#define R_ARM_GNU_VTENTRY	100
#define R_ARM_GNU_VTINHERIT	101
#define R_ARM_THM_PC11		102	
#define R_ARM_THM_PC9		103	
#define R_ARM_RXPC25		249
#define R_ARM_RSBREL32		250
#define R_ARM_THM_RPC22		251
#define R_ARM_RREL32		252
#define R_ARM_RABS22		253
#define R_ARM_RPC24		254
#define R_ARM_RBASE		255
#define R_ARM_NUM		256

#define R_390_NONE		0	
#define R_390_8			1	
#define R_390_12		2	
#define R_390_16		3	
#define R_390_32		4	
#define R_390_PC32		5	
#define R_390_GOT12		6	
#define R_390_GOT32		7	
#define R_390_PLT32		8	
#define R_390_COPY		9	
#define R_390_GLOB_DAT		10	
#define R_390_JMP_SLOT		11	
#define R_390_RELATIVE		12	
#define R_390_GOTOFF32		13	
#define R_390_GOTPC		14	
#define R_390_GOT16		15	
#define R_390_PC16		16	
#define R_390_PC16DBL		17	
#define R_390_PLT16DBL		18	
#define R_390_PC32DBL		19	
#define R_390_PLT32DBL		20	
#define R_390_GOTPCDBL		21	
#define R_390_64		22	
#define R_390_PC64		23	
#define R_390_GOT64		24	
#define R_390_PLT64		25	
#define R_390_GOTENT		26	
#define R_390_GOTOFF16		27	
#define R_390_GOTOFF64		28	
#define R_390_GOTPLT12		29	
#define R_390_GOTPLT16		30	
#define R_390_GOTPLT32		31	
#define R_390_GOTPLT64		32	
#define R_390_GOTPLTENT		33	
#define R_390_PLTOFF16		34	
#define R_390_PLTOFF32		35	
#define R_390_PLTOFF64		36	
#define R_390_TLS_LOAD		37	
#define R_390_TLS_GDCALL	38	
#define R_390_TLS_LDCALL	39	
#define R_390_TLS_GD32		40	
#define R_390_TLS_GD64		41	
#define R_390_TLS_GOTIE12	42	
#define R_390_TLS_GOTIE32	43	
#define R_390_TLS_GOTIE64	44	
#define R_390_TLS_LDM32		45	
#define R_390_TLS_LDM64		46	
#define R_390_TLS_IE32		47	
#define R_390_TLS_IE64		48	
#define R_390_TLS_IEENT		49	
#define R_390_TLS_LE32		50	
#define R_390_TLS_LE64		51	
#define R_390_TLS_LDO32		52	
#define R_390_TLS_LDO64		53	
#define R_390_TLS_DTPMOD	54	
#define R_390_TLS_DTPOFF	55	
#define R_390_TLS_TPOFF		56	
#define R_390_NUM	57

#define R_X86_64_NONE		0	
#define R_X86_64_64		1	
#define R_X86_64_PC32		2	
#define R_X86_64_GOT32		3	
#define R_X86_64_PLT32		4	
#define R_X86_64_COPY		5	
#define R_X86_64_GLOB_DAT	6	
#define R_X86_64_JUMP_SLOT	7	
#define R_X86_64_RELATIVE	8	
#define R_X86_64_GOTPCREL	9	
#define R_X86_64_32		10	
#define R_X86_64_32S		11	
#define R_X86_64_16		12	
#define R_X86_64_PC16		13	
#define R_X86_64_8		14	
#define R_X86_64_PC8		15	

#define R_X86_64_NUM		16


#define EF_ALPHA_32BIT		1	



#define EF_PARISC_TRAPNIL	0x00010000 
#define EF_PARISC_EXT		0x00020000 
#define EF_PARISC_LSB		0x00040000 
#define EF_PARISC_WIDE		0x00080000 
#define EF_PARISC_NO_KABP	0x00100000 
#define EF_PARISC_LAZYSWAP	0x00400000 
#define EF_PARISC_ARCH		0x0000ffff 


#define EFA_PARISC_1_0		    0x020b 
#define EFA_PARISC_1_1		    0x0210 
#define EFA_PARISC_2_0		    0x0214 


#define SHN_PARISC_ANSI_COMMON	0xff00	   
#define SHN_PARISC_HUGE_COMMON	0xff01	   


#define SHT_PARISC_EXT		0x70000000 
#define SHT_PARISC_UNWIND	0x70000001 
#define SHT_PARISC_DOC		0x70000002 


#define SHF_PARISC_SHORT	0x20000000 
#define SHF_PARISC_HUGE		0x40000000 
#define SHF_PARISC_SBP		0x80000000 


#define STT_PARISC_MILLICODE	13	

#define STT_HP_OPAQUE		(STT_LOOS + 0x1)
#define STT_HP_STUB		(STT_LOOS + 0x2)


#define R_PARISC_NONE		0	
#define R_PARISC_DIR32		1	
#define R_PARISC_DIR21L		2	
#define R_PARISC_DIR17R		3	
#define R_PARISC_DIR17F		4	
#define R_PARISC_DIR14R		6	
#define R_PARISC_PCREL32	9	
#define R_PARISC_PCREL21L	10	
#define R_PARISC_PCREL17R	11	
#define R_PARISC_PCREL17F	12	
#define R_PARISC_PCREL14R	14	
#define R_PARISC_DPREL21L	18	
#define R_PARISC_DPREL14R	22	
#define R_PARISC_GPREL21L	26	
#define R_PARISC_GPREL14R	30	
#define R_PARISC_LTOFF21L	34	
#define R_PARISC_LTOFF14R	38	
#define R_PARISC_SECREL32	41	
#define R_PARISC_SEGBASE	48	
#define R_PARISC_SEGREL32	49	
#define R_PARISC_PLTOFF21L	50	
#define R_PARISC_PLTOFF14R	54	
#define R_PARISC_LTOFF_FPTR32	57	
#define R_PARISC_LTOFF_FPTR21L	58	
#define R_PARISC_LTOFF_FPTR14R	62	
#define R_PARISC_FPTR64		64	
#define R_PARISC_PLABEL32	65	
#define R_PARISC_PCREL64	72	
#define R_PARISC_PCREL22F	74	
#define R_PARISC_PCREL14WR	75	
#define R_PARISC_PCREL14DR	76	
#define R_PARISC_PCREL16F	77	
#define R_PARISC_PCREL16WF	78	
#define R_PARISC_PCREL16DF	79	
#define R_PARISC_DIR64		80	
#define R_PARISC_DIR14WR	83	
#define R_PARISC_DIR14DR	84	
#define R_PARISC_DIR16F		85	
#define R_PARISC_DIR16WF	86	
#define R_PARISC_DIR16DF	87	
#define R_PARISC_GPREL64	88	
#define R_PARISC_GPREL14WR	91	
#define R_PARISC_GPREL14DR	92	
#define R_PARISC_GPREL16F	93	
#define R_PARISC_GPREL16WF	94	
#define R_PARISC_GPREL16DF	95	
#define R_PARISC_LTOFF64	96	
#define R_PARISC_LTOFF14WR	99	
#define R_PARISC_LTOFF14DR	100	
#define R_PARISC_LTOFF16F	101	
#define R_PARISC_LTOFF16WF	102	
#define R_PARISC_LTOFF16DF	103	
#define R_PARISC_SECREL64	104	
#define R_PARISC_SEGREL64	112	
#define R_PARISC_PLTOFF14WR	115	
#define R_PARISC_PLTOFF14DR	116	
#define R_PARISC_PLTOFF16F	117	
#define R_PARISC_PLTOFF16WF	118	
#define R_PARISC_PLTOFF16DF	119	
#define R_PARISC_LTOFF_FPTR64	120	
#define R_PARISC_LTOFF_FPTR14WR	123	
#define R_PARISC_LTOFF_FPTR14DR	124	
#define R_PARISC_LTOFF_FPTR16F	125	
#define R_PARISC_LTOFF_FPTR16WF	126	
#define R_PARISC_LTOFF_FPTR16DF	127	
#define R_PARISC_LORESERVE	128
#define R_PARISC_COPY		128	
#define R_PARISC_IPLT		129	
#define R_PARISC_EPLT		130	
#define R_PARISC_TPREL32	153	
#define R_PARISC_TPREL21L	154	
#define R_PARISC_TPREL14R	158	
#define R_PARISC_LTOFF_TP21L	162	
#define R_PARISC_LTOFF_TP14R	166	
#define R_PARISC_LTOFF_TP14F	167	
#define R_PARISC_TPREL64	216	
#define R_PARISC_TPREL14WR	219	
#define R_PARISC_TPREL14DR	220	
#define R_PARISC_TPREL16F	221	
#define R_PARISC_TPREL16WF	222	
#define R_PARISC_TPREL16DF	223	
#define R_PARISC_LTOFF_TP64	224	
#define R_PARISC_LTOFF_TP14WR	227	
#define R_PARISC_LTOFF_TP14DR	228	
#define R_PARISC_LTOFF_TP16F	229	
#define R_PARISC_LTOFF_TP16WF	230	
#define R_PARISC_LTOFF_TP16DF	231	
#define R_PARISC_HIRESERVE	255


#define PT_HP_TLS		(PT_LOOS + 0x0)
#define PT_HP_CORE_NONE		(PT_LOOS + 0x1)
#define PT_HP_CORE_VERSION	(PT_LOOS + 0x2)
#define PT_HP_CORE_KERNEL	(PT_LOOS + 0x3)
#define PT_HP_CORE_COMM		(PT_LOOS + 0x4)
#define PT_HP_CORE_PROC		(PT_LOOS + 0x5)
#define PT_HP_CORE_LOADABLE	(PT_LOOS + 0x6)
#define PT_HP_CORE_STACK	(PT_LOOS + 0x7)
#define PT_HP_CORE_SHM		(PT_LOOS + 0x8)
#define PT_HP_CORE_MMF		(PT_LOOS + 0x9)
#define PT_HP_PARALLEL		(PT_LOOS + 0x10)
#define PT_HP_FASTBIND		(PT_LOOS + 0x11)
#define PT_HP_OPT_ANNOT		(PT_LOOS + 0x12)
#define PT_HP_HSL_ANNOT		(PT_LOOS + 0x13)
#define PT_HP_STACK		(PT_LOOS + 0x14)

#define PT_PARISC_ARCHEXT	0x70000000
#define PT_PARISC_UNWIND	0x70000001


#define PF_PARISC_SBP		0x08000000

#define PF_HP_PAGE_SIZE		0x00100000
#define PF_HP_FAR_SHARED	0x00200000
#define PF_HP_NEAR_SHARED	0x00400000
#define PF_HP_CODE		0x01000000
#define PF_HP_MODIFY		0x02000000
#define PF_HP_LAZYSWAP		0x04000000
#define PF_HP_SBP		0x08000000


#define EF_IA_64_MASKOS		0x0000000f	
#define EF_IA_64_ABI64		0x00000010	
#define EF_IA_64_ARCH		0xff000000	

#define PT_IA_64_ARCHEXT	(PT_LOPROC + 0)	
#define PT_IA_64_UNWIND		(PT_LOPROC + 1)	

#define PF_IA_64_NORECOV	0x80000000	

#define SHT_IA_64_EXT		(SHT_LOPROC + 0) 
#define SHT_IA_64_UNWIND	(SHT_LOPROC + 1) 

#define SHF_IA_64_SHORT		0x10000000	
#define SHF_IA_64_NORECOV	0x20000000	

#define DT_IA_64_PLT_RESERVE	(DT_LOPROC + 0)
#define DT_IA_64_NUM		1

#define R_IA64_NONE		0x00	
#define R_IA64_IMM14		0x21	
#define R_IA64_IMM22		0x22	
#define R_IA64_IMM64		0x23	
#define R_IA64_DIR32MSB		0x24	
#define R_IA64_DIR32LSB		0x25	
#define R_IA64_DIR64MSB		0x26	
#define R_IA64_DIR64LSB		0x27	
#define R_IA64_GPREL22		0x2a	
#define R_IA64_GPREL64I		0x2b	
#define R_IA64_GPREL32MSB	0x2c	
#define R_IA64_GPREL32LSB	0x2d	
#define R_IA64_GPREL64MSB	0x2e	
#define R_IA64_GPREL64LSB	0x2f	
#define R_IA64_LTOFF22		0x32	
#define R_IA64_LTOFF64I		0x33	
#define R_IA64_PLTOFF22		0x3a	
#define R_IA64_PLTOFF64I	0x3b	
#define R_IA64_PLTOFF64MSB	0x3e	
#define R_IA64_PLTOFF64LSB	0x3f	
#define R_IA64_FPTR64I		0x43	
#define R_IA64_FPTR32MSB	0x44	
#define R_IA64_FPTR32LSB	0x45	
#define R_IA64_FPTR64MSB	0x46	
#define R_IA64_FPTR64LSB	0x47	
#define R_IA64_PCREL60B		0x48	
#define R_IA64_PCREL21B		0x49	
#define R_IA64_PCREL21M		0x4a	
#define R_IA64_PCREL21F		0x4b	
#define R_IA64_PCREL32MSB	0x4c	
#define R_IA64_PCREL32LSB	0x4d	
#define R_IA64_PCREL64MSB	0x4e	
#define R_IA64_PCREL64LSB	0x4f	
#define R_IA64_LTOFF_FPTR22	0x52	
#define R_IA64_LTOFF_FPTR64I	0x53	
#define R_IA64_LTOFF_FPTR32MSB	0x54	
#define R_IA64_LTOFF_FPTR32LSB	0x55	
#define R_IA64_LTOFF_FPTR64MSB	0x56	
#define R_IA64_LTOFF_FPTR64LSB	0x57	
#define R_IA64_SEGREL32MSB	0x5c	
#define R_IA64_SEGREL32LSB	0x5d	
#define R_IA64_SEGREL64MSB	0x5e	
#define R_IA64_SEGREL64LSB	0x5f	
#define R_IA64_SECREL32MSB	0x64	
#define R_IA64_SECREL32LSB	0x65	
#define R_IA64_SECREL64MSB	0x66	
#define R_IA64_SECREL64LSB	0x67	
#define R_IA64_REL32MSB		0x6c	
#define R_IA64_REL32LSB		0x6d	
#define R_IA64_REL64MSB		0x6e	
#define R_IA64_REL64LSB		0x6f	
#define R_IA64_LTV32MSB		0x74	
#define R_IA64_LTV32LSB		0x75	
#define R_IA64_LTV64MSB		0x76	
#define R_IA64_LTV64LSB		0x77	
#define R_IA64_PCREL21BI	0x79	
#define R_IA64_PCREL22		0x7a	
#define R_IA64_PCREL64I		0x7b	
#define R_IA64_IPLTMSB		0x80	
#define R_IA64_IPLTLSB		0x81	
#define R_IA64_COPY		0x84	
#define R_IA64_SUB		0x85	
#define R_IA64_LTOFF22X		0x86	
#define R_IA64_LDXMOV		0x87	
#define R_IA64_TPREL14		0x91	
#define R_IA64_TPREL22		0x92	
#define R_IA64_TPREL64I		0x93	
#define R_IA64_TPREL64MSB	0x96	
#define R_IA64_TPREL64LSB	0x97	
#define R_IA64_LTOFF_TPREL22	0x9a	
#define R_IA64_DTPMOD64MSB	0xa6	
#define R_IA64_DTPMOD64LSB	0xa7	
#define R_IA64_LTOFF_DTPMOD22	0xaa	
#define R_IA64_DTPREL14		0xb1	
#define R_IA64_DTPREL22		0xb2	
#define R_IA64_DTPREL64I	0xb3	
#define R_IA64_DTPREL32MSB	0xb4	
#define R_IA64_DTPREL32LSB	0xb5	
#define R_IA64_DTPREL64MSB	0xb6	
#define R_IA64_DTPREL64LSB	0xb7	
#define R_IA64_LTOFF_DTPREL22	0xba	

typedef struct elf32_rel {
  Elf32_Addr	r_offset;
  Elf32_Word	r_info;
} Elf32_Rel;

typedef struct elf64_rel {
  Elf64_Addr r_offset;	
  Elf64_Xword r_info;	
} Elf64_Rel;

typedef struct elf32_rela{
  Elf32_Addr	r_offset;
  Elf32_Word	r_info;
  Elf32_Sword	r_addend;
} Elf32_Rela;

typedef struct elf64_rela {
  Elf64_Addr r_offset;	
  Elf64_Xword r_info;	
  Elf64_Sxword r_addend;	
} Elf64_Rela;

typedef struct elf32_sym{
  Elf32_Word	st_name;
  Elf32_Addr	st_value;
  Elf32_Word	st_size;
  unsigned char	st_info;
  unsigned char	st_other;
  Elf32_Half	st_shndx;
} Elf32_Sym;

typedef struct elf64_sym {
  Elf64_Word st_name;		
  unsigned char	st_info;	
  unsigned char	st_other;	
  Elf64_Half st_shndx;		
  Elf64_Addr st_value;		
  Elf64_Xword st_size;		
} Elf64_Sym;


#define EI_NIDENT	16

typedef struct elf32_hdr{
  unsigned char	e_ident[EI_NIDENT];
  Elf32_Half	e_type;
  Elf32_Half	e_machine;
  Elf32_Word	e_version;
  Elf32_Addr	e_entry;  
  Elf32_Off	e_phoff;
  Elf32_Off	e_shoff;
  Elf32_Word	e_flags;
  Elf32_Half	e_ehsize;
  Elf32_Half	e_phentsize;
  Elf32_Half	e_phnum;
  Elf32_Half	e_shentsize;
  Elf32_Half	e_shnum;
  Elf32_Half	e_shstrndx;
} Elf32_Ehdr;

typedef struct elf64_hdr {
  unsigned char	e_ident[16];		
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;		
  Elf64_Off e_phoff;		
  Elf64_Off e_shoff;		
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} Elf64_Ehdr;

#define PF_R		0x4
#define PF_W		0x2
#define PF_X		0x1

typedef struct elf32_phdr{
  Elf32_Word	p_type;
  Elf32_Off	p_offset;
  Elf32_Addr	p_vaddr;
  Elf32_Addr	p_paddr;
  Elf32_Word	p_filesz;
  Elf32_Word	p_memsz;
  Elf32_Word	p_flags;
  Elf32_Word	p_align;
} Elf32_Phdr;

typedef struct elf64_phdr {
  Elf64_Word p_type;
  Elf64_Word p_flags;
  Elf64_Off p_offset;		
  Elf64_Addr p_vaddr;		
  Elf64_Addr p_paddr;		
  Elf64_Xword p_filesz;		
  Elf64_Xword p_memsz;		
  Elf64_Xword p_align;		
} Elf64_Phdr;

#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_NUM		12
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff
#define SHT_MIPS_LIST		0x70000000
#define SHT_MIPS_CONFLICT	0x70000002
#define SHT_MIPS_GPTAB		0x70000003
#define SHT_MIPS_UCODE		0x70000004

#define SHF_WRITE	0x1
#define SHF_ALLOC	0x2
#define SHF_EXECINSTR	0x4
#define SHF_MASKPROC	0xf0000000
#define SHF_MIPS_GPREL	0x10000000

#define SHN_UNDEF	0
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff
#define SHN_MIPS_ACCOMON	0xff00

typedef struct elf32_shdr {
  Elf32_Word	sh_name;
  Elf32_Word	sh_type;
  Elf32_Word	sh_flags;
  Elf32_Addr	sh_addr;
  Elf32_Off	sh_offset;
  Elf32_Word	sh_size;
  Elf32_Word	sh_link;
  Elf32_Word	sh_info;
  Elf32_Word	sh_addralign;
  Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct elf64_shdr {
  Elf64_Word sh_name;		
  Elf64_Word sh_type;		
  Elf64_Xword sh_flags;		
  Elf64_Addr sh_addr;		
  Elf64_Off sh_offset;		
  Elf64_Xword sh_size;		
  Elf64_Word sh_link;		
  Elf64_Word sh_info;		
  Elf64_Xword sh_addralign;	
  Elf64_Xword sh_entsize;	
} Elf64_Shdr;

#define	EI_MAG0		0		
#define	EI_MAG1		1
#define	EI_MAG2		2
#define	EI_MAG3		3
#define	EI_CLASS	4
#define	EI_DATA		5
#define	EI_VERSION	6
#define	EI_OSABI	7
#define	EI_PAD		8

#define ELFOSABI_NONE           0       
#define ELFOSABI_SYSV           0       
#define ELFOSABI_HPUX           1       
#define ELFOSABI_NETBSD         2       
#define ELFOSABI_LINUX          3       
#define ELFOSABI_SOLARIS        6       
#define ELFOSABI_AIX            7       
#define ELFOSABI_IRIX           8       
#define ELFOSABI_FREEBSD        9       
#define ELFOSABI_TRU64          10      
#define ELFOSABI_MODESTO        11      
#define ELFOSABI_OPENBSD        12      
#define ELFOSABI_ARM            97      
#define ELFOSABI_STANDALONE     255     

#define	ELFMAG0		0x7f		
#define	ELFMAG1		'E'
#define	ELFMAG2		'L'
#define	ELFMAG3		'F'
#define	ELFMAG		"\177ELF"
#define	SELFMAG		4

#define	ELFCLASSNONE	0		
#define	ELFCLASS32	1
#define	ELFCLASS64	2
#define	ELFCLASSNUM	3

#define ELFDATANONE	0		
#define ELFDATA2LSB	1
#define ELFDATA2MSB	2

#define EV_NONE		0		
#define EV_CURRENT	1
#define EV_NUM		2

#define NT_PRSTATUS	1
#define NT_PRFPREG	2
#define NT_PRPSINFO	3
#define NT_TASKSTRUCT	4
#define NT_AUXV		6
#define NT_PRXFPREG     0x46e62b7f      


typedef struct elf32_note {
  Elf32_Word	n_namesz;	
  Elf32_Word	n_descsz;	
  Elf32_Word	n_type;		
} Elf32_Nhdr;

typedef struct elf64_note {
  Elf64_Word n_namesz;	
  Elf64_Word n_descsz;	
  Elf64_Word n_type;	
} Elf64_Nhdr;


struct elf32_fdpic_loadseg {
  
  Elf32_Addr addr;
  
  Elf32_Addr p_vaddr;
  
  Elf32_Word p_memsz;
};
struct elf32_fdpic_loadmap {
  
  Elf32_Half version;
  
  Elf32_Half nsegs;
  
  struct elf32_fdpic_loadseg segs[];
};

#ifdef ELF_CLASS
#if ELF_CLASS == ELFCLASS32

#define elfhdr		elf32_hdr
#define elf_phdr	elf32_phdr
#define elf_note	elf32_note
#define elf_shdr	elf32_shdr
#define elf_sym		elf32_sym
#define elf_addr_t	Elf32_Off

#ifdef ELF_USES_RELOCA
# define ELF_RELOC      Elf32_Rela
#else
# define ELF_RELOC      Elf32_Rel
#endif

#else

#define elfhdr		elf64_hdr
#define elf_phdr	elf64_phdr
#define elf_note	elf64_note
#define elf_shdr	elf64_shdr
#define elf_sym		elf64_sym
#define elf_addr_t	Elf64_Off

#ifdef ELF_USES_RELOCA
# define ELF_RELOC      Elf64_Rela
#else
# define ELF_RELOC      Elf64_Rel
#endif

#endif 

#ifndef ElfW
# if ELF_CLASS == ELFCLASS32
#  define ElfW(x)  Elf32_ ## x
#  define ELFW(x)  ELF32_ ## x
# else
#  define ElfW(x)  Elf64_ ## x
#  define ELFW(x)  ELF64_ ## x
# endif
#endif

#endif 


#endif 
