// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_MANAGER_HELPER_H_
#define CHROME_BROWSER_SAFE_BROWSING_PROTOCOL_MANAGER_HELPER_H_


#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

#include "base/gtest_prod_util.h"

struct SafeBrowsingProtocolConfig {
  SafeBrowsingProtocolConfig();
  ~SafeBrowsingProtocolConfig();
  std::string client_name;
  std::string url_prefix;
  std::string backup_connect_error_url_prefix;
  std::string backup_http_error_url_prefix;
  std::string backup_network_error_url_prefix;
  std::string version;
  bool disable_auto_update;
};

class SafeBrowsingProtocolManagerHelper {
 public:
  
  static std::string Version();

  
  
  
  static std::string ComposeUrl(const std::string& prefix,
                                const std::string& method,
                                const std::string& client_name,
                                const std::string& version,
                                const std::string& additional_query);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(SafeBrowsingProtocolManagerHelper);
};

#endif  
