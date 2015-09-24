// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_BUILDER_RECORD_H_
#define TOOLS_GN_BUILDER_RECORD_H_

#include <set>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "tools/gn/item.h"
#include "tools/gn/location.h"

class ParseNode;

class BuilderRecord {
 public:
  typedef std::set<BuilderRecord*> BuilderRecordSet;

  enum ItemType {
    ITEM_UNKNOWN,
    ITEM_TARGET,
    ITEM_CONFIG,
    ITEM_TOOLCHAIN
  };

  
  BuilderRecord(ItemType type, const Label& label);
  ~BuilderRecord();

  ItemType type() const { return type_; }
  const Label& label() const { return label_; }

  
  static const char* GetNameForType(ItemType type);

  
  static bool IsItemOfType(const Item* item, ItemType type);

  
  static ItemType TypeOfItem(const Item* item);

  Item* item() { return item_.get(); }
  const Item* item() const { return item_.get(); }
  void set_item(scoped_ptr<Item> item) { item_ = item.Pass(); }

  
  
  
  const ParseNode* originally_referenced_from() const {
    return originally_referenced_from_;
  }
  void set_originally_referenced_from(const ParseNode* pn) {
    originally_referenced_from_ = pn;
  }

  bool should_generate() const { return should_generate_; }
  void set_should_generate(bool sg) { should_generate_ = sg; }

  bool resolved() const { return resolved_; }
  void set_resolved(bool r) { resolved_ = r; }

  bool can_resolve() const {
    return item_ && unresolved_deps_.empty();
  }

  
  BuilderRecordSet& all_deps() { return all_deps_; }
  const BuilderRecordSet& all_deps() const { return all_deps_; }

  
  BuilderRecordSet& unresolved_deps() { return unresolved_deps_; }
  const BuilderRecordSet& unresolved_deps() const { return unresolved_deps_; }

  
  
  BuilderRecordSet& waiting_on_resolution() { return waiting_on_resolution_; }
  const BuilderRecordSet& waiting_on_resolution() const {
    return waiting_on_resolution_;
  }

  void AddDep(BuilderRecord* record);

 private:
  ItemType type_;
  Label label_;
  scoped_ptr<Item> item_;
  const ParseNode* originally_referenced_from_;
  bool should_generate_;
  bool resolved_;

  BuilderRecordSet all_deps_;
  BuilderRecordSet unresolved_deps_;
  BuilderRecordSet waiting_on_resolution_;

  DISALLOW_COPY_AND_ASSIGN(BuilderRecord);
};

#endif  
