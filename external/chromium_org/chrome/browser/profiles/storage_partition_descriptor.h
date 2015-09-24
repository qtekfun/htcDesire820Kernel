// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_STORAGE_PARTITION_DESCRIPTOR_H_
#define CHROME_BROWSER_PROFILES_STORAGE_PARTITION_DESCRIPTOR_H_

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"

struct StoragePartitionDescriptor {
  StoragePartitionDescriptor(const base::FilePath& partition_path,
                             const bool in_memory_only)
    : path(partition_path),
      in_memory(in_memory_only) {}

  const base::FilePath path;
  const bool in_memory;
};

struct StoragePartitionDescriptorLess {
  bool operator()(const StoragePartitionDescriptor& lhs,
                  const StoragePartitionDescriptor& rhs) const {
    if (lhs.path != rhs.path)
      return lhs.path < rhs.path;
    else if (lhs.in_memory != rhs.in_memory)
      return lhs.in_memory < rhs.in_memory;
    else
      return false;
  }
};

#endif  
