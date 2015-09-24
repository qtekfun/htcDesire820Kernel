// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_PARENT_CHILD_INDEX
#define SYNC_SYNCABLE_PARENT_CHILD_INDEX

#include <map>
#include <set>

#include "base/basictypes.h"
#include "sync/base/sync_export.h"

namespace syncer {
namespace syncable {

struct EntryKernel;
class Id;
class ParentChildIndex;

struct SYNC_EXPORT_PRIVATE ChildComparator {
  bool operator() (const EntryKernel* a, const EntryKernel* b) const;
};

typedef std::set<EntryKernel*, ChildComparator> OrderedChildSet;

class SYNC_EXPORT_PRIVATE ParentChildIndex {
 public:
  ParentChildIndex();
  ~ParentChildIndex();

  
  
  static bool ShouldInclude(const EntryKernel* e);

  
  bool Insert(EntryKernel* e);

  
  void Remove(EntryKernel* e);

  
  bool Contains(EntryKernel* e) const;

  
  
  const OrderedChildSet* GetChildren(const Id& id);

 private:
  typedef std::map<syncable::Id, OrderedChildSet*> ParentChildrenMap;

  
  
  ParentChildrenMap parent_children_map_;

  DISALLOW_COPY_AND_ASSIGN(ParentChildIndex);
};

}  
}  

#endif  
