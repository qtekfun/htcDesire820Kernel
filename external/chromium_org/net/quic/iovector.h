// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_IOVECTOR_H_
#define NET_QUIC_IOVECTOR_H_

#include <stddef.h>
#include <algorithm>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "net/base/iovec.h"
#include "net/base/net_export.h"

namespace net {

inline size_t TotalIovecLength(const struct iovec* iov, size_t iovcnt) {
  size_t length = 0;
  if (iov != NULL) {
    for (size_t i = 0; i < iovcnt; ++i) {
      length += iov[i].iov_len;
    }
  }
  return length;
}

// be read or written together and in order. It does not take ownership of the
//   int bytes_written = writev(fd, iovector.iovec(), iovector.Size());
//   if (bytes_written > 0)
//     iovector.Consume(bytes_written);
// memory not yet written to.

class NET_EXPORT_PRIVATE IOVector {
 public:
  
  
  IOVector();
  ~IOVector();

  
  
  void AppendIovec(const struct iovec* iov, size_t iovcnt) {
    for (size_t i = 0; i < iovcnt; ++i)
      Append(static_cast<char*>(iov[i].iov_base), iov[i].iov_len);
  }

  
  size_t AppendIovecAtMostBytes(const struct iovec* iov,
                                size_t iovcnt,
                                size_t max_bytes) {
    size_t bytes_appended = 0;
    for (size_t i = 0; i < iovcnt && max_bytes > 0; ++i) {
      const size_t length = std::min(max_bytes, iov[i].iov_len);
      Append(static_cast<char*>(iov[i].iov_base), length);
      max_bytes -= length;
      bytes_appended += length;
    }
    return bytes_appended;
  }

  
  
  
  void Append(char* buffer, size_t length) {
    if (buffer != NULL && length > 0) {
      if (iovec_.size() > 0) {
        struct iovec& last = iovec_.back();
        
        if (static_cast<char*>(last.iov_base) + last.iov_len == buffer) {
          last.iov_len += length;
          return;
        }
      }
      struct iovec tmp = {buffer, length};
      iovec_.push_back(tmp);
    }
  }

  
  
  void AppendNoCoalesce(char* buffer, size_t length) {
    if (buffer != NULL && length > 0) {
      struct iovec tmp = {buffer, length};
      iovec_.push_back(tmp);
    }
  }

  
  
  
  
  
  size_t Consume(size_t length) {
    if (length == 0) return 0;

    size_t bytes_to_consume = length;
    std::vector<struct iovec>::iterator iter = iovec_.begin();
    std::vector<struct iovec>::iterator end = iovec_.end();
    for (; iter < end && bytes_to_consume >= iter->iov_len; ++iter) {
      bytes_to_consume -= iter->iov_len;
    }
    iovec_.erase(iovec_.begin(), iter);
    if (iovec_.size() > 0 && bytes_to_consume != 0) {
      iovec_[0].iov_base =
          static_cast<char*>(iovec_[0].iov_base) + bytes_to_consume;
      iovec_[0].iov_len -= bytes_to_consume;
      return length;
    }
    if (iovec_.size() == 0 && bytes_to_consume > 0) {
      LOG(DFATAL) << "Attempting to consume " << bytes_to_consume
                  << " non-existent bytes.";
    }
    
    
    return length - bytes_to_consume;
  }

  
  
  void Clear() { iovec_.clear(); }

  
  void Swap(IOVector* other) { iovec_.swap(other->iovec_); }

  
  
  size_t Size() const { return iovec_.size(); }

  
  
  size_t Capacity() const { return iovec_.capacity(); }

  
  bool Empty() const { return iovec_.empty(); }

  
  
  
  struct iovec* iovec() { return !Empty() ? &iovec_[0] : NULL; }

  
  const struct iovec* iovec() const { return !Empty() ? &iovec_[0] : NULL; }

  
  
  const char* LastBlockEnd() const {
    return iovec_.size() > 0 ?
        static_cast<char *>(iovec_.back().iov_base) + iovec_.back().iov_len :
        NULL;
  }

  
  size_t TotalBufferSize() const { return TotalIovecLength(iovec(), Size()); }

  void Resize(size_t count) {
    iovec_.resize(count);
  }

 private:
  std::vector<struct iovec> iovec_;

  
  
  
  
  
  
  
};

}  

#endif  
