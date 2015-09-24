// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYNC_SOCKET_H_
#define BASE_SYNC_SOCKET_H_
#pragma once


#include "base/basictypes.h"
#if defined(OS_WIN)
#include <windows.h>
#endif
#include <sys/types.h>

#include "base/base_api.h"

namespace base {

class BASE_API SyncSocket {
 public:
#if defined(OS_WIN)
  typedef HANDLE Handle;
#else
  typedef int Handle;
#endif

  
  explicit SyncSocket(Handle handle) : handle_(handle) { }
  ~SyncSocket() { Close(); }

  
  
  
  static bool CreatePair(SyncSocket* pair[2]);

  
  bool Close();

  
  
  
  
  
  
  size_t Send(const void* buffer, size_t length);

  
  
  
  
  size_t Receive(void* buffer, size_t length);

  
  
  
  
  size_t Peek();

  
  
  Handle handle() const { return handle_; }

 private:
  Handle handle_;

  DISALLOW_COPY_AND_ASSIGN(SyncSocket);
};

}  

#endif  
