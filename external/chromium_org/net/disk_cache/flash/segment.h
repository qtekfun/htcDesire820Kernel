// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_FLASH_SEGMENT_H_
#define NET_DISK_CACHE_FLASH_SEGMENT_H_

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "net/base/net_export.h"

namespace disk_cache {

class Storage;

// Once the entries are written to the Segment and Close() called on it and the
// object and recreate all the entries that were previously written to it.  To
// ReadData can be called over the range that was previously written with
// WriteData.  Reading from area that was not written will fail.

class NET_EXPORT_PRIVATE Segment {
 public:
  
  
  
  
  
  Segment(int32 index, bool read_only, Storage* storage);
  ~Segment();

  int32 index() const { return index_; }
  int32 write_offset() const { return write_offset_; }

  bool HaveOffset(int32 offset) const;
  std::vector<int32> GetOffsets() const { return offsets_; }

  
  void AddUser();
  void ReleaseUser();
  bool HasNoUsers() const;

  
  
  bool Init();

  
  
  bool WriteData(const void* buffer, int32 size);

  
  
  bool ReadData(void* buffer, int32 size, int32 offset) const;

  
  void StoreOffset(int32 offset);

  
  
  bool Close();

  
  bool CanHold(int32 size) const;

 private:
  int32 index_;
  int32 num_users_;
  bool read_only_;  // Indicates whether the segment can be written to.
  bool init_;  
  Storage* storage_;  
  const int32 offset_;  
  const int32 summary_offset_;  
  int32 write_offset_;  
  std::vector<int32> offsets_;

  DISALLOW_COPY_AND_ASSIGN(Segment);
};

}  

#endif  
