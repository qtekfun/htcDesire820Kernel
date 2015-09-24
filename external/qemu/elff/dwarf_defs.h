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


#ifndef ELFF_DWARF_DEFS_H_
#define ELFF_DWARF_DEFS_H_

#include "dwarf.h"
#include "elf_defs.h"

#define ELFF_PACKED __attribute__ ((packed))


typedef uint32_t  Dwarf_AbbrNum;

typedef uint16_t  Dwarf_Tag;

typedef uint16_t  Dwarf_At;

typedef uint16_t  Dwarf_Form;

typedef uint32_t  Dwarf32_Off;

typedef uint64_t  Dwarf64_Off;

typedef enum DwarfValueType {
  
  DWARF_VALUE_UNKNOWN = 1,

  
  DWARF_VALUE_U8,

  
  DWARF_VALUE_S8,

  
  DWARF_VALUE_U16,

  
  DWARF_VALUE_S16,

  
  DWARF_VALUE_U32,

  
  DWARF_VALUE_S32,

  
  DWARF_VALUE_U64,

  
  DWARF_VALUE_S64,

  
  DWARF_VALUE_STR,

  
  DWARF_VALUE_PTR32,

  
  DWARF_VALUE_PTR64,

  
  DWARF_VALUE_BLOCK,
} DwarfValueType;

typedef struct Dwarf_Block {
  
  const void*   block_ptr;

  
  Elf_Word      block_size;
} Dwarf_Block;

typedef struct Dwarf_Value {
  union {
    Elf_Byte      u8;
    Elf_Sbyte     s8;
    Elf_Half      u16;
    Elf_Shalf     s16;
    Elf_Word      u32;
    Elf_Sword     s32;
    Elf_Xword     u64;
    Elf_Sxword    s64;
    Elf_Word      ptr32;
    Elf_Xword     ptr64;
    const char*   str;
    Dwarf_Block   block;
  };

  DwarfValueType  type;

  Elf_Word        encoded_size;
} Dwarf_Value;

typedef struct ELFF_PACKED Dwarf_Leb128 {
  
  Elf_Byte  val;

  void get_common(Dwarf_Value* value, bool sign) const {
    value->u64 = 0;
    
    if (val == 0) {
      value->type = sign ? DWARF_VALUE_S32 : DWARF_VALUE_U32;
      value->encoded_size = 1;
      return;
    }

    
    value->type = DWARF_VALUE_UNKNOWN;
    value->encoded_size = 0;

    const Elf_Byte* cur = &val;
    Elf_Word shift = 0;
    while ((*cur & 0x80) != 0) {
      value->u64 |= (static_cast<Elf_Xword>(*cur) & 0x7F) << shift;
      shift += 7;
      value->encoded_size++;
      cur++;
    }
    value->u64 |= (static_cast<Elf_Xword>(*cur) & 0x7F) << shift;
    value->encoded_size++;

    if ((value->u64 & 0xFFFFFFFF00000000LL) == 0) {
      
      if (sign) {
        value->type = DWARF_VALUE_S32;
        if (((*cur) & 0x40) != 0) {
          
          value->u64 |= - (1 << (shift + 7));
        } else if ((value->u32 & 0x80000000) != 0) {
          
          value->type = DWARF_VALUE_S64;
        }
      } else {
        value->type = DWARF_VALUE_U32;
      }
    } else {
      
      if (sign) {
        value->type = DWARF_VALUE_S64;
        if (((*cur) & 0x40) != 0) {
          
          value->u64 |= - (1 << (shift + 7));
        }
      } else {
        value->type = DWARF_VALUE_U64;
      }
    }
  }

  void get_unsigned(Dwarf_Value* value) const {
    get_common(value, false);
  }

  void get_signed(Dwarf_Value* value) const {
    get_common(value, true);
  }

  const void* process(Dwarf_Value* value, bool sign) const {
    get_common(value, sign);
    return INC_CPTR(&val, value->encoded_size);
  }

  const void* process_unsigned(Dwarf_Value* value) const {
    return process(value, false);
  }

  const void* process_signed(Dwarf_Value* value) const {
    return process(value, true);
  }
} Dwarf_Leb128;

typedef struct ELFF_PACKED Dwarf_Abbr_AT {
  Dwarf_Leb128  at;

  bool is_separator() const {
    return at.val == 0;
  }

  const Dwarf_Abbr_AT* process(Dwarf_At* at_value, Dwarf_Form* form) const {
    if (is_separator()) {
      
      *at_value = 0;
      *form = 0;
      return INC_CPTR_T(Dwarf_Abbr_AT, &at.val, 2);
    }

    Dwarf_Value val;

    
    const Dwarf_Leb128* next =
        reinterpret_cast<const Dwarf_Leb128*>(at.process_unsigned(&val));
    *at_value = val.u16;

    
    next = reinterpret_cast<const Dwarf_Leb128*>(next->process_unsigned(&val));
    *form = val.u16;
    return reinterpret_cast<const Dwarf_Abbr_AT*>(next);
  }
} Dwarf_Abbr_AT;

typedef struct ELFF_PACKED Dwarf_Abbr_DIE {
  
  Dwarf_Leb128  abbr_num;

  
  Dwarf_AbbrNum get_abbr_num() const {
    Dwarf_Value val;
    abbr_num.get_unsigned(&val);
    return val.u16;
  }

  
  Dwarf_Tag get_tag() const {
    Dwarf_Tag tag;
    process(NULL, &tag);
    return tag;
  }

  const Dwarf_Abbr_AT* process(Dwarf_AbbrNum* abbr_index,
                               Dwarf_Tag* tag) const {
    Dwarf_Value val;
    const Dwarf_Leb128* next =
        reinterpret_cast<const Dwarf_Leb128*>(abbr_num.process_unsigned(&val));
    if (abbr_index != NULL) {
      *abbr_index = val.u32;
    }

    
    next = reinterpret_cast<const Dwarf_Leb128*>(next->process_unsigned(&val));
    if (tag != NULL) {
      *tag = val.u16;
    }

    return INC_CPTR_T(Dwarf_Abbr_AT, next, 1);
  }
} Dwarf_Abbr_DIE;

typedef struct ELFF_PACKED Dwarf_DIE {
  Dwarf_Leb128  abbr_num;

  
  bool is_separator() const {
    return abbr_num.val == 0;
  }

  
  Dwarf_AbbrNum get_abbr_num() const {
    Dwarf_Value val;
    abbr_num.get_unsigned(&val);
    return val.u16;
  }

  const Elf_Byte* process(Dwarf_AbbrNum* abbr_number) const {
    if (is_separator()) {
      if (abbr_number != NULL) {
        *abbr_number = 0;
      }
      
      return INC_CPTR_T(Elf_Byte, &abbr_num.val, 1);
    }
    Dwarf_Value val;
    const void* ret = abbr_num.process_unsigned(&val);
    if (abbr_number != NULL) {
      *abbr_number = val.u32;
    }
    return reinterpret_cast<const Elf_Byte*>(ret);
  }
} Dwarf_DIE;


typedef struct ELFF_PACKED Dwarf32_SizeHdr {
  
  Elf_Word  size;
} Dwarf32_SizeHdr;

typedef struct ELFF_PACKED Dwarf64_SizeHdr {
  
  Elf_Word  size_selector;

  
  Elf_Xword   size;
} Dwarf64_SizeHdr;

template <typename Dwarf_SizeHdr, typename Elf_Off>
struct ELFF_PACKED Dwarf_CUHdr {
  
  Dwarf_SizeHdr   size_hdr;

  
  Elf_Half        version;

  Elf_Off         abbrev_offset;

  
  Elf_Byte        address_size;
};
typedef Dwarf_CUHdr<Dwarf32_SizeHdr, Elf_Word> Dwarf32_CUHdr;
typedef Dwarf_CUHdr<Dwarf64_SizeHdr, Elf_Xword> Dwarf64_CUHdr;

template <typename Dwarf_SizeHdr, typename Elf_Size>
struct ELFF_PACKED Dwarf_STMTLHdr {
  Dwarf_SizeHdr unit_length;

  Elf_Half      version;

  Elf_Size      header_length;

  Elf_Byte      min_instruction_len;

  
  Elf_Byte      default_is_stmt;

  
  Elf_Sbyte     line_base;

  
  Elf_Byte      line_range;

  
  Elf_Byte      opcode_base;

  Elf_Byte      standard_opcode_lengths;
};
typedef Dwarf_STMTLHdr<Dwarf32_SizeHdr, Elf_Word> Dwarf32_STMTLHdr;
typedef Dwarf_STMTLHdr<Dwarf64_SizeHdr, Elf_Xword> Dwarf64_STMTLHdr;

typedef struct ELFF_PACKED Dwarf_STMTL_FileDesc {
  
  char  file_name[1];

  
  bool is_last_entry() const {
    return file_name[0] == '\0';
  }

  
  const char* get_file_name() const {
    return file_name;
  }

  const Dwarf_STMTL_FileDesc* process(Elf_Word* dir_index) const {
    if (is_last_entry()) {
      return this;
    }

    
    Dwarf_Value tmp;
    const Dwarf_Leb128* leb =
        INC_CPTR_T(Dwarf_Leb128, file_name, strlen(file_name) + 1);
    leb = reinterpret_cast<const Dwarf_Leb128*>(leb->process_unsigned(&tmp));
    if (dir_index != NULL) {
      *dir_index = tmp.u32;
    }
    
    leb = reinterpret_cast<const Dwarf_Leb128*>(leb->process_unsigned(&tmp));
    
    return reinterpret_cast<const Dwarf_STMTL_FileDesc*>(leb->process_unsigned(&tmp));
  }

  
  Elf_Word get_dir_index() const {
    assert(!is_last_entry());
    if (is_last_entry()) {
      return 0;
    }
    
    Dwarf_Value ret;
    const Dwarf_Leb128* leb =
      INC_CPTR_T(Dwarf_Leb128, file_name, strlen(file_name) + 1);
    leb->process_unsigned(&ret);
    return ret.u32;
  }
} Dwarf_STMTL_FileDesc;

class DIEAttrib {
 public:
  
  DIEAttrib()
      : at_(0),
        form_(0) {
    value_.type = DWARF_VALUE_UNKNOWN;
  }

  
  ~DIEAttrib() {
  }

  
  Dwarf_At at() const {
    return at_;
  }

  
  Dwarf_Form form() const {
    return form_;
  }

  
  const Dwarf_Value* value() const {
    return &value_;
  }

  
  Dwarf_Value   value_;

  
  Dwarf_At      at_;

  
  Dwarf_Form    form_;
};

typedef struct DwarfParseContext {
  const Dwarf_Tag*  tags;
} DwarfParseContext;

static inline bool
collect_die(const DwarfParseContext* parse_context, Dwarf_Tag tag) {
  if (parse_context == NULL || parse_context->tags == NULL) {
    return true;
  }
  for (const Dwarf_Tag* tags = parse_context->tags; *tags != 0; tags++) {
    if (*tags == tag) {
      return true;
    }
  }
  return false;
}

class DwarfAbbrDieArray {
 public:
  DwarfAbbrDieArray()
      : array_(&small_array_[0]),
        array_size_(ELFF_ARRAY_SIZE(small_array_)),
        count_(0) {
  }

  
  ~DwarfAbbrDieArray() {
    if (array_ != &small_array_[0]) {
      delete[] array_;
    }
  }

  bool add(const Dwarf_Abbr_DIE* abbr, Dwarf_AbbrNum num) {
    assert(num != 0);
    if (num == 0) {
      
      _set_errno(EINVAL);
      return false;
    }

    if (num <= count_) {
      
      return true;
    }

    
    assert(num == (count_ + 1));
    if (num != (count_ + 1)) {
      _set_errno(EINVAL);
      return false;
    }

    if (num >= array_size_) {
      Dwarf_AbbrNum new_size = num + 64;

      
      const Dwarf_Abbr_DIE** new_array = new const Dwarf_Abbr_DIE*[new_size];
      assert(new_array != NULL);
      if (new_array == NULL) {
        _set_errno(ENOMEM);
        return false;
      }
      memcpy(new_array, array_, count_ * sizeof(const Dwarf_Abbr_DIE*));
      if (array_ != &small_array_[0]) {
        delete[] array_;
      }
      array_ = new_array;
      array_size_ = new_size;
    }

    
    array_[num - 1] = abbr;
    count_++;
    return true;
  }

  bool add(const Dwarf_Abbr_DIE* abbr) {
    return add(abbr, abbr->get_abbr_num());
  }

  const Dwarf_Abbr_DIE* get(Dwarf_AbbrNum num) const {
    assert(num != 0 && num <= count_);
    if (num != 0 && num <= count_) {
      return array_[num - 1];
    } else {
      _set_errno(EINVAL);
      return NULL;
    }
  }

  const Dwarf_Abbr_DIE* cache_to(Dwarf_AbbrNum num) {
    const Dwarf_Abbr_DIE* cur_abbr = get(count_);
    assert(cur_abbr != NULL);
    if (cur_abbr == NULL) {
      return NULL;
    }

    while (num > count_) {
      Dwarf_AbbrNum abbr_num;
      Dwarf_Tag tmp2;
      Dwarf_Form tmp3;
      Dwarf_At tmp4;

      const Dwarf_Abbr_AT* abbr_at = cur_abbr->process(&abbr_num, &tmp2);
      while (!abbr_at->is_separator()) {
        abbr_at = abbr_at->process(&tmp4, &tmp3);
      }

      
      cur_abbr = reinterpret_cast<const Dwarf_Abbr_DIE*>
                                              (abbr_at->process(&tmp4, &tmp3));
      if (!add(cur_abbr)) {
        return NULL;
      }
    }

    return array_[num - 1];
  }

  
  void empty() {
    if (array_ != &small_array_[0]) {
      delete[] array_;
      array_ = &small_array_[0];
      array_size_ = sizeof(small_array_) / sizeof(small_array_[0]);
    }
    count_ = 0;
  }

 protected:
  const Dwarf_Abbr_DIE*   small_array_[64];

  
  const Dwarf_Abbr_DIE**  array_;

  
  Dwarf_AbbrNum           array_size_;

  
  Dwarf_AbbrNum           count_;
};

class DwarfStateMachine {
 public:
  explicit DwarfStateMachine(bool set_is_stmt)
    : address_(0),
      file_(1),
      line_(1),
      column_(0),
      discriminator_(0),
      is_stmt_(set_is_stmt),
      basic_block_(false),
      end_sequence_(false),
      prologue_end_(false),
      epilogue_begin_(false),
      isa_(0),
      set_file_info_(NULL) {
  }

  
  ~DwarfStateMachine() {
  }

  void reset(bool set_is_stmt) {
    address_ = 0;
    file_ = 1;
    line_ = 1;
    column_ = 0;
    discriminator_ = 0;
    is_stmt_ = set_is_stmt;
    basic_block_ = false;
    end_sequence_ = false;
    prologue_end_ = false;
    epilogue_begin_ = false;
    isa_ = 0;
    set_file_info_ = NULL;
  }


  
  Elf_Xword                   address_;

  
  Elf_Word                    file_;

  
  Elf_Word                    line_;

  
  Elf_Word                    column_;

  
  Elf_Word                    discriminator_;

  
  bool                        is_stmt_;

  
  bool                        basic_block_;

  
  bool                        end_sequence_;

  
  bool                        prologue_end_;

  
  bool                        epilogue_begin_;

  
  Elf_Word                    isa_;

  const Dwarf_STMTL_FileDesc* set_file_info_;
};

static inline bool
dwarf_tag_is_routine(Dwarf_Tag tag) {
  return tag == DW_TAG_inlined_subroutine ||
         tag == DW_TAG_subprogram ||
         tag == DW_AT_main_subprogram;
}

static inline bool
dwarf_tag_is_cu(Dwarf_Tag tag) {
  return tag == DW_TAG_compile_unit ||
         tag == DW_TAG_partial_unit;
}

#endif  
