// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_PLUGIN_PLUGIN_CLIENT_H_
#define CONTENT_TEST_PLUGIN_PLUGIN_CLIENT_H_

#include "third_party/npapi/bindings/npapi.h"
#include "third_party/npapi/bindings/nphostapi.h"

namespace NPAPIClient {

class PluginClient {
 public:
  
  
  
  static NPError GetEntryPoints(NPPluginFuncs* pFuncs);

  
  
  
  
  
  static NPError Initialize(NPNetscapeFuncs* pFuncs);

  
  
  
  
  static NPError Shutdown();

  
  static NPNetscapeFuncs *HostFunctions() { return host_functions_; }

 private:
  static NPNetscapeFuncs* host_functions_;
};

}  

#endif  
