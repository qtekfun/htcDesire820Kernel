// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_PARTIAL_DATA_H_
#define NET_HTTP_PARTIAL_DATA_H_

#include "base/basictypes.h"
#include "net/base/completion_callback.h"
#include "net/http/http_byte_range.h"
#include "net/http/http_request_headers.h"

namespace disk_cache {
class Entry;
}

namespace net {

class HttpResponseHeaders;
class IOBuffer;

class PartialData {
 public:
  PartialData();
  ~PartialData();

  
  
  
  bool Init(const HttpRequestHeaders& headers);

  
  
  
  void SetHeaders(const HttpRequestHeaders& headers);

  
  
  void RestoreHeaders(HttpRequestHeaders* headers) const;

  
  
  
  
  
  
  int ShouldValidateCache(disk_cache::Entry* entry,
                          const CompletionCallback& callback);

  
  
  void PrepareCacheValidation(disk_cache::Entry* entry,
                              HttpRequestHeaders* headers);

  
  bool IsCurrentRangeCached() const;

  
  
  bool IsLastRange() const;

  
  
  
  bool UpdateFromStoredHeaders(const HttpResponseHeaders* headers,
                               disk_cache::Entry* entry, bool truncated);

  
  void SetRangeToStartDownload();

  
  bool IsRequestedRangeOK();

  
  bool ResponseHeadersOK(const HttpResponseHeaders* headers);

  
  
  
  void FixResponseHeaders(HttpResponseHeaders* headers, bool success);

  
  void FixContentLength(HttpResponseHeaders* headers);

  
  
  
  
  
  int CacheRead(disk_cache::Entry* entry, IOBuffer* data, int data_len,
                const net::CompletionCallback& callback);

  
  
  int CacheWrite(disk_cache::Entry* entry, IOBuffer* data, int data_len,
                 const net::CompletionCallback& callback);

  
  
  void OnCacheReadCompleted(int result);

  
  
  void OnNetworkReadCompleted(int result);

  bool initial_validation() const { return initial_validation_; }

 private:
  class Core;
  
  int GetNextRangeLen();

  
  void GetAvailableRangeCompleted(int result, int64 start);

  int64 current_range_start_;
  int64 cached_start_;
  int64 resource_size_;
  int cached_min_len_;
  HttpByteRange byte_range_;  
  
  HttpRequestHeaders extra_headers_;
  bool range_present_;  
  bool final_range_;
  bool sparse_entry_;
  bool truncated_;  
  bool initial_validation_;  
  Core* core_;
  CompletionCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(PartialData);
};

}  

#endif  
