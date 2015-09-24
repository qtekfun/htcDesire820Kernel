// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_STREAMS_STREAM_REGISTRY_H_
#define CONTENT_BROWSER_STREAMS_STREAM_REGISTRY_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {

class Stream;

class CONTENT_EXPORT StreamRegistry : public base::NonThreadSafe {
 public:
  StreamRegistry();
  virtual ~StreamRegistry();

  
  void RegisterStream(scoped_refptr<Stream> stream);

  
  
  bool CloneStream(const GURL& url, const GURL& src_url);

  void UnregisterStream(const GURL& url);

  
  
  
  
  
  
  
  bool UpdateMemoryUsage(const GURL& url, size_t current_size, size_t increase);

  
  
  scoped_refptr<Stream> GetStream(const GURL& url);

  void set_max_memory_usage_for_testing(size_t size) {
    max_memory_usage_ = size;
  }

 private:
  typedef std::map<GURL, scoped_refptr<Stream> > StreamMap;

  StreamMap streams_;

  size_t total_memory_usage_;

  
  
  size_t max_memory_usage_;

  DISALLOW_COPY_AND_ASSIGN(StreamRegistry);
};

}  

#endif  
