// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef TOOLS_MEMORY_WATCHER_MEMORY_WATCHER_
#define TOOLS_MEMORY_WATCHER_MEMORY_WATCHER_

#include <map>
#include <functional>

#include "base/synchronization/lock.h"
#include "tools/memory_watcher/memory_hook.h"

class CallStack;
class AllocationStack;

class MemoryWatcher : MemoryObserver {
 public:
  struct StackTrack {
    CallStack* stack;
    int count;
    int size;
  };

  typedef std::map<int32, AllocationStack*, std::less<int32>,
                   PrivateHookAllocator<int32> > CallStackMap;
  typedef std::map<int32, StackTrack, std::less<int32>,
                   PrivateHookAllocator<int32> > CallStackIdMap;
  typedef std::basic_string<char, std::char_traits<char>,
                            PrivateHookAllocator<char> > PrivateAllocatorString;

  MemoryWatcher();
  virtual ~MemoryWatcher();

  
  void DumpLeaks();

  
  virtual void OnTrack(HANDLE heap, int32 id, int32 size);
  virtual void OnUntrack(HANDLE heap, int32 id, int32 size);

  
  void SetLogName(char* log_name);

 private:
  
  void OpenLogFile();

  
  void CloseLogFile();

  
  void Hook();

  
  void Unhook();

  
  
  bool LockedRecursionDetected() const;

  
  FILE* file_;

  bool hooked_;  

  
  
  DWORD active_thread_id_;

  base::Lock block_map_lock_;
  
  
  
  CallStackMap *block_map_;

  
  std::string file_name_;

  
  
  std::string log_name_;
};



#endif  
