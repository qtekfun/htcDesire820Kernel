// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_POSIX_FILE_DESCRIPTOR_SHUFFLE_H_
#define BASE_POSIX_FILE_DESCRIPTOR_SHUFFLE_H_


#include <vector>

#include "base/base_export.h"
#include "base/compiler_specific.h"

namespace base {

class InjectionDelegate {
 public:
  
  
  virtual bool Duplicate(int* result, int fd) = 0;
  
  
  virtual bool Move(int src, int dest) = 0;
  
  virtual void Close(int fd) = 0;

 protected:
  virtual ~InjectionDelegate() {}
};

class BASE_EXPORT FileDescriptorTableInjection : public InjectionDelegate {
  virtual bool Duplicate(int* result, int fd) OVERRIDE;
  virtual bool Move(int src, int dest) OVERRIDE;
  virtual void Close(int fd) OVERRIDE;
};

struct InjectionArc {
  InjectionArc(int in_source, int in_dest, bool in_close)
      : source(in_source),
        dest(in_dest),
        close(in_close) {
  }

  int source;
  int dest;
  bool close;  
               
};

typedef std::vector<InjectionArc> InjectiveMultimap;

BASE_EXPORT bool PerformInjectiveMultimap(const InjectiveMultimap& map,
                                          InjectionDelegate* delegate);

BASE_EXPORT bool PerformInjectiveMultimapDestructive(
    InjectiveMultimap* map,
    InjectionDelegate* delegate);

static inline bool ShuffleFileDescriptors(InjectiveMultimap* map) {
  FileDescriptorTableInjection delegate;
  return PerformInjectiveMultimapDestructive(map, &delegate);
}

}  

#endif  
