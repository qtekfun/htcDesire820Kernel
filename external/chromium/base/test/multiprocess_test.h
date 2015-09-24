// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_MULTIPROCESS_TEST_H_
#define BASE_TEST_MULTIPROCESS_TEST_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/process.h"
#include "base/process_util.h"
#include "build/build_config.h"
#include "testing/platform_test.h"

class CommandLine;

namespace base {

class MultiProcessTest : public PlatformTest {
 public:
  MultiProcessTest();

 protected:
  
  
  
  
  
  
  
  
  
  
  
  ProcessHandle SpawnChild(const std::string& procname, bool debug_on_start);

#if defined(OS_POSIX)
  ProcessHandle SpawnChild(const std::string& procname,
                           const file_handle_mapping_vector& fds_to_map,
                           bool debug_on_start);
#endif

  
  virtual CommandLine MakeCmdLine(const std::string& procname,
                                  bool debug_on_start);

 private:
#if defined(OS_WIN)
  ProcessHandle SpawnChildImpl(const std::string& procname,
                               bool debug_on_start);

#elif defined(OS_POSIX)
  
  
  ProcessHandle SpawnChildImpl(const std::string& procname,
                               const file_handle_mapping_vector& fds_to_map,
                               bool debug_on_start);
#endif

  DISALLOW_COPY_AND_ASSIGN(MultiProcessTest);
};

}  

#endif  
