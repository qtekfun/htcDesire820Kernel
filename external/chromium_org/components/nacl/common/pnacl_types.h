// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_COMMON_PNACL_TYPES_H_
#define COMPONENTS_NACL_COMMON_PNACL_TYPES_H_


#include "base/time/time.h"
#include "url/gurl.h"

namespace nacl {
struct PnaclCacheInfo {
  PnaclCacheInfo();
  ~PnaclCacheInfo();
  GURL pexe_url;
  int abi_version;
  int opt_level;
  base::Time last_modified;
  std::string etag;
  bool has_no_store_header;
};

}  

#endif  
