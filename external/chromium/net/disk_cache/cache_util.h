// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_CACHE_UTIL_H_
#define NET_DISK_CACHE_CACHE_UTIL_H_
#pragma once

#include "base/basictypes.h"

class FilePath;

namespace disk_cache {

bool MoveCache(const FilePath& from_path, const FilePath& to_path);

void DeleteCache(const FilePath& path, bool remove_folder);

bool DeleteCacheFile(const FilePath& name);

}  

#endif  
