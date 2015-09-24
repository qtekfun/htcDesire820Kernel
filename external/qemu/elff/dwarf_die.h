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


#ifndef ELFF_DWARF_DIE_H_
#define ELFF_DWARF_DIE_H_

#include "dwarf_defs.h"
#include "elf_alloc.h"

class ElfFile;
class DwarfCU;

class DIEObject : public DwarfAllocBase {
 public:
  DIEObject(const Dwarf_DIE* die, DwarfCU* parent_cu, DIEObject* parent_die)
      : die_(die),
        parent_cu_(parent_cu),
        parent_die_(parent_die),
        last_child_(NULL),
        prev_sibling_(NULL) {
  }

  
  ~DIEObject();

  
  ElfFile* elf_file() const;

  
  Dwarf_Tag get_tag() const;

  const char* get_name() const;

  bool get_attrib(Dwarf_At at, DIEAttrib* attr) const;

  DIEObject* get_leaf_for_address(Elf_Xword address);

  DIEObject* find_die_object(const Dwarf_DIE* die_to_find);

  void dump(bool only_this) const;

 protected:
  template <typename AddrType>
  bool contains_address(Elf_Xword address);

  const Elf_Byte* advance(const Dwarf_Abbr_AT** at_abbr, Dwarf_Tag* tag) const;

 public:
  
  const Dwarf_DIE* die() const {
    return die_;
  }

  
  DwarfCU* parent_cu() const {
    return parent_cu_;
  }

  
  DIEObject* parent_die() const {
    return parent_die_;
  }

  DIEObject* last_child() const {
    return last_child_;
  }

  
  void link_child(DIEObject* child) {
    last_child_ = child;
  }

  
  DIEObject* prev_sibling() const {
    return prev_sibling_;
  }

  
  void link_sibling(DIEObject* sibl) {
    prev_sibling_ = sibl;
  }

  bool is_cu_die() const {
    return parent_die_ == NULL;
  }

  Elf_Word get_level() const {
    return parent_die_ != NULL ? parent_die_->get_level() + 1 : 0;
  }

 protected:
  
  const Dwarf_DIE*  die_;

  
  DwarfCU*          parent_cu_;

  
  DIEObject*        parent_die_;

  DIEObject*        last_child_;

  
  DIEObject*        prev_sibling_;
};

#endif  
