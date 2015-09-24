// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_GLOBAL_DESCRIPTORS_POSIX_H_
#define BASE_GLOBAL_DESCRIPTORS_POSIX_H_
#pragma once

#include "build/build_config.h"

#include <vector>
#include <utility>

#include <stdint.h>

#include "base/memory/singleton.h"

namespace base {

class GlobalDescriptors {
 public:
  typedef uint32_t Key;
  typedef std::vector<std::pair<Key, int> > Mapping;

  
  
  static const int kBaseDescriptor = 3;  

  
  static GlobalDescriptors* GetInstance();

  
  int Get(Key key) const;

  
  int MaybeGet(Key key) const;

  
  void Set(Key key, int fd);

  void Reset(const Mapping& mapping);

 private:
  friend struct DefaultSingletonTraits<GlobalDescriptors>;
  GlobalDescriptors();
  ~GlobalDescriptors();

  Mapping descriptors_;
};

}  

#endif  
