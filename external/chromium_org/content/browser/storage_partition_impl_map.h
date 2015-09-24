// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_STORAGE_PARTITION_MAP_H_
#define CONTENT_BROWSER_STORAGE_PARTITION_MAP_H_

#include <map>
#include <string>

#include "base/callback_forward.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/supports_user_data.h"
#include "content/browser/storage_partition_impl.h"
#include "content/public/browser/browser_context.h"

namespace base {
class FilePath;
class SequencedTaskRunner;
}  

namespace content {

class BrowserContext;

class StoragePartitionImplMap : public base::SupportsUserData::Data {
 public:
  explicit StoragePartitionImplMap(BrowserContext* browser_context);

  virtual ~StoragePartitionImplMap();

  
  StoragePartitionImpl* Get(const std::string& partition_domain,
                            const std::string& partition_name,
                            bool in_memory);

  
  
  
  
  
  
  void AsyncObliterate(const GURL& site, const base::Closure& on_gc_required);

  
  
  
  
  
  
  void GarbageCollect(scoped_ptr<base::hash_set<base::FilePath> > active_paths,
                      const base::Closure& done);

  void ForEach(const BrowserContext::StoragePartitionCallback& callback);

 private:
  FRIEND_TEST_ALL_PREFIXES(StoragePartitionConfigTest, OperatorLess);

  
  
  
  
  
  
  
  
  
  
  
  struct StoragePartitionConfig {
    const std::string partition_domain;
    const std::string partition_name;
    const bool in_memory;

    StoragePartitionConfig(const std::string& domain,
                               const std::string& partition,
                               const bool& in_memory_only)
      : partition_domain(domain),
        partition_name(partition),
        in_memory(in_memory_only) {}
  };

  
  struct StoragePartitionConfigLess {
    bool operator()(const StoragePartitionConfig& lhs,
                    const StoragePartitionConfig& rhs) const {
      if (lhs.partition_domain != rhs.partition_domain)
        return lhs.partition_domain < rhs.partition_domain;
      else if (lhs.partition_name != rhs.partition_name)
        return lhs.partition_name < rhs.partition_name;
      else if (lhs.in_memory != rhs.in_memory)
        return lhs.in_memory < rhs.in_memory;
      else
        return false;
    }
  };

  typedef std::map<StoragePartitionConfig,
                   StoragePartitionImpl*,
                   StoragePartitionConfigLess>
      PartitionMap;

  
  
  
  static base::FilePath GetStoragePartitionPath(
      const std::string& partition_domain,
      const std::string& partition_name);

  
  
  
  
  
  
  void PostCreateInitialization(StoragePartitionImpl* partition,
                                bool in_memory);

  BrowserContext* browser_context_;  
  scoped_refptr<base::SequencedTaskRunner> file_access_runner_;
  PartitionMap partitions_;

  
  
  bool resource_context_initialized_;
};

}  

#endif  
