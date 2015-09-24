// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_UPLOAD_DATA_H_
#define NET_BASE_UPLOAD_DATA_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/supports_user_data.h"
#include "net/base/net_export.h"
#include "net/base/upload_element.h"

namespace base {
class FilePath;
class Time;
}  

namespace net {

class NET_EXPORT UploadData
    : public base::RefCounted<UploadData>,
      public base::SupportsUserData {
 public:
  UploadData();

  void AppendBytes(const char* bytes, int bytes_len);

  void AppendFileRange(const base::FilePath& file_path,
                       uint64 offset, uint64 length,
                       const base::Time& expected_modification_time);

  
  
  void set_is_chunked(bool set) { is_chunked_ = set; }
  bool is_chunked() const { return is_chunked_; }

  
  void set_last_chunk_appended(bool set) { last_chunk_appended_ = set; }
  bool last_chunk_appended() const { return last_chunk_appended_; }

  const ScopedVector<UploadElement>& elements() const {
    return elements_;
  }

  ScopedVector<UploadElement>* elements_mutable() {
    return &elements_;
  }

  void swap_elements(ScopedVector<UploadElement>* elements) {
    elements_.swap(*elements);
  }

  
  
  
  void set_identifier(int64 id) { identifier_ = id; }
  int64 identifier() const { return identifier_; }

 private:
  friend class base::RefCounted<UploadData>;

  virtual ~UploadData();

  ScopedVector<UploadElement> elements_;
  int64 identifier_;
  bool is_chunked_;
  bool last_chunk_appended_;

  DISALLOW_COPY_AND_ASSIGN(UploadData);
};

}  

#endif  
