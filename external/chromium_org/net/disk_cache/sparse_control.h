// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SPARSE_CONTROL_H_
#define NET_DISK_CACHE_SPARSE_CONTROL_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/completion_callback.h"
#include "net/disk_cache/bitmap.h"
#include "net/disk_cache/disk_format.h"

namespace net {
class IOBuffer;
class DrainableIOBuffer;
}

namespace disk_cache {

class Entry;
class EntryImpl;

class SparseControl {
 public:
  typedef net::CompletionCallback CompletionCallback;

  
  enum SparseOperation {
    kNoOperation,
    kReadOperation,
    kWriteOperation,
    kGetRangeOperation
  };

  explicit SparseControl(EntryImpl* entry);
  ~SparseControl();

  
  
  
  int Init();

  
  
  bool CouldBeSparse() const;

  
  
  
  
  
  // number of bytes read or written, or a net error code.
  int StartIO(SparseOperation op, int64 offset, net::IOBuffer* buf,
              int buf_len, const CompletionCallback& callback);

  
  int GetAvailableRange(int64 offset, int len, int64* start);

  
  void CancelIO();

  
  
  
  int ReadyToUse(const CompletionCallback& completion_callback);

  
  static void DeleteChildren(EntryImpl* entry);

 private:
  
  
  
  
  int CreateSparseEntry();
  int OpenSparseEntry(int data_len);

  
  
  
  bool OpenChild();
  void CloseChild();
  std::string GenerateChildKey();

  
  bool KillChildAndContinue(const std::string& key, bool fatal);

  
  bool ContinueWithoutChild(const std::string& key);

  
  
  bool ChildPresent();

  
  
  void SetChildBit(bool value);

  
  void WriteSparseData();

  
  
  
  
  bool VerifyRange();

  
  
  void UpdateRange(int result);

  
  
  int PartialBlockLength(int block_index) const;

  
  void InitChildData();

  
  void DoChildrenIO();

  
  
  
  bool DoChildIO();

  
  int DoGetAvailableRange();

  
  void DoChildIOCompleted(int result);

  
  void OnChildIOCompleted(int result);

  
  void DoUserCallback();
  void DoAbortCallbacks();

  EntryImpl* entry_;  
  EntryImpl* child_;  
  SparseOperation operation_;
  bool pending_;  
  bool finished_;
  bool init_;
  bool range_found_;  
  bool abort_;  

  SparseHeader sparse_header_;  
  Bitmap children_map_;  
  SparseData child_data_;  
  Bitmap child_map_;  

  CompletionCallback user_callback_;
  std::vector<CompletionCallback> abort_callbacks_;
  int64 offset_;  
  scoped_refptr<net::DrainableIOBuffer> user_buf_;
  int buf_len_;  
  int child_offset_;  
  int child_len_;  
  int result_;

  DISALLOW_COPY_AND_ASSIGN(SparseControl);
};

}  

#endif  
