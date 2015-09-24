// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_TEST_H_
#define CONTENT_TEST_PLUGIN_PLUGIN_TEST_H_

#include <string>

#include "third_party/npapi/bindings/npapi.h"
#include "third_party/npapi/bindings/nphostapi.h"

namespace NPAPIClient {

class PluginTest {
 public:
  
  PluginTest(NPP id, NPNetscapeFuncs *host_functions);

  
  virtual ~PluginTest();

  
  virtual bool IsWindowless() const;

  
  
  
  virtual NPError New(uint16 mode, int16 argc, const char* argn[],
                      const char* argv[], NPSavedData* saved);
  virtual NPError Destroy();
  virtual NPError SetWindow(NPWindow* pNPWindow);
  virtual NPError NewStream(NPMIMEType type, NPStream* stream,
                            NPBool seekable, uint16* stype);
  virtual int32   WriteReady(NPStream *stream);
  virtual int32   Write(NPStream *stream, int32 offset, int32 len,
                        void *buffer);
  virtual NPError DestroyStream(NPStream *stream, NPError reason);
  virtual void    StreamAsFile(NPStream* stream, const char* fname);
  virtual void    URLNotify(const char* url, NPReason reason, void* data);
  virtual int16   HandleEvent(void* event);
  virtual void    URLRedirectNotify(const char* url, int32_t status,
                                    void* notify_data);

  
  bool Succeeded() { return test_status_.length() == 0; }

  
  
  void SetError(const std::string &msg);

  
  
  void ExpectStringLowerCaseEqual(const std::string &val1,
                                  const std::string &val2);

  
  
  void ExpectAsciiStringNotEqual(const char *val1, const char *val2);

  
  
  void ExpectIntegerEqual(int val1, int val2);

  
  
  
  
  
  
  
  
  void SignalTestCompleted();

 protected:
  
  
  
  const char *GetArgValue(const char *name, const int16 argc,
                          const char *argn[], const char *argv[]);

  
  
  NPNetscapeFuncs *HostFunctions() { return host_functions_; }

  
  NPP id() { return id_; }
  std::string test_id() const { return test_id_; }
  std::string test_name() const { return test_name_; }
  bool test_completed() const { return test_completed_; }
 private:
  NPP                       id_;
  NPNetscapeFuncs *         host_functions_;
  std::string               test_name_;
  std::string               test_id_;
  std::string               test_status_;
  bool                      test_completed_;
};

}  

#endif  
