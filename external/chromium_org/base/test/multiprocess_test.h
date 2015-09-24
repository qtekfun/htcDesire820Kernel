// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_MULTIPROCESS_TEST_H_
#define BASE_TEST_MULTIPROCESS_TEST_H_

#include <string>

#include "base/basictypes.h"
#include "base/process/launch.h"
#include "base/process/process_handle.h"
#include "build/build_config.h"
#include "testing/platform_test.h"

class CommandLine;

namespace base {

class MultiProcessTest : public PlatformTest {
 public:
  MultiProcessTest();

 protected:
  
  
  
  
  
  
  
  
  
  
  
  ProcessHandle SpawnChild(const std::string& procname, bool debug_on_start);

  
  
  
  ProcessHandle SpawnChildWithOptions(const std::string& procname,
                                      const LaunchOptions& options,
                                      bool debug_on_start);

#if defined(OS_POSIX)
  
  
  ProcessHandle SpawnChild(const std::string& procname,
                           const FileHandleMappingVector& fds_to_map,
                           bool debug_on_start);
#endif

  
  virtual CommandLine MakeCmdLine(const std::string& procname,
                                  bool debug_on_start);

 private:
  DISALLOW_COPY_AND_ASSIGN(MultiProcessTest);
};

}  

#endif  
