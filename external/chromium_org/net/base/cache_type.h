// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CACHE_TYPE_H_
#define NET_BASE_CACHE_TYPE_H_

namespace net {

enum CacheType {
  DISK_CACHE,  
  MEMORY_CACHE,  
  MEDIA_CACHE,  
  APP_CACHE,  
  SHADER_CACHE, 
  PNACL_CACHE, 
};

enum BackendType {
  CACHE_BACKEND_DEFAULT,
  CACHE_BACKEND_BLOCKFILE,  
  CACHE_BACKEND_SIMPLE  
};

}  

#endif  
