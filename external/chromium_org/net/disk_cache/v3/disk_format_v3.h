// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_V3_DISK_FORMAT_V3_H_
#define NET_DISK_CACHE_V3_DISK_FORMAT_V3_H_

#include "base/basictypes.h"
#include "net/disk_cache/disk_format_base.h"

namespace disk_cache {

const int kBaseTableLen = 0x10000;
const uint32 kIndexMagicV3 = 0xC103CAC3;
const uint32 kVersion3 = 0x30000;  

enum CacheFlags {
  CACHE_EVICTION_2 = 1,      
  CACHE_EVICTED = 1 << 1     
};

struct IndexHeaderV3 {
  uint32      magic;
  uint32      version;
  int32       num_entries;   
  int32       num_bytes;     
  int32       last_file;     
  int32       reserved1;
  CacheAddr   stats;         
  int32       table_len;     
  int32       crash;         
  int32       experiment;    
  int32       max_bytes;     
  uint32      flags;
  int32       used_cells;
  int32       max_bucket;
  uint64      create_time;   
  uint64      base_time;     
  uint64      old_time;      
  int32       max_block_file;
  int32       num_no_use_entries;
  int32       num_low_use_entries;
  int32       num_high_use_entries;
  int32       reserved;
  int32       num_evicted_entries;
  int32       pad[6];
};

const int kBaseBitmapBytes = 3968;
struct IndexBitmap {
  IndexHeaderV3   header;
  uint32          bitmap[kBaseBitmapBytes / 4];  
};
COMPILE_ASSERT(sizeof(IndexBitmap) == 4096, bad_IndexHeader);

enum EntryState {
  ENTRY_FREE = 0,   
  ENTRY_NEW,        
  ENTRY_OPEN,       
  ENTRY_MODIFIED,   
  ENTRY_DELETED,    
  ENTRY_FIXING,     
  ENTRY_USED        
};
COMPILE_ASSERT(ENTRY_USED <= 7, state_uses_3_bits);

enum EntryGroup {
  ENTRY_NO_USE = 0,   
  ENTRY_LOW_USE,      
  ENTRY_HIGH_USE,     
  ENTRY_RESERVED,     
  ENTRY_EVICTED       
};
COMPILE_ASSERT(ENTRY_USED <= 7, group_uses_3_bits);

#pragma pack(push, 1)
struct IndexCell {
  void Clear() { memset(this, 0, sizeof(*this)); }

  uint64      address : 22;
  uint64      hash : 18;
  uint64      timestamp : 20;
  uint64      reuse : 4;
  uint8       state : 3;
  uint8       group : 3;
  uint8       sum : 2;
};
COMPILE_ASSERT(sizeof(IndexCell) == 9, bad_IndexCell);

struct IndexBucket {
  IndexCell   cells[4];
  int32       next;
  uint32      hash : 24;      
  uint32      reserved : 8;   
};
COMPILE_ASSERT(sizeof(IndexBucket) == 44, bad_IndexBucket);
const int kBytesPerCell = 44 / 4;

struct Index {
  
  IndexBucket table[kBaseTableLen / 4];
};
#pragma pack(pop)

enum EntryFlags {
  PARENT_ENTRY = 1,         
  CHILD_ENTRY = 1 << 1      
};

struct EntryRecord {
  uint32      hash;
  uint32      pad1;
  uint8       reuse_count;
  uint8       refetch_count;
  int8        state;              
  uint8       flags;              
  int32       key_len;
  int32       data_size[4];       
  CacheAddr   data_addr[4];       
  uint32      data_hash[4];
  uint64      creation_time;
  uint64      last_modified_time;
  uint64      last_access_time;
  int32       pad[3];
  uint32      self_hash;
};
COMPILE_ASSERT(sizeof(EntryRecord) == 104, bad_EntryRecord);

struct ShortEntryRecord {
  uint32      hash;
  uint32      pad1;
  uint8       reuse_count;
  uint8       refetch_count;
  int8        state;              
  uint8       flags;
  int32       key_len;
  uint64      last_access_time;
  uint32      long_hash[5];
  uint32      self_hash;
};
COMPILE_ASSERT(sizeof(ShortEntryRecord) == 48, bad_ShortEntryRecord);

}  

#endif  
