// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_BACKEND_VERSION_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_BACKEND_VERSION_H_

namespace disk_cache {

const uint32 kSimpleVersion = 6;

// The version of the entry file(s) as written to disk. Must be updated iff the
const uint32 kSimpleEntryVersionOnDisk = 5;

}  

#endif  
