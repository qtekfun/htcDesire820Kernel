// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_ERRORS_H__
#define NET_DISK_CACHE_ERRORS_H__

namespace disk_cache {

enum {
  ERR_NO_ERROR = 0,
  ERR_INIT_FAILED = -1,
  ERR_INVALID_TAIL = -2,
  ERR_INVALID_HEAD = -3,
  ERR_INVALID_PREV = -4,
  ERR_INVALID_NEXT = -5,
  ERR_INVALID_ENTRY = -6,
  ERR_INVALID_ADDRESS = -7,
  ERR_INVALID_LINKS = -8,
  ERR_NUM_ENTRIES_MISMATCH = -9,
  ERR_READ_FAILURE = -10,
  ERR_PREVIOUS_CRASH = -11,
  ERR_STORAGE_ERROR = -12,
  ERR_INVALID_MASK = -13,
  ERR_CACHE_DOOMED = -14,   
  ERR_CACHE_CREATED = -15   
};

}  

#endif  
