// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_MEM_RANKINGS_H__
#define NET_DISK_CACHE_MEM_RANKINGS_H__

#include "base/basictypes.h"

namespace disk_cache {

class MemEntryImpl;

class MemRankings {
 public:
  MemRankings() : head_(NULL), tail_(NULL) {}
  ~MemRankings();

  
  void Insert(MemEntryImpl* node);

  
  void Remove(MemEntryImpl* node);

  
  void UpdateRank(MemEntryImpl* node);

  
  MemEntryImpl* GetNext(MemEntryImpl* node);
  MemEntryImpl* GetPrev(MemEntryImpl* node);

 private:
  MemEntryImpl* head_;
  MemEntryImpl* tail_;

  DISALLOW_COPY_AND_ASSIGN(MemRankings);
};

}  

#endif  
