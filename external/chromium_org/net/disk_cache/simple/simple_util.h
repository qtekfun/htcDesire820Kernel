// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_UTIL_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

namespace base {
class FilePath;
class Time;
}

namespace disk_cache {

namespace simple_util {

NET_EXPORT_PRIVATE std::string ConvertEntryHashKeyToHexString(uint64 hash_key);

NET_EXPORT_PRIVATE std::string GetEntryHashKeyAsHexString(
    const std::string& key);

NET_EXPORT_PRIVATE uint64 GetEntryHashKey(const std::string& key);

NET_EXPORT_PRIVATE bool GetEntryHashKeyFromHexString(
    const base::StringPiece& hash_key,
    uint64* hash_key_out);

NET_EXPORT_PRIVATE std::string GetFilenameFromKeyAndFileIndex(
    const std::string& key,
    int file_index);

std::string GetFilenameFromEntryHashAndFileIndex(uint64 entry_hash,
                                                 int file_index);

std::string GetSparseFilenameFromEntryHash(uint64 entry_hash);

NET_EXPORT_PRIVATE int32 GetDataSizeFromKeyAndFileSize(const std::string& key,
                                                       int64 file_size);

NET_EXPORT_PRIVATE int64 GetFileSizeFromKeyAndDataSize(const std::string& key,
                                                       int32 data_size);

NET_EXPORT_PRIVATE int GetFileIndexFromStreamIndex(int stream_index);

NET_EXPORT_PRIVATE bool GetMTime(const base::FilePath& path,
                                 base::Time* out_mtime);
}  

}  

#endif  
