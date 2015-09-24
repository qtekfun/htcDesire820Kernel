// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_BROWSER_SHUTDOWN_PROFILE_DUMPER_H_
#define CONTENT_BROWSER_BROWSER_SHUTDOWN_PROFILE_DUMPER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "content/common/content_export.h"

namespace base {
class FilePath;
class WaitableEvent;
}

namespace content {

// Use the class with a scoped_ptr to get files written in the destructor.
class BrowserShutdownProfileDumper {
 public:
  BrowserShutdownProfileDumper();

  ~BrowserShutdownProfileDumper();

 private:
  
  void WriteTracesToDisc(const base::FilePath& file_name);

  void EndTraceAndFlush(base::WaitableEvent* flush_complete_event);

  
  base::FilePath GetFileName();

  
  
  void WriteTraceDataCollected(
      base::WaitableEvent* flush_complete_event,
      const scoped_refptr<base::RefCountedString>& events_str,
      bool has_more_events);

  
  bool IsFileValid();

  
  void WriteString(const std::string& string);

  
  void WriteChars(const char* chars, size_t size);

  
  void CloseFile();

  // The number of blocks we have already written.
  int blocks_;
  
  FILE* dump_file_;

  DISALLOW_COPY_AND_ASSIGN(BrowserShutdownProfileDumper);
};

}  

#endif  
