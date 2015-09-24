// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PPAPI_UNITTEST_H_
#define CONTENT_TEST_PPAPI_UNITTEST_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {
class MessageLoop;
}

namespace content {

class PepperPluginInstanceImpl;
class PluginModule;

class PpapiUnittest : public testing::Test {
 public:
  PpapiUnittest();
  virtual ~PpapiUnittest();

  virtual void SetUp();
  virtual void TearDown();

  PluginModule* module() const { return module_.get(); }
  PepperPluginInstanceImpl* instance() const { return instance_.get(); }

  
  
  virtual const void* GetMockInterface(const char* interface_name) const;

  
  void ShutdownModule();

  
  void SetViewSize(int width, int height) const;

 private:
  
  scoped_refptr<PluginModule> module_;
  scoped_refptr<PepperPluginInstanceImpl> instance_;

  scoped_ptr<base::MessageLoop> message_loop_;

  DISALLOW_COPY_AND_ASSIGN(PpapiUnittest);
};

}  

#endif  
