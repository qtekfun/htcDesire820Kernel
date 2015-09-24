// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_GETURL_TEST_H_
#define CONTENT_TEST_PLUGIN_PLUGIN_GETURL_TEST_H_

#include <stdio.h>

#include "base/compiler_specific.h"
#include "content/test/plugin/plugin_test.h"

namespace NPAPIClient {

class PluginGetURLTest : public PluginTest {
 public:
  
  PluginGetURLTest(NPP id, NPNetscapeFuncs *host_functions);
  virtual ~PluginGetURLTest();

  
  
  
  virtual NPError New(uint16 mode, int16 argc, const char* argn[],
                      const char* argv[], NPSavedData* saved) OVERRIDE;
  virtual NPError SetWindow(NPWindow* pNPWindow) OVERRIDE;
  virtual NPError NewStream(NPMIMEType type, NPStream* stream,
                            NPBool seekable, uint16* stype) OVERRIDE;
  virtual int32   WriteReady(NPStream *stream) OVERRIDE;
  virtual int32   Write(NPStream *stream, int32 offset, int32 len,
                        void *buffer) OVERRIDE;
  virtual NPError DestroyStream(NPStream *stream, NPError reason) OVERRIDE;
  virtual void    StreamAsFile(NPStream* stream, const char* fname) OVERRIDE;
  virtual void    URLNotify(const char* url, NPReason reason,
                            void* data) OVERRIDE;
  virtual void    URLRedirectNotify(const char* url, int32_t status,
                                    void* notify_data) OVERRIDE;

 private:
  bool tests_started_;
  int tests_in_progress_;
  std::string self_url_;
  FILE* test_file_;
  bool expect_404_response_;
  
  bool npn_evaluate_context_;
  
  
  bool handle_url_redirects_;
  bool received_url_redirect_cancel_notification_;
  bool received_url_redirect_allow_notification_;
  std::string page_not_found_url_;
  std::string fail_write_url_;
  std::string referrer_target_url_;
  bool check_cookies_;
};

}  

#endif  
