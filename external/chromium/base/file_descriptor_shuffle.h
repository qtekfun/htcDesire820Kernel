// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILE_DESCRIPTOR_SHUFFLE_H_
#define BASE_FILE_DESCRIPTOR_SHUFFLE_H_
#pragma once


#include <vector>

namespace base {

class InjectionDelegate {
 public:
  
  
  virtual bool Duplicate(int* result, int fd) = 0;
  
  
  virtual bool Move(int src, int dest) = 0;
  
  virtual void Close(int fd) = 0;

 protected:
  virtual ~InjectionDelegate() {}
};

class FileDescriptorTableInjection : public InjectionDelegate {
  virtual bool Duplicate(int* result, int fd);
  virtual bool Move(int src, int dest);
  virtual void Close(int fd);
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

bool PerformInjectiveMultimap(const InjectiveMultimap& map,
                              InjectionDelegate* delegate);

bool PerformInjectiveMultimapDestructive(InjectiveMultimap* map,
                                         InjectionDelegate* delegate);

static inline bool ShuffleFileDescriptors(InjectiveMultimap* map) {
  FileDescriptorTableInjection delegate;
  return PerformInjectiveMultimapDestructive(map, &delegate);
}

}  

#endif  
