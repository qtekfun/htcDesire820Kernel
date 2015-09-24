// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_SELECTION_UTILS_H_
#define UI_BASE_X_SELECTION_UTILS_H_

#include <X11/Xlib.h>

#undef RootWindow

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted_memory.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/x/x11_atom_cache.h"

namespace ui {
class SelectionData;
class X11AtomCache;

extern const char kMimeTypeMozillaURL[];
extern const char kString[];
extern const char kText[];
extern const char kUtf8String[];

UI_EXPORT std::vector< ::Atom> GetTextAtomsFrom(const X11AtomCache* atom_cache);

UI_EXPORT std::vector< ::Atom> GetURLAtomsFrom(const X11AtomCache* atom_cache);

UI_EXPORT void GetAtomIntersection(const std::vector< ::Atom>& desired,
                                   const std::vector< ::Atom>& offered,
                                   std::vector< ::Atom>* output);

UI_EXPORT void AddString16ToVector(const string16& str,
                                   std::vector<unsigned char>* bytes);

UI_EXPORT std::string RefCountedMemoryToString(
    const scoped_refptr<base::RefCountedMemory>& memory);

UI_EXPORT string16 RefCountedMemoryToString16(
    const scoped_refptr<base::RefCountedMemory>& memory);


class UI_EXPORT SelectionFormatMap {
 public:
  
  typedef std::map< ::Atom, scoped_refptr<base::RefCountedMemory> > InternalMap;
  typedef InternalMap::const_iterator const_iterator;

  SelectionFormatMap();
  ~SelectionFormatMap();
  

  
  
  void Insert(::Atom atom, const scoped_refptr<base::RefCountedMemory>& item);

  
  ui::SelectionData GetFirstOf(
      const std::vector< ::Atom>& requested_types) const;

  
  std::vector< ::Atom> GetTypes() const;

  
  const_iterator begin() const { return data_.begin(); }
  const_iterator end() const { return data_.end(); }
  const_iterator find(::Atom atom) const { return data_.find(atom); }
  size_t size() const { return data_.size(); }

 private:
  InternalMap data_;
};


class UI_EXPORT SelectionData {
 public:
  
  SelectionData();
  SelectionData(::Atom type,
                const scoped_refptr<base::RefCountedMemory>& memory);
  SelectionData(const SelectionData& rhs);
  ~SelectionData();
  SelectionData& operator=(const SelectionData& rhs);

  bool IsValid() const;
  ::Atom GetType() const;
  const unsigned char* GetData() const;
  size_t GetSize() const;

  
  std::string GetText() const;

  
  
  string16 GetHtml() const;

  
  void AssignTo(std::string* result) const;
  void AssignTo(string16* result) const;

 private:
  ::Atom type_;
  scoped_refptr<base::RefCountedMemory> memory_;

  X11AtomCache atom_cache_;
};

}  

#endif  
