// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_ADDR_H_
#define NET_DISK_CACHE_ADDR_H_
#pragma once

#include "net/disk_cache/disk_format.h"

namespace disk_cache {

enum FileType {
  EXTERNAL = 0,
  RANKINGS = 1,
  BLOCK_256,
  BLOCK_1K,
  BLOCK_4K,
};

const int kMaxBlockSize = 4096 * 4;
const int kMaxBlockFile = 255;
const int kMaxNumBlocks = 4;
const int kFirstAdditionalBlockFile = 4;

class Addr {
 public:
  Addr() : value_(0) {}
  explicit Addr(CacheAddr address) : value_(address) {}
  Addr(FileType file_type, int max_blocks, int block_file, int index) {
    value_ = ((file_type << kFileTypeOffset) & kFileTypeMask) |
             (((max_blocks - 1) << kNumBlocksOffset) & kNumBlocksMask) |
             ((block_file << kFileSelectorOffset) & kFileSelectorMask) |
             (index  & kStartBlockMask) | kInitializedMask;
  }

  CacheAddr value() const { return value_; }
  void set_value(CacheAddr address) {
    value_ = address;
  }

  bool is_initialized() const {
    return (value_ & kInitializedMask) != 0;
  }

  bool is_separate_file() const {
    return (value_ & kFileTypeMask) == 0;
  }

  bool is_block_file() const {
    return !is_separate_file();
  }

  FileType file_type() const {
    return static_cast<FileType>((value_ & kFileTypeMask) >> kFileTypeOffset);
  }

  int FileNumber() const {
    if (is_separate_file())
      return value_ & kFileNameMask;
    else
      return ((value_ & kFileSelectorMask) >> kFileSelectorOffset);
  }

  int start_block() const;
  int num_blocks() const;
  bool SetFileNumber(int file_number);
  int BlockSize() const {
    return BlockSizeForFileType(file_type());
  }

  static int BlockSizeForFileType(FileType file_type) {
    switch (file_type) {
      case RANKINGS:
        return 36;
      case BLOCK_256:
        return 256;
      case BLOCK_1K:
        return 1024;
      case BLOCK_4K:
        return 4096;
      default:
        return 0;
    }
  }

  static FileType RequiredFileType(int size) {
    if (size < 1024)
      return BLOCK_256;
    else if (size < 4096)
      return BLOCK_1K;
    else if (size <= 4096 * 4)
      return BLOCK_4K;
    else
      return EXTERNAL;
  }

  
  bool SanityCheck() const;

 private:
  static const uint32 kInitializedMask    = 0x80000000;
  static const uint32 kFileTypeMask       = 0x70000000;
  static const uint32 kFileTypeOffset     = 28;
  static const uint32 kNumBlocksMask      = 0x03000000;
  static const uint32 kNumBlocksOffset    = 24;
  static const uint32 kFileSelectorMask   = 0x00ff0000;
  static const uint32 kFileSelectorOffset = 16;
  static const uint32 kStartBlockMask     = 0x0000FFFF;
  static const uint32 kFileNameMask       = 0x0FFFFFFF;

  CacheAddr value_;
};

}  

#endif  
