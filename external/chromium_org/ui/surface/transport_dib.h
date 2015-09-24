// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SURFACE_TRANSPORT_DIB_H_
#define UI_SURFACE_TRANSPORT_DIB_H_

#include "base/basictypes.h"
#include "ui/surface/surface_export.h"

#if !defined(TOOLKIT_GTK)
#include "base/memory/shared_memory.h"
#endif

#if defined(OS_WIN)
#include <windows.h>
#elif defined(TOOLKIT_GTK)
#include "ui/base/x/x11_util.h"
#include "ui/gfx/x/x11_types.h"
#endif

class SkCanvas;

class SURFACE_EXPORT TransportDIB {
 public:
  ~TransportDIB();

  
  
  
  
#if defined(OS_WIN)
  typedef HANDLE Handle;
  
  
  
  
  
  
  
  
  struct HandleAndSequenceNum {
    HandleAndSequenceNum()
        : handle(NULL),
          sequence_num(0) {
    }

    HandleAndSequenceNum(HANDLE h, uint32 seq_num)
        : handle(h),
          sequence_num(seq_num) {
    }

    bool operator==(const HandleAndSequenceNum& other) const {
      return other.handle == handle && other.sequence_num == sequence_num;
    }

    bool operator<(const HandleAndSequenceNum& other) const {
      
      if (other.handle != handle)
        return other.handle < handle;
      return other.sequence_num < sequence_num;
    }

    HANDLE handle;
    uint32 sequence_num;
  };
  typedef HandleAndSequenceNum Id;

  
  
  static Handle DefaultHandleValue() { return NULL; }

  
  
  static Handle GetFakeHandleForTest() {
    static int fake_handle = 10;
    return reinterpret_cast<Handle>(fake_handle++);
  }
#elif defined(TOOLKIT_GTK)
  typedef int Handle;  
  struct Id {
    
    Id() : shmkey(-1) {
    }

    bool operator<(const Id& other) const {
      return shmkey < other.shmkey;
    }

    bool operator==(const Id& other) const {
      return shmkey == other.shmkey;
    }

    int shmkey;
  };

  
  
  static Handle DefaultHandleValue() { return -1; }

  
  
  static Handle GetFakeHandleForTest() {
    static int fake_handle = 10;
    return fake_handle++;
  }
#else  
  typedef base::SharedMemoryHandle Handle;
  
#if defined(OS_ANDROID)
  typedef base::SharedMemoryHandle Id;
#else
  typedef base::SharedMemoryId Id;
#endif

  
  
  static Handle DefaultHandleValue() { return Handle(); }

  
  
  static Handle GetFakeHandleForTest() {
    static int fake_handle = 10;
    return Handle(fake_handle++, false);
  }
#endif

  
  
  
  
  
  
  
  
  
  static TransportDIB* Create(size_t size, uint32 sequence_num);

  
  
  static TransportDIB* Map(Handle transport_dib);

  
  
  static TransportDIB* CreateWithHandle(Handle handle);

  
  static bool is_valid_handle(Handle dib);

  
  static bool is_valid_id(Id id);

  
  
  
  
  
  
  
  
  
  
  SkCanvas* GetPlatformCanvas(int w, int h);

  
  
  bool Map();

  
  void* memory() const;

  
  
  
  size_t size() const { return size_; }

  
  
  Id id() const;

  
  
  Handle handle() const;

#if defined(TOOLKIT_GTK)
  
  
  XID MapToX(XDisplay* connection);

  void IncreaseInFlightCounter() { inflight_counter_++; }
  
  
  void DecreaseInFlightCounter();

  
  
  void Detach();
#endif

 private:
  TransportDIB();

  
  bool VerifyCanvasSize(int w, int h);

#if defined(TOOLKIT_GTK)
  Id key_;  
  void* address_;  
  XSharedMemoryId x_shm_;  
  XDisplay* display_;  
  size_t inflight_counter_;  
  bool detached_;  
#else
  explicit TransportDIB(base::SharedMemoryHandle dib);
  base::SharedMemory shared_memory_;
  uint32 sequence_num_;
#endif
  size_t size_;  

  DISALLOW_COPY_AND_ASSIGN(TransportDIB);
};

#endif  
