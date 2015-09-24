// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_COMMON_TEST_MULTIPROCESS_TEST_BASE_H_
#define MOJO_COMMON_TEST_MULTIPROCESS_TEST_BASE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/process/process_handle.h"
#include "base/test/multiprocess_test.h"
#include "mojo/system/platform_channel.h"
#include "testing/multiprocess_func_list.h"

namespace mojo {
namespace test {

class MultiprocessTestBase : public base::MultiProcessTest {
 public:
  MultiprocessTestBase();
  virtual ~MultiprocessTestBase();

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  
  
  void StartChild(const std::string& test_child_name);
  
  
  
  
  
  
  int WaitForChildShutdown();

  
  static void ChildSetup();

  
  scoped_ptr<system::PlatformServerChannel> platform_server_channel;

  
  static scoped_ptr<system::PlatformClientChannel> platform_client_channel;

 private:
  virtual CommandLine MakeCmdLine(const std::string& procname,
                                  bool debug_on_start) OVERRIDE;

  
  base::ProcessHandle test_child_handle_;

  DISALLOW_COPY_AND_ASSIGN(MultiprocessTestBase);
};

#define MOJO_MULTIPROCESS_TEST_CHILD_MAIN(test_child_name) \
    MULTIPROCESS_TEST_MAIN_WITH_SETUP( \
        test_child_name ## TestChildMain, \
        ::mojo::test::MultiprocessTestBase::ChildSetup)

}  
}  

#endif  
