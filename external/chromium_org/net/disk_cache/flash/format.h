// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_FLASH_FORMAT_H_
#define NET_DISK_CACHE_FLASH_FORMAT_H_

namespace disk_cache {

const int32 kFlashPageSize = 8 * 1024;
const int32 kFlashBlockSize = 512 * kFlashPageSize;

const int32 kFlashSegmentSize = 4 * 1024 * 1024;
const int32 kFlashSmallEntrySize = 4 * 1024;
const size_t kFlashMaxEntryCount = kFlashSegmentSize / kFlashSmallEntrySize - 1;

const int32 kFlashSummarySize = (1 + kFlashMaxEntryCount) * sizeof(int32);
const int32 kFlashSegmentFreeSpace = kFlashSegmentSize - kFlashSummarySize;

const int32 kFlashLogStoreEntryNumStreams = 4;
const int32 kFlashLogStoreEntryHeaderSize =
    kFlashLogStoreEntryNumStreams * sizeof(int32);

}  

#endif  
