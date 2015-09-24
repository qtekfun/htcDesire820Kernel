// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_RANKINGS_H_
#define NET_DISK_CACHE_RANKINGS_H_
#pragma once

#include <list>

#include "base/memory/scoped_ptr.h"
#include "net/disk_cache/addr.h"
#include "net/disk_cache/mapped_file.h"
#include "net/disk_cache/storage_block.h"

namespace disk_cache {

class BackendImpl;

enum RankCrashes {
  NO_CRASH = 0,
  INSERT_EMPTY_1,
  INSERT_EMPTY_2,
  INSERT_EMPTY_3,
  INSERT_ONE_1,
  INSERT_ONE_2,
  INSERT_ONE_3,
  INSERT_LOAD_1,
  INSERT_LOAD_2,
  REMOVE_ONE_1,
  REMOVE_ONE_2,
  REMOVE_ONE_3,
  REMOVE_ONE_4,
  REMOVE_HEAD_1,
  REMOVE_HEAD_2,
  REMOVE_HEAD_3,
  REMOVE_HEAD_4,
  REMOVE_TAIL_1,
  REMOVE_TAIL_2,
  REMOVE_TAIL_3,
  REMOVE_LOAD_1,
  REMOVE_LOAD_2,
  REMOVE_LOAD_3,
  MAX_CRASH
};

class Rankings {
 public:
  
  enum List {
    NO_USE = 0,   
    LOW_USE,      
    HIGH_USE,     
    RESERVED,     
    DELETED,      
    LAST_ELEMENT
  };

  
  
  
  class ScopedRankingsBlock : public scoped_ptr<CacheRankingsBlock> {
   public:
    ScopedRankingsBlock() : rankings_(NULL) {}
    explicit ScopedRankingsBlock(Rankings* rankings) : rankings_(rankings) {}
    ScopedRankingsBlock(Rankings* rankings, CacheRankingsBlock* node)
        : scoped_ptr<CacheRankingsBlock>(node), rankings_(rankings) {}

    ~ScopedRankingsBlock() {
      rankings_->FreeRankingsBlock(get());
    }

    void set_rankings(Rankings* rankings) {
      rankings_ = rankings;
    }

    
    void reset(CacheRankingsBlock* p = NULL) {
      if (p != get())
        rankings_->FreeRankingsBlock(get());
      scoped_ptr<CacheRankingsBlock>::reset(p);
    }

   private:
    Rankings* rankings_;
    DISALLOW_COPY_AND_ASSIGN(ScopedRankingsBlock);
  };

  
  
  struct Iterator {
    explicit Iterator(Rankings* rankings);
    ~Iterator();

    List list;                     
    CacheRankingsBlock* nodes[3];  
    Rankings* my_rankings;
  };

  Rankings();
  ~Rankings();

  bool Init(BackendImpl* backend, bool count_lists);

  
  void Reset();

  
  void Insert(CacheRankingsBlock* node, bool modified, List list);

  
  
  
  
  
  void Remove(CacheRankingsBlock* node, List list, bool strict);

  
  void UpdateRank(CacheRankingsBlock* node, bool modified, List list);

  
  CacheRankingsBlock* GetNext(CacheRankingsBlock* node, List list);
  CacheRankingsBlock* GetPrev(CacheRankingsBlock* node, List list);
  void FreeRankingsBlock(CacheRankingsBlock* node);

  
  void TrackRankingsBlock(CacheRankingsBlock* node, bool start_tracking);

  
  
  int SelfCheck();

  
  
  bool SanityCheck(CacheRankingsBlock* node, bool from_list) const;
  bool DataSanityCheck(CacheRankingsBlock* node, bool from_list) const;

  
  void SetContents(CacheRankingsBlock* node, CacheAddr address);

 private:
  typedef std::pair<CacheAddr, CacheRankingsBlock*> IteratorPair;
  typedef std::list<IteratorPair> IteratorList;

  void ReadHeads();
  void ReadTails();
  void WriteHead(List list);
  void WriteTail(List list);

  
  
  
  bool GetRanking(CacheRankingsBlock* rankings);

  
  void ConvertToLongLived(CacheRankingsBlock* rankings);

  
  void CompleteTransaction();
  void FinishInsert(CacheRankingsBlock* rankings);
  void RevertRemove(CacheRankingsBlock* rankings);

  
  
  bool CheckEntry(CacheRankingsBlock* rankings);

  
  
  bool CheckLinks(CacheRankingsBlock* node, CacheRankingsBlock* prev,
                  CacheRankingsBlock* next, List* list);

  
  bool CheckSingleLink(CacheRankingsBlock* prev, CacheRankingsBlock* next);

  
  
  int CheckList(List list);

  
  
  bool IsHead(CacheAddr addr, List* list) const;
  bool IsTail(CacheAddr addr, List* list) const;

  
  void UpdateIterators(CacheRankingsBlock* node);

  
  void InvalidateIterators(CacheRankingsBlock* node);

  
  void IncrementCounter(List list);
  void DecrementCounter(List list);

  bool init_;
  bool count_lists_;
  Addr heads_[LAST_ELEMENT];
  Addr tails_[LAST_ELEMENT];
  BackendImpl* backend_;
  LruData* control_data_;  
  IteratorList iterators_;

  DISALLOW_COPY_AND_ASSIGN(Rankings);
};

}  

#endif  
