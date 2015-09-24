/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_JSON_MANIFEST_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_JSON_MANIFEST_H_

#include <map>
#include <set>
#include <string>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_string.h"
#include "ppapi/native_client/src/trusted/plugin/manifest.h"
#include "third_party/jsoncpp/source/include/json/value.h"

namespace pp {
class URLUtil_Dev;
}  

namespace plugin {

class ErrorInfo;
class PnaclOptions;

class JsonManifest : public Manifest {
 public:
  JsonManifest(const pp::URLUtil_Dev* url_util,
           const nacl::string& manifest_base_url,
           const nacl::string& sandbox_isa)
      : url_util_(url_util),
        manifest_base_url_(manifest_base_url),
        sandbox_isa_(sandbox_isa),
        dictionary_(Json::nullValue) { }
  virtual ~JsonManifest() { }

  
  
  bool Init(const nacl::string& json, ErrorInfo* error_info);

  
  
  virtual bool GetProgramURL(nacl::string* full_url,
                             PnaclOptions* pnacl_options,
                             ErrorInfo* error_info) const;

  
  virtual bool ResolveURL(const nacl::string& relative_url,
                          nacl::string* full_url,
                          ErrorInfo* error_info) const;

  
  
  
  
  virtual bool GetFileKeys(std::set<nacl::string>* keys) const;

  
  
  
  
  virtual bool ResolveKey(const nacl::string& key,
                          nacl::string* full_url,
                          PnaclOptions* pnacl_options,
                          ErrorInfo* error_info) const;

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(JsonManifest);

  
  
  
  bool MatchesSchema(ErrorInfo* error_info);

  const pp::URLUtil_Dev* url_util_;
  nacl::string manifest_base_url_;
  nacl::string sandbox_isa_;

  Json::Value dictionary_;
};


}  

#endif  
