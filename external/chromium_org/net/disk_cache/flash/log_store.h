// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_FLASH_LOG_STORE_H_
#define NET_DISK_CACHE_FLASH_LOG_STORE_H_

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "net/base/net_export.h"
#include "net/disk_cache/flash/storage.h"

namespace disk_cache {

class Segment;

// entry, a new version must be written.  Only one entry can be written to at
class NET_EXPORT_PRIVATE LogStore {
 public:
  LogStore(const base::FilePath& path, int32 size);
  ~LogStore();

  
  
  bool Init();

  
  bool Close();

  
  
  bool CreateEntry(int32 size, int32* entry_id);

  
  
  
  void DeleteEntry(int32 entry_id, int32 size);

  
  bool WriteData(const void* buffer, int32 size);

  
  bool OpenEntry(int32 entry_id);

  
  
  bool ReadData(int32 entry_id, void* buffer, int32 size, int32 offset) const;

  
  
  void CloseEntry(int32 id);

 private:
  FRIEND_TEST_ALL_PREFIXES(FlashCacheTest, LogStoreReadFromClosedSegment);
  FRIEND_TEST_ALL_PREFIXES(FlashCacheTest, LogStoreSegmentSelectionIsFifo);
  FRIEND_TEST_ALL_PREFIXES(FlashCacheTest, LogStoreInUseSegmentIsSkipped);
  FRIEND_TEST_ALL_PREFIXES(FlashCacheTest, LogStoreReadFromCurrentAfterClose);

  int32 GetNextSegmentIndex();
  bool InUse(int32 segment_index) const;

  Storage storage_;

  int32 num_segments_;

  
  
  std::vector<Segment*> open_segments_;

  // The index of the segment currently being written to.  It's an index to
  
  int32 write_index_;

  
  std::set<int32> open_entries_;

  // Id of the entry that is currently being written to, -1 if there is no entry
  // currently being written to.
  int32 current_entry_id_;

  // Number of bytes left to be written to the entry identified by
  
  
  int32 current_entry_num_bytes_left_to_write_;

  bool init_;  
  bool closed_;  

  DISALLOW_COPY_AND_ASSIGN(LogStore);
};

}  

#endif  
