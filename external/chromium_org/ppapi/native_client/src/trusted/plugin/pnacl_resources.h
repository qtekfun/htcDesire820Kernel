// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PNACL_RESOURCES_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PNACL_RESOURCES_H_

#include <map>
#include <vector>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/trusted/desc/nacl_desc_wrapper.h"

#include "ppapi/c/private/pp_file_handle.h"
#include "ppapi/cpp/completion_callback.h"

#include "ppapi/native_client/src/trusted/plugin/nexe_arch.h"
#include "ppapi/native_client/src/trusted/plugin/plugin_error.h"

namespace plugin {

class Manifest;
class Plugin;
class PnaclCoordinator;

class PnaclUrls {
 public:
  
  static nacl::string GetBaseUrl();

  
  static nacl::string PrependPlatformPrefix(const nacl::string& url);

  static bool IsPnaclComponent(const nacl::string& full_url);
  static nacl::string PnaclComponentURLToFilename(
      const nacl::string& full_url);

  
  
  static const nacl::string GetResourceInfoUrl() {
    return nacl::string(kResourceInfoUrl);
  }
 private:
  static const char kResourceInfoUrl[];
};

class PnaclResources {
 public:
  PnaclResources(Plugin* plugin,
                 PnaclCoordinator* coordinator,
                 const Manifest* manifest)
      : plugin_(plugin),
        coordinator_(coordinator),
        manifest_(manifest) {
  }
  virtual ~PnaclResources();

  
  
  virtual void ReadResourceInfo(
      const nacl::string& resource_info_url,
      const pp::CompletionCallback& resource_info_read_cb);

  
  virtual void StartLoad(
      const pp::CompletionCallback& all_loaded_callback);

  const nacl::string& GetLlcUrl() {
    return llc_tool_name;
  }

  const nacl::string& GetLdUrl() {
    return ld_tool_name;
  }

  
  
  nacl::DescWrapper* WrapperForUrl(const nacl::string& url);

  static int32_t GetPnaclFD(Plugin* plugin, const char* filename);

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(PnaclResources);

  
  Plugin* plugin_;
  
  PnaclCoordinator* coordinator_;
  
  const Manifest* manifest_;
  
  
  std::map<nacl::string, nacl::DescWrapper*> resource_wrappers_;

  
  nacl::string llc_tool_name;
  nacl::string ld_tool_name;

  
  
  bool ParseResourceInfo(const nacl::string& buf, nacl::string& errmsg);

  
  
  void ReadResourceInfoError(const nacl::string& msg);
};

}  
#endif  
