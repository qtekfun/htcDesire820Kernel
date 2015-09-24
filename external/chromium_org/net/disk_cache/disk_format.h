// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_DISK_FORMAT_H_
#define NET_DISK_CACHE_DISK_FORMAT_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/disk_cache/disk_format_base.h"

namespace disk_cache {

const int kIndexTablesize = 0x10000;
const uint32 kIndexMagic = 0xC103CAC3;
const uint32 kCurrentVersion = 0x20000;  

struct LruData {
  int32     pad1[2];
  int32     filled;          
  int32     sizes[5];
  CacheAddr heads[5];
  CacheAddr tails[5];
  CacheAddr transaction;     
  int32     operation;       
  int32     operation_list;  
  int32     pad2[7];
};

struct NET_EXPORT_PRIVATE IndexHeader {
  IndexHeader();

  uint32      magic;
  uint32      version;
  int32       num_entries;   
  int32       num_bytes;     
  int32       last_file;     
  int32       this_id;       
  CacheAddr   stats;         
  int32       table_len;     
  int32       crash;         
  int32       experiment;    
  uint64      create_time;   
  int32       pad[52];
  LruData     lru;           
};

struct Index {
  IndexHeader header;
  CacheAddr   table[kIndexTablesize];  
                                       
};

struct EntryStore {
  uint32      hash;               
  CacheAddr   next;               
  CacheAddr   rankings_node;      
  int32       reuse_count;        
  int32       refetch_count;      
  int32       state;              
  uint64      creation_time;
  int32       key_len;
  CacheAddr   long_key;           
  int32       data_size[4];       
  CacheAddr   data_addr[4];       
  uint32      flags;              
  int32       pad[4];
  uint32      self_hash;          
  char        key[256 - 24 * 4];  
};

COMPILE_ASSERT(sizeof(EntryStore) == 256, bad_EntyStore);
const int kMaxInternalKeyLength = 4 * sizeof(EntryStore) -
                                  offsetof(EntryStore, key) - 1;

enum EntryState {
  ENTRY_NORMAL = 0,
  ENTRY_EVICTED,    
  ENTRY_DOOMED      
};

enum EntryFlags {
  PARENT_ENTRY = 1,         
  CHILD_ENTRY = 1 << 1      
};

#pragma pack(push, 4)
struct RankingsNode {
  uint64      last_used;        
  uint64      last_modified;    
  CacheAddr   next;             
  CacheAddr   prev;             
  CacheAddr   contents;         
  int32       dirty;            
  uint32      self_hash;        
};
#pragma pack(pop)

COMPILE_ASSERT(sizeof(RankingsNode) == 36, bad_RankingsNode);

}  

#endif  
