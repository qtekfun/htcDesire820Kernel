// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_CHILD_NPAPI_PLUGIN_HOST_H_
#define CONTENT_CHILD_NPAPI_PLUGIN_HOST_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "third_party/npapi/bindings/npapi.h"
#include "third_party/npapi/bindings/nphostapi.h"

namespace content {

class PluginHost : public base::RefCounted<PluginHost> {
 public:
  
  
  static PluginHost* Singleton();

  
  NPNetscapeFuncs* host_functions() { return &host_funcs_; }

  
  
  
  
  static bool SetPostData(const char *buf,
                          uint32 length,
                          std::vector<std::string>* names,
                          std::vector<std::string>* values,
                          std::vector<char>* body);

  void PatchNPNetscapeFuncs(NPNetscapeFuncs* overrides);

 private:
  friend class base::RefCounted<PluginHost>;

  virtual ~PluginHost();

  PluginHost();
  void InitializeHostFuncs();
  NPNetscapeFuncs host_funcs_;
  DISALLOW_COPY_AND_ASSIGN(PluginHost);
};

}  

#endif  
