// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PNACL_OPTIONS_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PNACL_OPTIONS_H_

#include <vector>

#include "native_client/src/include/nacl_string.h"
#include "native_client/src/include/portability.h"

namespace plugin {

class PnaclOptions {

 public:
  PnaclOptions();
  ~PnaclOptions();

  
  std::vector<char> GetOptCommandline() const;

  bool translate() const { return translate_; }
  void set_translate(bool t) { translate_ = t; }

  int32_t opt_level() const { return opt_level_; }
  void set_opt_level(int32_t l);

 private:
  
  
  
  bool translate_;
  int32_t opt_level_;
};

}  
#endif  
