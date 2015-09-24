// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_REQUEST_READ_TEST_H_
#define CONTENT_TEST_PLUGIN_PLUGIN_REQUEST_READ_TEST_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "content/test/plugin/plugin_test.h"

namespace NPAPIClient {

class PluginRequestReadTest : public PluginTest {
 public:
  PluginRequestReadTest(NPP id, NPNetscapeFuncs* host_functions);
  virtual ~PluginRequestReadTest();

  
  
  
  virtual NPError New(uint16 mode, int16 argc, const char* argn[],
                      const char* argv[], NPSavedData* saved) OVERRIDE;
  virtual NPError SetWindow(NPWindow* window) OVERRIDE;
  virtual NPError NewStream(NPMIMEType type, NPStream* stream,
                            NPBool seekable, uint16* stream_type) OVERRIDE;
  virtual NPError DestroyStream(NPStream *stream, NPError reason) OVERRIDE;
  virtual int32 WriteReady(NPStream* stream) OVERRIDE;
  virtual int32 Write(NPStream* stream, int32 offset, int32 len,
                      void* buffer) OVERRIDE;

 private:
  
  std::vector<NPByteRange> requested_ranges_;
  std::string url_to_request_;
  bool tests_started_;
  bool read_requested_;
};

}  

#endif  

