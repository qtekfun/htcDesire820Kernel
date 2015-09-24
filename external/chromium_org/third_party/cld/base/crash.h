// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_CRASH_H_
#define BASE_CRASH_H_

namespace base {

struct CrashReason {
  CrashReason() : filename(0), line_number(0), message(0), depth(0) {}

  const char* filename;
  int line_number;
  const char* message;

  
  
  void* stack[32];
  int depth;

  
  
  
  
  char trace_info[512];
};

void SetCrashReason(const CrashReason* reason);

const CrashReason* GetCrashReason();

}  

#endif  
