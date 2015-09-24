/* Copyright (C) 2007-2010 The Android Open Source Project
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


#ifndef ELFF_ELH_H_
#define ELFF_ELH_H_

#include <stdint.h>
#include "elff-common.h"




typedef uint8_t   Elf_Byte;
typedef int8_t    Elf_Sbyte;

typedef uint16_t  Elf_Half;
typedef int16_t   Elf_Shalf;

typedef uint32_t  Elf_Word;
typedef int32_t   Elf_Sword;

typedef uint64_t  Elf_Xword;
typedef int64_t   Elf_Sxword;


typedef uint32_t  Elf32_Addr;
typedef uint32_t  Elf32_Off;

typedef uint64_t  Elf64_Addr;
typedef uint64_t  Elf64_Off;


#define EI_NIDENT	16

typedef struct Elf_CommonHdr {
  union {
    struct {
      
      Elf_Byte  ei_mag0;
      Elf_Byte  ei_mag1;
      Elf_Byte  ei_mag2;
      Elf_Byte  ei_mag3;

      
      Elf_Byte  ei_class;

      
      Elf_Byte  ei_data;

      
      Elf_Byte  ei_version;
    } ei_info;
    unsigned char e_ident[EI_NIDENT];
  };

  
  Elf_Half      e_type;

  
  Elf_Half      e_machine;

  
  Elf_Word      e_version;
} Elf_CommonHdr;


#define ELFMAG0		0x7f
#define ELFMAG1		'E'
#define ELFMAG2		'L'
#define ELFMAG3		'F'
#define ELFMAG		"\177ELF"
#define SELFMAG		4


#define ELFCLASSNONE  0
#define ELFCLASS32    1
#define ELFCLASS64    2


#define ELFDATANONE   0
#define ELFDATA2LSB   1
#define ELFDATA2MSB   2

template <typename Elf_Addr, typename Elf_Off>
struct Elf_FHdr {
  
  Elf_CommonHdr common;

  
  Elf_Addr      e_entry;

  Elf_Off       e_phoff;

  Elf_Off       e_shoff;

  
  Elf_Word      e_flags;

  
  Elf_Half      e_ehsize;

  Elf_Half      e_phentsize;

  
  Elf_Half      e_phnum;

  Elf_Half      e_shentsize;

  
  Elf_Half      e_shnum;

  Elf_Half      e_shstrndx;
};
typedef Elf_FHdr<Elf32_Addr, Elf32_Off> Elf32_FHdr;
typedef Elf_FHdr<Elf64_Addr, Elf64_Off> Elf64_FHdr;


template <typename Elf_Addr, typename Elf_Off>
struct Elf_SHdr {
  
  Elf_Word    sh_name;

  
  Elf_Word    sh_type;

  
  Elf_Word    sh_flags;

  
  Elf_Addr    sh_addr;

  Elf_Off     sh_offset;

  
  Elf_Word    sh_size;

  
  Elf_Word    sh_link;

  
  Elf_Word    sh_info;

  Elf_Word    sh_addralign;

  
  Elf_Word    sh_entsize;
};
typedef Elf_SHdr<Elf32_Addr, Elf32_Off> Elf32_SHdr;
typedef Elf_SHdr<Elf64_Addr, Elf64_Off> Elf64_SHdr;

#define SHN_UNDEF       0
#define SHN_LORESERVE   0xff00
#define SHN_LOPROC      0xff00
#define SHN_HIPROC      0xff1f
#define SHN_LOOS        0xff20
#define SHN_HIOS        0xff3f
#define SHN_ABS         0xfff1
#define SHN_COMMON      0xfff2
#define SHN_XINDEX      0xffff
#define SHN_HIRESERVE   0xffff

#define SHT_NULL            0
#define SHT_PROGBITS        1
#define SHT_SYMTAB          2
#define SHT_STRTAB          3
#define SHT_RELA            4
#define SHT_HASH            5
#define SHT_DYNAMIC         6
#define SHT_NOTE            7
#define SHT_NOBITS          8
#define SHT_REL             9
#define SHT_SHLIB           10
#define SHT_DYNSYM          11
#define SHT_INIT_ARRAY      14
#define SHT_FINI_ARRAY      15
#define SHT_PREINIT_ARRAY   16
#define SHT_GROUP           17
#define SHT_SYMTAB_SHNDX    18
#define SHT_NUM             19

#endif  
