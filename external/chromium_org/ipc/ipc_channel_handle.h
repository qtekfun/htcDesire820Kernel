// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_CHANNEL_HANDLE_H_
#define IPC_IPC_CHANNEL_HANDLE_H_

#include <string>

#include "build/build_config.h"

#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#elif defined(OS_WIN)
#include <windows.h>
#endif  



namespace IPC {

struct ChannelHandle {
  
  
  ChannelHandle() {}
  
  
  
  ChannelHandle(const std::string& n) : name(n) {}
  ChannelHandle(const char* n) : name(n) {}
#if defined(OS_WIN)
  explicit ChannelHandle(HANDLE h) : pipe(h) {}
#elif defined(OS_POSIX)
  ChannelHandle(const std::string& n, const base::FileDescriptor& s)
      : name(n), socket(s) {}
#endif  

  std::string name;
#if defined(OS_POSIX)
  base::FileDescriptor socket;
#elif defined(OS_WIN)
  
  struct PipeHandle {
    PipeHandle() : handle(NULL) {}
    PipeHandle(HANDLE h) : handle(h) {}
    HANDLE handle;
  };
  PipeHandle pipe;
#endif  
};

}  

#endif  
