// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_THREAD_ASYNC_CALL_TEST_H_
#define CONTENT_TEST_PLUGIN_PLUGIN_THREAD_ASYNC_CALL_TEST_H_

#include "content/test/plugin/plugin_test.h"

namespace base {
class ShadowingAtExitManager;
}

namespace NPAPIClient {

class PluginThreadAsyncCallTest : public PluginTest {
 public:
  PluginThreadAsyncCallTest(NPP id, NPNetscapeFuncs *host_functions);
  virtual ~PluginThreadAsyncCallTest();

  virtual NPError New(uint16 mode, int16 argc, const char* argn[],
                      const char* argv[], NPSavedData* saved) OVERRIDE;

  virtual NPError Destroy() OVERRIDE;

  void AsyncCall();
  void OnCallSucceeded();
  void OnCallCompleted();

 private:
  
  base::ShadowingAtExitManager* at_exit_manager_;

  DISALLOW_COPY_AND_ASSIGN(PluginThreadAsyncCallTest);
};

}  

#endif  
