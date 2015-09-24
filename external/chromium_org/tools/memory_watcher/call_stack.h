// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_MEMORY_WATCHER_CALL_STACK_H_
#define TOOLS_MEMORY_WATCHER_CALL_STACK_H_

#include <windows.h>
#include <dbghelp.h>
#include <functional>
#include <map>
#include <string>

#include "base/logging.h"
#include "base/synchronization/lock.h"
#include "tools/memory_watcher/memory_watcher.h"

class CallStack {
 public:
  
  static bool Initialize();

  CallStack();
  virtual ~CallStack() {}

  
  
  int32 hash() { return hash_; }

  
  
  
  int32 id() { return id_; }

  
  DWORD_PTR frame(int32 index) {
    DCHECK(index < frame_count_ && index >= 0);
    return frames_[index];
  }

  
  
  
  bool IsEqual(const CallStack &target);

  typedef std::basic_string<char, std::char_traits<char>,
                            PrivateHookAllocator<char> > PrivateAllocatorString;

  
  void CallStack::ToString(PrivateAllocatorString* output);

  
  bool Valid() const { return valid_; }

 private:
  
  static const int kMaxTraceFrames = 32;

  
  void AddFrame(DWORD_PTR programcounter);

  
  
  bool GetStackTrace();

  
  void ClearFrames();

  
  
  static bool LoadDbgHelp();

  static void LockDbgHelp() {
    dbghelp_lock_.Acquire();
    active_thread_id_ = GetCurrentThreadId();
  }

  static void UnlockDbgHelp() {
    active_thread_id_ = 0;
    dbghelp_lock_.Release();
  }

  class AutoDbgHelpLock {
  public:
    AutoDbgHelpLock() {
      CallStack::LockDbgHelp();
    }
    ~AutoDbgHelpLock() {
      CallStack::UnlockDbgHelp();
    }
  };

  
  bool LockedRecursionDetected() const;

  
  
  
  
  
  
  
  
  
  static base::Lock dbghelp_lock_;

  
  
  
  static bool dbghelp_loaded_;

  
  
  
  
  static DWORD active_thread_id_;

  int frame_count_;  
  DWORD_PTR frames_[kMaxTraceFrames];
  int32 hash_;
  int32 id_;

  
  
  bool valid_;

  
  
  typedef std::map<int32, PrivateAllocatorString, std::less<int32>,
                   PrivateHookAllocator<int32> > SymbolCache;
  static SymbolCache* symbol_cache_;

  DISALLOW_COPY_AND_ASSIGN(CallStack);
};

class AllocationStack : public CallStack {
 public:
  explicit AllocationStack(int32 size)
      : next_(NULL), size_(size), CallStack() {}

  
  void* operator new(size_t s);
  void operator delete(void*p);

  int32 size() const { return size_; }

 private:
  AllocationStack* next_;     
  int32 size_;                
  static AllocationStack* freelist_;
  static base::Lock freelist_lock_;

  DISALLOW_COPY_AND_ASSIGN(AllocationStack);
};

#endif  
