// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_PRIVATE_TEST_H_
#define CONTENT_TEST_PLUGIN_PLUGIN_PRIVATE_TEST_H_

#include "base/compiler_specific.h"
#include "content/test/plugin/plugin_test.h"

namespace NPAPIClient {

class PrivateTest : public PluginTest {
 public:
  PrivateTest(NPP id, NPNetscapeFuncs *host_functions);

  
  virtual NPError New(uint16 mode, int16 argc, const char* argn[],
                      const char* argv[], NPSavedData* saved) OVERRIDE;
};

}  

#endif  
