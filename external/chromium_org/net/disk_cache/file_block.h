// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_FILE_BLOCK_H__
#define NET_DISK_CACHE_FILE_BLOCK_H__

namespace disk_cache {

class FileBlock {
 public:
  virtual ~FileBlock() {}

  
  virtual void* buffer() const = 0;

  
  virtual size_t size() const = 0;

  
  virtual int offset() const = 0;
};

}  

#endif  
