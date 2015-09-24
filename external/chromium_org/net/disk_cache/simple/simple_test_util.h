// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_TEST_UTIL_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_TEST_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"

namespace base {
class FilePath;
}

namespace disk_cache {
namespace simple_util {

template <typename T, size_t Size>
class ImmutableArray {
 public:
  static const size_t size = Size;

  ImmutableArray(const base::Callback<T (size_t index)>& initializer) {
    for (size_t i = 0; i < size; ++i)
      data_[i] = initializer.Run(i);
  }

  template <size_t Index>
  const T& at() const {
    COMPILE_ASSERT(Index < size, array_out_of_bounds);
    return data_[Index];
  }

 private:
  T data_[size];
};

bool CreateCorruptFileForTests(const std::string& key,
                               const base::FilePath& cache_path);

}  
}  

#endif  
