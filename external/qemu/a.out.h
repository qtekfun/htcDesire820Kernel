/* a.out.h

   Copyright 1997, 1998, 1999, 2001 Red Hat, Inc.

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef _A_OUT_H_
#define _A_OUT_H_

#ifdef __cplusplus
extern "C" {
#endif
#define COFF_IMAGE_WITH_PE
#define COFF_LONG_SECTION_NAMES




struct external_filehdr {
  short f_magic;	
  short f_nscns;	
  host_ulong f_timdat;	
  host_ulong f_symptr;	
  host_ulong f_nsyms;	
  short f_opthdr;	
  short f_flags;	
};


#define F_RELFLG	(0x0001)
#define F_EXEC		(0x0002)
#define F_LNNO		(0x0004)
#define F_LSYMS		(0x0008)



#define	I386MAGIC	0x14c
#define I386PTXMAGIC	0x154
#define I386AIXMAGIC	0x175


#define LYNXCOFFMAGIC	0415

#define I386BADMAG(x) (((x).f_magic != I386MAGIC) \
		       && (x).f_magic != I386AIXMAGIC \
		       && (x).f_magic != I386PTXMAGIC \
		       && (x).f_magic != LYNXCOFFMAGIC)

#define	FILHDR	struct external_filehdr
#define	FILHSZ	20




typedef struct
{
  unsigned short magic;		
  unsigned short vstamp;	
  host_ulong	tsize;		
  host_ulong	dsize;		
  host_ulong	bsize;		
  host_ulong	entry;		
  host_ulong text_start;	
  host_ulong data_start;	
}
AOUTHDR;

#define AOUTSZ 28
#define AOUTHDRSZ 28

#define OMAGIC          0404    
#define ZMAGIC          0413    
#define STMAGIC		0401	
#define SHMAGIC		0443	


#define NT_SECTION_ALIGNMENT 0x1000
#define NT_FILE_ALIGNMENT    0x200
#define NT_DEF_RESERVE       0x100000
#define NT_DEF_COMMIT        0x1000



struct external_scnhdr {
  char		s_name[8];	
  host_ulong	s_paddr;	
  host_ulong	s_vaddr;	
  host_ulong	s_size;		
  host_ulong	s_scnptr;	
  host_ulong	s_relptr;	
  host_ulong	s_lnnoptr;	
  unsigned short s_nreloc;	
  unsigned short s_nlnno;	
  host_ulong	s_flags;	
};

#define	SCNHDR	struct external_scnhdr
#define	SCNHSZ	40

#define _TEXT	".text"
#define _DATA	".data"
#define _BSS	".bss"
#define _COMMENT ".comment"
#define _LIB ".lib"


struct external_lineno {
  union {
    host_ulong l_symndx; 
    host_ulong l_paddr;	
  } l_addr;
  unsigned short l_lnno;	
};

#define	LINENO	struct external_lineno
#define	LINESZ	6


#define E_SYMNMLEN	8	
#define E_FILNMLEN	14	
#define E_DIMNUM	4	

struct __attribute__((packed)) external_syment
{
  union {
    char e_name[E_SYMNMLEN];
    struct {
      host_ulong e_zeroes;
      host_ulong e_offset;
    } e;
  } e;
  host_ulong e_value;
  unsigned short e_scnum;
  unsigned short e_type;
  char e_sclass[1];
  char e_numaux[1];
};

#define N_BTMASK	(0xf)
#define N_TMASK		(0x30)
#define N_BTSHFT	(4)
#define N_TSHIFT	(2)

union external_auxent {
  struct {
    host_ulong x_tagndx;	
    union {
      struct {
	unsigned short  x_lnno; 
	unsigned short  x_size; 
      } x_lnsz;
      host_ulong x_fsize;	
    } x_misc;
    union {
      struct {			
	host_ulong x_lnnoptr;
	host_ulong x_endndx;	
      } x_fcn;
      struct {			
	char x_dimen[E_DIMNUM][2];
      } x_ary;
    } x_fcnary;
    unsigned short x_tvndx;	
  } x_sym;

  union {
    char x_fname[E_FILNMLEN];
    struct {
      host_ulong x_zeroes;
      host_ulong x_offset;
    } x_n;
  } x_file;

  struct {
    host_ulong x_scnlen;	
    unsigned short x_nreloc;	
    unsigned short x_nlinno;	
    host_ulong x_checksum;	
    unsigned short x_associated;
    char x_comdat[1];		
  } x_scn;

  struct {
    host_ulong x_tvfill;	
    unsigned short x_tvlen;	
    char x_tvran[2][2];		
  } x_tv;	

};

#define	SYMENT	struct external_syment
#define	SYMESZ	18
#define	AUXENT	union external_auxent
#define	AUXESZ	18

#define _ETEXT	"etext"


struct external_reloc {
  char r_vaddr[4];
  char r_symndx[4];
  char r_type[2];
};

#define RELOC struct external_reloc
#define RELSZ 10



#ifndef _PE_H
#define _PE_H

#define IMAGE_FILE_RELOCS_STRIPPED           0x0001
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080
#define IMAGE_FILE_32BIT_MACHINE             0x0100
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200
#define IMAGE_FILE_SYSTEM                    0x1000
#define IMAGE_FILE_DLL                       0x2000
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000

#define IMAGE_SCN_MEM_DISCARDABLE 0x02000000
#define IMAGE_SCN_MEM_EXECUTE     0x20000000
#define IMAGE_SCN_MEM_READ        0x40000000
#define IMAGE_SCN_MEM_WRITE       0x80000000


#define IMAGE_SCN_TYPE_NO_PAD                0x00000008  

#define IMAGE_SCN_CNT_CODE                   0x00000020  
#define IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  

#define IMAGE_SCN_LNK_OTHER                  0x00000100  
#define IMAGE_SCN_LNK_INFO                   0x00000200  
#define IMAGE_SCN_LNK_REMOVE                 0x00000800  
#define IMAGE_SCN_LNK_COMDAT                 0x00001000  

#define IMAGE_SCN_MEM_FARDATA                0x00008000

#define IMAGE_SCN_MEM_PURGEABLE              0x00020000
#define IMAGE_SCN_MEM_16BIT                  0x00020000
#define IMAGE_SCN_MEM_LOCKED                 0x00040000
#define IMAGE_SCN_MEM_PRELOAD                0x00080000

#define IMAGE_SCN_ALIGN_1BYTES               0x00100000
#define IMAGE_SCN_ALIGN_2BYTES               0x00200000
#define IMAGE_SCN_ALIGN_4BYTES               0x00300000
#define IMAGE_SCN_ALIGN_8BYTES               0x00400000
#define IMAGE_SCN_ALIGN_16BYTES              0x00500000  
#define IMAGE_SCN_ALIGN_32BYTES              0x00600000
#define IMAGE_SCN_ALIGN_64BYTES              0x00700000


#define IMAGE_SCN_LNK_NRELOC_OVFL            0x01000000  
#define IMAGE_SCN_MEM_NOT_CACHED             0x04000000  
#define IMAGE_SCN_MEM_NOT_PAGED              0x08000000  
#define IMAGE_SCN_MEM_SHARED                 0x10000000  


#define IMAGE_COMDAT_SELECT_NODUPLICATES     (1) 
#define IMAGE_COMDAT_SELECT_ANY		     (2) 
#define IMAGE_COMDAT_SELECT_SAME_SIZE	     (3) 
#define IMAGE_COMDAT_SELECT_EXACT_MATCH	     (4) 
#define IMAGE_COMDAT_SELECT_ASSOCIATIVE	     (5) 

#define DOSMAGIC       0x5a4d
#define NT_SIGNATURE   0x00004550

#undef  FILNMLEN
#define FILNMLEN	18	


#ifdef COFF_IMAGE_WITH_PE

#undef FILHDR
struct external_PE_filehdr
{
  
  unsigned short e_magic;	
  unsigned short e_cblp;	
  unsigned short e_cp;		
  unsigned short e_crlc;	
  unsigned short e_cparhdr;	
  unsigned short e_minalloc;	
  unsigned short e_maxalloc;	
  unsigned short e_ss;		
  unsigned short e_sp;		
  unsigned short e_csum;	
  unsigned short e_ip;		
  unsigned short e_cs;		
  unsigned short e_lfarlc;	
  unsigned short e_ovno;	
  char e_res[4][2];		
  unsigned short e_oemid;	
  unsigned short e_oeminfo;	
  char e_res2[10][2];		
  host_ulong e_lfanew;	
  char dos_message[16][4];	
  unsigned int nt_signature;	

  

  unsigned short f_magic;	
  unsigned short f_nscns;	
  host_ulong f_timdat;	
  host_ulong f_symptr;	
  host_ulong f_nsyms;	
  unsigned short f_opthdr;	
  unsigned short f_flags;	
};


#define FILHDR struct external_PE_filehdr
#undef FILHSZ
#define FILHSZ 152

#endif

typedef struct
{
  unsigned short magic;		
  unsigned short vstamp;	
  host_ulong	tsize;		
  host_ulong	dsize;		
  host_ulong	bsize;		
  host_ulong	entry;		
  host_ulong text_start;	
  host_ulong data_start;	

  
  host_ulong  ImageBase;
  host_ulong  SectionAlignment;
  host_ulong  FileAlignment;
  unsigned short  MajorOperatingSystemVersion;
  unsigned short  MinorOperatingSystemVersion;
  unsigned short  MajorImageVersion;
  unsigned short  MinorImageVersion;
  unsigned short  MajorSubsystemVersion;
  unsigned short  MinorSubsystemVersion;
  char  Reserved1[4];
  host_ulong  SizeOfImage;
  host_ulong  SizeOfHeaders;
  host_ulong  CheckSum;
  unsigned short Subsystem;
  unsigned short DllCharacteristics;
  host_ulong  SizeOfStackReserve;
  host_ulong  SizeOfStackCommit;
  host_ulong  SizeOfHeapReserve;
  host_ulong  SizeOfHeapCommit;
  host_ulong  LoaderFlags;
  host_ulong  NumberOfRvaAndSizes;
  
  char  DataDirectory[16][2][4]; 

} PEAOUTHDR;


#undef AOUTSZ
#define AOUTSZ (AOUTHDRSZ + 196)

#undef  E_FILNMLEN
#define E_FILNMLEN	18	
#endif


#define DT_NON		(0)	
#define DT_PTR		(1)	
#define DT_FCN		(2)	
#define DT_ARY		(3)	

#define ISPTR(x)	(((x) & N_TMASK) == (DT_PTR << N_BTSHFT))
#define ISFCN(x)	(((x) & N_TMASK) == (DT_FCN << N_BTSHFT))
#define ISARY(x)	(((x) & N_TMASK) == (DT_ARY << N_BTSHFT))

#ifdef __cplusplus
}
#endif

#endif 
