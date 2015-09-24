// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_BLOOM_FILTER_H_
#define CHROME_BROWSER_SAFE_BROWSING_BLOOM_FILTER_H_
#pragma once

#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/safe_browsing/safe_browsing_util.h"

class FilePath;

class BloomFilter : public base::RefCountedThreadSafe<BloomFilter> {
 public:
  typedef uint64 HashKey;
  typedef std::vector<HashKey> HashKeys;

  
  explicit BloomFilter(int bit_size);

  
  
  BloomFilter(char* data, int size, const HashKeys& keys);

  void Insert(SBPrefix hash);
  bool Exists(SBPrefix hash) const;

  const char* data() const { return data_.get(); }
  int size() const { return byte_size_; }

  
  static BloomFilter* LoadFile(const FilePath& filter_name);
  bool WriteFile(const FilePath& filter_name) const;

  
  static const int kBloomFilterSizeRatio = 25;

  
  
  static const int kBloomFilterMinSize = 250000;

  
  
  static const int kBloomFilterMaxSize = 3 * 1024 * 1024;

  
  
  
  
  
  static int FilterSizeForKeyCount(int key_count);

 private:
  friend class base::RefCountedThreadSafe<BloomFilter>;
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingBloomFilter, BloomFilterUse);
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingBloomFilter, BloomFilterFile);

  static const int kNumHashKeys = 20;
  static const int kFileVersion = 1;

  
  
  enum FailureType {
    FAILURE_FILTER_READ_OPEN,
    FAILURE_FILTER_READ_VERSION,
    FAILURE_FILTER_READ_NUM_KEYS,
    FAILURE_FILTER_READ_KEY,
    FAILURE_FILTER_READ_DATA_MINSIZE,
    FAILURE_FILTER_READ_DATA_MAXSIZE,
    FAILURE_FILTER_READ_DATA_SHORT,
    FAILURE_FILTER_READ_DATA,

    
    
    FAILURE_FILTER_MAX
  };

  static void RecordFailure(FailureType failure_type);

  ~BloomFilter();

  int byte_size_;  
  int bit_size_;   
  scoped_array<char> data_;

  
  HashKeys hash_keys_;

  DISALLOW_COPY_AND_ASSIGN(BloomFilter);
};

#endif  
