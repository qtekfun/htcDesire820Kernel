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


#ifndef ELFF_ELF_FILE_H_
#define ELFF_ELF_FILE_H_

#include "dwarf_die.h"
#include "elf_mapped_section.h"
#include "elff_api.h"
#include "android/utils/mapfile.h"

class ElfFile {
 public:
  
  ElfFile();

  
  virtual ~ElfFile();

  static ElfFile* Create(const char* path);

  
  bool is_ELF_64() const {
    return is_ELF_64_;
  }
  bool is_ELF_32() const {
    return !is_ELF_64_;
  }

  
  bool is_elf_big_endian() const {
    return is_elf_big_endian_;
  }
  bool is_elf_little_endian() const {
    return !is_elf_big_endian_;
  }

  bool same_endianness() const {
    return same_endianness_;
  }

  
  bool is_DWARF_64() const {
    return is_DWARF_64_;
  }
  bool is_DWARF_32() const {
    return !is_DWARF_64_;
  }

  
  class ElfAllocator* allocator() const {
    return allocator_;
  }

  class DwarfCU* last_cu() const {
    return last_cu_;
  }

  int cu_count() const {
    return cu_count_;
  }

  
  bool is_exec() const {
      return is_exec_;
  }

 protected:
  virtual bool initialize(const Elf_CommonHdr* elf_hdr, const char* path);


 public:
  uint8_t pull_val(const uint8_t* val) const {
    return *val;
  }
  uint8_t pull_val(const uint8_t& val) const {
    return val;
  }
  int8_t pull_val(const int8_t* val) const {
    return *val;
  }
  int8_t pull_val(const int8_t& val) const {
    return val;
  }

  uint16_t pull_val(const uint16_t* val) const {
    if (same_endianness()) {
      return *val;
    }
    if (is_elf_big_endian()) {
      return (uint16_t)get_byte(val, 0) << 8 | get_byte(val, 1);
    } else {
      return (uint16_t)get_byte(val, 1) << 8 | get_byte(val, 0);
    }
  }
  uint16_t pull_val(const uint16_t& val) const {
    return same_endianness() ? val : pull_val(&val);
  }
  int16_t pull_val(const int16_t* val) const {
    return static_cast<int16_t>
              (pull_val(reinterpret_cast<const uint16_t*>(val)));
  }
  int16_t pull_val(const int16_t& val) const {
    return static_cast<int16_t>
              (pull_val(reinterpret_cast<const uint16_t&>(val)));
  }

  uint32_t pull_val(const uint32_t* val) const {
    if (same_endianness()) {
      return *val;
    }
    if (is_elf_big_endian()) {
      return (uint32_t)get_byte(val, 0) << 24 |
             (uint32_t)get_byte(val, 1) << 16 |
             (uint32_t)get_byte(val, 2) << 8  |
             (uint32_t)get_byte(val, 3);
    } else {
      return (uint32_t)get_byte(val, 3) << 24 |
             (uint32_t)get_byte(val, 2) << 16 |
             (uint32_t)get_byte(val, 1) << 8  |
             (uint32_t)get_byte(val, 0);
    }
  }
  uint32_t pull_val(const uint32_t& val) const {
    return same_endianness() ? val : pull_val(&val);
  }
  int32_t pull_val(const int32_t* val) const {
    return static_cast<int32_t>
              (pull_val(reinterpret_cast<const uint32_t*>(val)));
  }
  int32_t pull_val(const int32_t& val) const {
    return static_cast<int32_t>
              (pull_val(reinterpret_cast<const uint32_t&>(val)));
  }

  uint64_t pull_val(const uint64_t* val) const {
    if (same_endianness()) {
      return *val;
    }
    if (is_elf_big_endian()) {
      return (uint64_t)get_byte(val, 0) << 56 |
             (uint64_t)get_byte(val, 1) << 48 |
             (uint64_t)get_byte(val, 2) << 40 |
             (uint64_t)get_byte(val, 3) << 32 |
             (uint64_t)get_byte(val, 4) << 24 |
             (uint64_t)get_byte(val, 5) << 16 |
             (uint64_t)get_byte(val, 6) << 8  |
             (uint64_t)get_byte(val, 7);
    } else {
      return (uint64_t)get_byte(val, 7) << 56 |
             (uint64_t)get_byte(val, 6) << 48 |
             (uint64_t)get_byte(val, 5) << 40 |
             (uint64_t)get_byte(val, 4) << 32 |
             (uint64_t)get_byte(val, 3) << 24 |
             (uint64_t)get_byte(val, 2) << 16 |
             (uint64_t)get_byte(val, 1) << 8  |
             (uint64_t)get_byte(val, 0);
    }
  }
  uint64_t pull_val(const uint64_t& val) const {
    return same_endianness() ? val : pull_val(&val);
  }
  int64_t pull_val(const int64_t* val) const {
    return static_cast<int64_t>
              (pull_val(reinterpret_cast<const uint64_t*>(val)));
  }
  int64_t pull_val(const int64_t& val) const {
    return static_cast<int64_t>
              (pull_val(reinterpret_cast<const uint64_t&>(val)));
  }


 public:
  const char* get_str_sec_str(Elf_Xword index) const {
    assert(string_section_.is_mapped() && index < string_section_.size());
    if (string_section_.is_mapped() && index < string_section_.size()) {
      return INC_CPTR_T(char, string_section_.data(), index);
    } else {
      _set_errno(EINVAL);
      return NULL;
    }
  }

  const char* get_debug_str(Elf_Xword index) const {
    assert(debug_str_.is_mapped() && index < debug_str_.size());
    if (debug_str_.is_mapped() && index < debug_str_.size()) {
      return INC_CPTR_T(char, debug_str_.data(), index);
    } else {
      _set_errno(EINVAL);
      return NULL;
    }
  }

 protected:
  const void* get_section_by_index(Elf_Half index) const {
    assert(index < sec_count_);
    if (index < sec_count_) {
      return INC_CPTR(sec_table_, static_cast<size_t>(index) * sec_entry_size_);
    } else {
      _set_errno(EINVAL);
      return NULL;
    }
  }


 protected:
  virtual int parse_compilation_units(const DwarfParseContext* parse_context) = 0;

 public:
  bool get_pc_address_info(Elf_Xword address, Elf_AddressInfo* address_info);

  void free_pc_address_info(Elf_AddressInfo* address_info) const;

  const void* get_debug_info_data() const {
    return debug_info_.data();
  }

  const void* get_debug_abbrev_data() const {
    return debug_abbrev_.data();
  }

  const void* get_debug_ranges_data() const {
    return debug_ranges_.data();
  }

  const void* get_debug_line_data() const {
    return debug_line_.data();
  }

  bool is_valid_die_ptr(const void* ptr, size_t size) const {
    return debug_info_.is_contained(ptr, size);
  }

  bool is_valid_abbr_ptr(const void* ptr, size_t size) const {
    return debug_abbrev_.is_contained(ptr, size);
  }

  bool is_valid_cu(const void* cu_header) const {
    if (is_DWARF_64()) {
      return is_valid_die_ptr(cu_header, sizeof(Dwarf64_CUHdr)) &&
             reinterpret_cast<const Dwarf64_CUHdr*>(cu_header)->size_hdr.size != 0;
    } else {
      return is_valid_die_ptr(cu_header, sizeof(Dwarf32_CUHdr)) &&
             reinterpret_cast<const Dwarf32_CUHdr*>(cu_header)->size_hdr.size != 0;
    }
  }

  template<typename AddrType>
  bool get_range(Elf_Word offset, AddrType* low, AddrType* high) {
    const AddrType* ptr = INC_CPTR_T(AddrType, debug_ranges_.data(), offset);
    assert(debug_ranges_.is_contained(ptr, sizeof(AddrType) * 2));
    if (!debug_ranges_.is_contained(ptr, sizeof(AddrType) * 2)) {
      _set_errno(EINVAL);
      return false;
    }
    *low = pull_val(ptr);
    *high = pull_val(ptr + 1);
    return true;
  }

 protected:
  
  ElfMappedSection    string_section_;

  
  ElfMappedSection    debug_info_;

  
  ElfMappedSection    debug_abbrev_;

  
  ElfMappedSection    debug_str_;

  
  ElfMappedSection    debug_line_;

  
  ElfMappedSection    debug_ranges_;

  Elf_Xword           fixed_base_address_;

  
  MapFile*            elf_handle_;

  
  char*               elf_file_path_;

  
  class ElfAllocator* allocator_;

  
  void*               sec_table_;

  
  Elf_Half            sec_count_;

  
  Elf_Half            sec_entry_size_;

  
  class DwarfCU*      last_cu_;

  
  int                 cu_count_;

  
  bool                is_ELF_64_;

  bool                is_elf_big_endian_;

  bool                same_endianness_;

  bool                is_DWARF_64_;

  bool                is_exec_;
};

template <typename Elf_Addr, typename Elf_Off>
class ElfFileImpl : protected ElfFile {
friend class ElfFile;
 protected:
  
  ElfFileImpl() {
  };

  
  ~ElfFileImpl() {
  }

 protected:
  bool initialize(const Elf_CommonHdr* elf_hdr, const char* path);

  virtual int parse_compilation_units(const DwarfParseContext* parse_context);

  bool get_section_info_by_name(const char* name,
                                Elf_Off* offset,
                                Elf_Word* size);

  bool map_section_by_name(const char* name, ElfMappedSection* section);
};

#endif  
