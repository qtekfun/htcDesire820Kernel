/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_MANIFEST_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_MANIFEST_H_

#include <map>
#include <set>
#include <string>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_string.h"
#include "third_party/jsoncpp/source/include/json/value.h"

namespace pp {
class URLUtil_Dev;
}  

namespace plugin {

class ErrorInfo;
class PnaclOptions;

class Manifest {
 public:
  Manifest() { }
  virtual ~Manifest() { }

  
  
  
  
  
  

  
  
  
  virtual bool GetProgramURL(nacl::string* full_url,
                             PnaclOptions* pnacl_options,
                             ErrorInfo* error_info) const = 0;

  
  virtual bool ResolveURL(const nacl::string& relative_url,
                          nacl::string* full_url,
                          ErrorInfo* error_info) const = 0;

  
  
  
  
  virtual bool GetFileKeys(std::set<nacl::string>* keys) const = 0;

  
  
  
  
  
  
  virtual bool ResolveKey(const nacl::string& key,
                          nacl::string* full_url,
                          PnaclOptions* pnacl_options,
                          ErrorInfo* error_info) const = 0;

 protected:
  NACL_DISALLOW_COPY_AND_ASSIGN(Manifest);
};


}  

#endif  
