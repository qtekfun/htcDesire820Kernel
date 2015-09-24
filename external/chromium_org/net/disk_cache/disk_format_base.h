// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_DISK_FORMAT_BASE_H_
#define NET_DISK_CACHE_DISK_FORMAT_BASE_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace disk_cache {

typedef uint32 CacheAddr;

const uint32 kBlockVersion2 = 0x20000;  
const uint32 kBlockCurrentVersion = 0x30000;  

const uint32 kBlockMagic = 0xC104CAC3;
const int kBlockHeaderSize = 8192;  
const int kMaxBlocks = (kBlockHeaderSize - 80) * 8;

typedef uint32 AllocBitmap[kMaxBlocks / 32];

struct BlockFileHeader {
  uint32          magic;
  uint32          version;
  int16           this_file;    
  int16           next_file;    
  int32           entry_size;   
  int32           num_entries;  
  int32           max_entries;  
  int32           empty[4];     
  int32           hints[4];     
  volatile int32  updating;     
  int32           user[5];
  AllocBitmap     allocation_map;
};

COMPILE_ASSERT(sizeof(BlockFileHeader) == kBlockHeaderSize, bad_header);

// Each child entry keeps track of all the 1 KB blocks that have been written
// read that spans data not written before. The actual sparse data is stored in

struct SparseHeader {
  int64 signature;          
  uint32 magic;             
  int32 parent_key_len;     
  int32 last_block;         // Index of the last written block.
  int32 last_block_len;     // Lenght of the last written block.
  int32 dummy[10];
};

struct SparseData {
  SparseHeader header;
  uint32 bitmap[32];        
                            
                            
                            
                            
};

const int kNumSparseBits = 1024;
COMPILE_ASSERT(sizeof(SparseData) == sizeof(SparseHeader) + kNumSparseBits / 8,
               Invalid_SparseData_bitmap);

}  

#endif  
