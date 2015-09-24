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


#ifndef ELFF_DWARF_CU_H_
#define ELFF_DWARF_CU_H_

#include "dwarf_defs.h"
#include "dwarf_die.h"

typedef struct Dwarf_AddressInfo {
  
  const DIEObject*  die_obj;

  
  const char*       file_name;

  
  const char*       dir_name;

  
  Elf_Word          line_number;
} Dwarf_AddressInfo;

typedef struct Dwarf_STMTL_Hdr {
  Elf_Xword                   unit_length;

  Elf_Half                    version;

  Elf_Xword                   header_length;

  Elf_Byte                    min_instruction_len;

  
  Elf_Byte                    default_is_stmt;

  
  Elf_Sbyte                   line_base;

  
  Elf_Byte                    line_range;

  
  Elf_Byte                    opcode_base;

  const Elf_Byte*             standard_opcode_lengths;

  const char*                 include_directories;

  Elf_Word                    inc_dir_num;

  const Dwarf_STMTL_FileDesc* file_infos;

  
  const Elf_Byte*             start;

  
  const Elf_Byte*             end;
} Dwarf_STMTL_Hdr;

class DwarfCU : public DwarfAllocBase {
friend class ElfFile;
 public:
  explicit DwarfCU(ElfFile* elf);

  
  virtual ~DwarfCU();

  static DwarfCU* create_instance(ElfFile* elf, const void* hdr);

  const Elf_Byte* process_attrib(const Elf_Byte* attr,
                                 Dwarf_Form form,
                                 Dwarf_Value* attr_value) const;

  
  void dump() const;

  
  ElfFile* elf_file() const {
    return elf_file_;
  }

  
  DIEObject* cu_die() const {
    return cu_die_;
  }

  
  Elf_Byte addr_sizeof() const {
    return addr_sizeof_;
  }

  const char* comp_dir_path() const {
    DIEAttrib attr;
    return cu_die()->get_attrib(DW_AT_comp_dir, &attr) ? attr.value()->str :
                                                         NULL;
  }

  const char* rel_cu_path() const {
    DIEAttrib attr;
    return cu_die()->get_attrib(DW_AT_name, &attr) ? attr.value()->str :
                                                     NULL;
  }

  
  DwarfCU* prev_cu() const {
    return prev_cu_;
  }

  
  void set_prev_cu(DwarfCU* prev) {
    prev_cu_ = prev;
  }

  
  bool is_DWARF3_or_higher() const {
    return version_ >= 3;
  }

  const Dwarf_Abbr_DIE* get_die_abbr(Dwarf_AbbrNum num) const {
    return abbrs_.get(num);
  }

  DIEObject* get_leaf_die_for_address(Elf_Xword address) const {
    return cu_die_->get_leaf_for_address(address);
  }

  
  bool is_CU_address_64() const {
    return addr_sizeof_ == 8;
  }
  bool is_CU_address_32() const {
    return addr_sizeof_ == 4;
  }


 public:
  virtual bool parse(const DwarfParseContext* parse_context,
                     const void** next_cu_die) = 0;

  virtual DIEObject* get_referenced_die_object(Elf_Word ref) const = 0;

  virtual Elf_Word get_die_reference(const Dwarf_DIE* die) const = 0;

  virtual bool get_pc_address_file_info(Elf_Xword address,
                                        Dwarf_AddressInfo* info) = 0;

  virtual const Dwarf_STMTL_FileDesc* get_stmt_file_info(Elf_Word index) = 0;

  virtual const char* get_stmt_dir_name(Elf_Word dir_index) = 0;

 protected:
  
  DwarfAbbrDieArray   abbrs_;

  
  ElfFile*            elf_file_;

  
  DIEObject*          cu_die_;

  DwarfCU*            prev_cu_;

  
  Elf_Half            version_;

  
  Elf_Byte            addr_sizeof_;
};

template <typename Dwarf_CUHdr, typename Dwarf_Off>
class DwarfCUImpl : public DwarfCU {
 public:
  DwarfCUImpl(ElfFile* elf, const Dwarf_CUHdr* hdr);

  
  ~DwarfCUImpl() {
  }

  bool parse(const DwarfParseContext* parse_context,
             const void** next_cu_die);

  bool get_pc_address_file_info(Elf_Xword address, Dwarf_AddressInfo* info);

  const Dwarf_STMTL_FileDesc* get_stmt_file_info(Elf_Word index);

  const char* get_stmt_dir_name(Elf_Word dir_index);

  DIEObject* get_referenced_die_object(Elf_Word ref) const {
    const Dwarf_DIE* die = get_referenced_die(ref);
    return cu_die_->find_die_object(die);
  }

  Elf_Word get_die_reference(const Dwarf_DIE* die) const {
    return static_cast<Elf_Word>(diff_ptr(cu_header_, die));
  }

 protected:
  const Elf_Byte* process_DIE(const DwarfParseContext* parse_context,
                              const Dwarf_DIE* die,
                              DIEObject* parent_obj);

  DIEObject* create_die_object(const DwarfParseContext* parse_context,
                               const Dwarf_DIE* die,
                               DIEObject* parent,
                               Dwarf_Tag tag);

  
  bool init_stmtl();

  bool set_source_info(const DwarfStateMachine* state,
                       Dwarf_AddressInfo* info);

  
  const Dwarf_DIE* get_DIE() const {
    
    return INC_CPTR_T(Dwarf_DIE, cu_header_, sizeof(Dwarf_CUHdr));
  }

  template <typename Dwarf_STMTL_Hdr>
  void cache_stmtl(const Dwarf_STMTL_Hdr* stmtl_hdr) {
    stmtl_header_.unit_length = elf_file()->pull_val(stmtl_hdr->unit_length.size);
    stmtl_header_.version = elf_file()->pull_val(stmtl_hdr->version);
    stmtl_header_.header_length = elf_file()->pull_val(stmtl_hdr->header_length);
    stmtl_header_.min_instruction_len = stmtl_hdr->min_instruction_len;
    stmtl_header_.default_is_stmt = stmtl_hdr->default_is_stmt;
    stmtl_header_.line_base = stmtl_hdr->line_base;
    stmtl_header_.line_range = stmtl_hdr->line_range;
    stmtl_header_.opcode_base = stmtl_hdr->opcode_base;
    stmtl_header_.standard_opcode_lengths = &stmtl_hdr->standard_opcode_lengths;
    stmtl_header_.start = INC_CPTR_T(Elf_Byte, &stmtl_hdr->min_instruction_len,
                                     stmtl_header_.header_length);
    stmtl_header_.end = INC_CPTR_T(Elf_Byte, &stmtl_hdr->version,
                                   stmtl_header_.unit_length);
    stmtl_header_.include_directories =
        INC_CPTR_T(char, stmtl_header_.standard_opcode_lengths,
                   stmtl_header_.opcode_base - 1);
    const char* dir = stmtl_header_.include_directories;
    while (*dir != '\0') {
      dir += strlen(dir) + 1;
      stmtl_header_.inc_dir_num++;
    }
    stmtl_header_.file_infos = INC_CPTR_T(Dwarf_STMTL_FileDesc, dir, 1);
  }

  const Dwarf_DIE* get_referenced_die(Elf_Word ref) const {
    return INC_CPTR_T(Dwarf_DIE, cu_header_, ref);
  }

  bool is_attrib_ptr_valid(const void* ptr) const {
    return diff_ptr(cu_header_, ptr) < cu_size_;
  }

 protected:
  const Dwarf_CUHdr*          cu_header_;

  Dwarf_Off                   cu_size_;

  
  Dwarf_STMTL_Hdr             stmtl_header_;
};

#endif  
