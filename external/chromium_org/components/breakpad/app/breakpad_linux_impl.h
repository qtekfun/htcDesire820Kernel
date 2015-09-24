// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef COMPONENTS_BREAKPAD_APP_BREAKPAD_LINUX_IMPL_H_
#define COMPONENTS_BREAKPAD_APP_BREAKPAD_LINUX_IMPL_H_

#include <sys/types.h>

#include "base/basictypes.h"
#include "breakpad/src/common/simple_string_dictionary.h"
#include "components/breakpad/app/breakpad_linux.h"

namespace breakpad {

typedef google_breakpad::NonAllocatingMap<256, 256, 64> CrashKeyStorage;

static const size_t kDistroSize = 128;
#if defined(ADDRESS_SANITIZER)
static const size_t kMaxAsanReportSize = 1 << 16;
#endif
static const off_t kMaxMinidumpFileSize = 1258291;

#if !defined(ADDRESS_SANITIZER)
const size_t kCrashIovSize = 7;
#else
const size_t kCrashIovSize = 8;
#endif

struct BreakpadInfo {
  int fd;                          
  const char* filename;            
#if defined(ADDRESS_SANITIZER)
  const char* log_filename;        
  const char* asan_report_str;     
  unsigned asan_report_length;     
#endif
  const char* process_type;        
  unsigned process_type_length;    
  const char* distro;              
  unsigned distro_length;          
  bool upload;                     
  uint64_t process_start_time;     
  size_t oom_size;                 
  uint64_t pid;                    
  CrashKeyStorage* crash_keys;
};

extern void HandleCrashDump(const BreakpadInfo& info);

}  

#endif  
